// SPDX-License-Identifier: GPL-2.0
/*
 * Performance event support for s390x - CPU-measurement Counter Facility
 *
 *  Copyright IBM Corp. 2012, 2019
 *  Author(s): Hendrik Brueckner <brueckner@linux.ibm.com>
 */
#define KMSG_COMPONENT	"cpum_cf"
#define pr_fmt(fmt)	KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/percpu.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/export.h>
#include <asm/cpu_mcf.h>
<<<<<<< HEAD
=======
#include <asm/hwctrset.h>
#include <asm/debug.h>

static unsigned int cfdiag_cpu_speed;	/* CPU speed for CF_DIAG trailer */
static debug_info_t *cf_dbg;

#define	CF_DIAG_CTRSET_DEF		0xfeef	/* Counter set header mark */
						/* interval in seconds */

/* Counter sets are stored as data stream in a page sized memory buffer and
 * exported to user space via raw data attached to the event sample data.
 * Each counter set starts with an eight byte header consisting of:
 * - a two byte eye catcher (0xfeef)
 * - a one byte counter set number
 * - a two byte counter set size (indicates the number of counters in this set)
 * - a three byte reserved value (must be zero) to make the header the same
 *   size as a counter value.
 * All counter values are eight byte in size.
 *
 * All counter sets are followed by a 64 byte trailer.
 * The trailer consists of a:
 * - flag field indicating valid fields when corresponding bit set
 * - the counter facility first and second version number
 * - the CPU speed if nonzero
 * - the time stamp the counter sets have been collected
 * - the time of day (TOD) base value
 * - the machine type.
 *
 * The counter sets are saved when the process is prepared to be executed on a
 * CPU and saved again when the process is going to be removed from a CPU.
 * The difference of both counter sets are calculated and stored in the event
 * sample data area.
 */
struct cf_ctrset_entry {	/* CPU-M CF counter set entry (8 byte) */
	unsigned int def:16;	/* 0-15  Data Entry Format */
	unsigned int set:16;	/* 16-31 Counter set identifier */
	unsigned int ctr:16;	/* 32-47 Number of stored counters */
	unsigned int res1:16;	/* 48-63 Reserved */
};

struct cf_trailer_entry {	/* CPU-M CF_DIAG trailer (64 byte) */
	/* 0 - 7 */
	union {
		struct {
			unsigned int clock_base:1;	/* TOD clock base set */
			unsigned int speed:1;		/* CPU speed set */
			/* Measurement alerts */
			unsigned int mtda:1;	/* Loss of MT ctr. data alert */
			unsigned int caca:1;	/* Counter auth. change alert */
			unsigned int lcda:1;	/* Loss of counter data alert */
		};
		unsigned long flags;	/* 0-63    All indicators */
	};
	/* 8 - 15 */
	unsigned int cfvn:16;			/* 64-79   Ctr First Version */
	unsigned int csvn:16;			/* 80-95   Ctr Second Version */
	unsigned int cpu_speed:32;		/* 96-127  CPU speed */
	/* 16 - 23 */
	unsigned long timestamp;		/* 128-191 Timestamp (TOD) */
	/* 24 - 55 */
	union {
		struct {
			unsigned long progusage1;
			unsigned long progusage2;
			unsigned long progusage3;
			unsigned long tod_base;
		};
		unsigned long progusage[4];
	};
	/* 56 - 63 */
	unsigned int mach_type:16;		/* Machine type */
	unsigned int res1:16;			/* Reserved */
	unsigned int res2:32;			/* Reserved */
};

/* Create the trailer data at the end of a page. */
static void cfdiag_trailer(struct cf_trailer_entry *te)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	struct cpuid cpuid;

	te->cfvn = cpuhw->info.cfvn;		/* Counter version numbers */
	te->csvn = cpuhw->info.csvn;

	get_cpu_id(&cpuid);			/* Machine type */
	te->mach_type = cpuid.machine;
	te->cpu_speed = cfdiag_cpu_speed;
	if (te->cpu_speed)
		te->speed = 1;
	te->clock_base = 1;			/* Save clock base */
	te->tod_base = tod_clock_base.tod;
	te->timestamp = get_tod_clock_fast();
}

/* Read a counter set. The counter set number determines the counter set and
 * the CPUM-CF first and second version number determine the number of
 * available counters in each counter set.
 * Each counter set starts with header containing the counter set number and
 * the number of eight byte counters.
 *
 * The functions returns the number of bytes occupied by this counter set
 * including the header.
 * If there is no counter in the counter set, this counter set is useless and
 * zero is returned on this case.
 *
 * Note that the counter sets may not be enabled or active and the stcctm
 * instruction might return error 3. Depending on error_ok value this is ok,
 * for example when called from cpumf_pmu_start() call back function.
 */
static size_t cfdiag_getctrset(struct cf_ctrset_entry *ctrdata, int ctrset,
			       size_t room, bool error_ok)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	size_t ctrset_size, need = 0;
	int rc = 3;				/* Assume write failure */

	ctrdata->def = CF_DIAG_CTRSET_DEF;
	ctrdata->set = ctrset;
	ctrdata->res1 = 0;
	ctrset_size = cpum_cf_ctrset_size(ctrset, &cpuhw->info);

	if (ctrset_size) {			/* Save data */
		need = ctrset_size * sizeof(u64) + sizeof(*ctrdata);
		if (need <= room) {
			rc = ctr_stcctm(ctrset, ctrset_size,
					(u64 *)(ctrdata + 1));
		}
		if (rc != 3 || error_ok)
			ctrdata->ctr = ctrset_size;
		else
			need = 0;
	}

	debug_sprintf_event(cf_dbg, 3,
			    "%s ctrset %d ctrset_size %zu cfvn %d csvn %d"
			    " need %zd rc %d\n", __func__, ctrset, ctrset_size,
			    cpuhw->info.cfvn, cpuhw->info.csvn, need, rc);
	return need;
}

/* Read out all counter sets and save them in the provided data buffer.
 * The last 64 byte host an artificial trailer entry.
 */
static size_t cfdiag_getctr(void *data, size_t sz, unsigned long auth,
			    bool error_ok)
{
	struct cf_trailer_entry *trailer;
	size_t offset = 0, done;
	int i;

	memset(data, 0, sz);
	sz -= sizeof(*trailer);		/* Always room for trailer */
	for (i = CPUMF_CTR_SET_BASIC; i < CPUMF_CTR_SET_MAX; ++i) {
		struct cf_ctrset_entry *ctrdata = data + offset;

		if (!(auth & cpumf_ctr_ctl[i]))
			continue;	/* Counter set not authorized */

		done = cfdiag_getctrset(ctrdata, i, sz - offset, error_ok);
		offset += done;
	}
	trailer = data + offset;
	cfdiag_trailer(trailer);
	return offset + sizeof(*trailer);
}

/* Calculate the difference for each counter in a counter set. */
static void cfdiag_diffctrset(u64 *pstart, u64 *pstop, int counters)
{
	for (; --counters >= 0; ++pstart, ++pstop)
		if (*pstop >= *pstart)
			*pstop -= *pstart;
		else
			*pstop = *pstart - *pstop + 1;
}

/* Scan the counter sets and calculate the difference of each counter
 * in each set. The result is the increment of each counter during the
 * period the counter set has been activated.
 *
 * Return true on success.
 */
static int cfdiag_diffctr(struct cpu_cf_events *cpuhw, unsigned long auth)
{
	struct cf_trailer_entry *trailer_start, *trailer_stop;
	struct cf_ctrset_entry *ctrstart, *ctrstop;
	size_t offset = 0;

	auth &= (1 << CPUMF_LCCTL_ENABLE_SHIFT) - 1;
	do {
		ctrstart = (struct cf_ctrset_entry *)(cpuhw->start + offset);
		ctrstop = (struct cf_ctrset_entry *)(cpuhw->stop + offset);

		if (memcmp(ctrstop, ctrstart, sizeof(*ctrstop))) {
			pr_err_once("cpum_cf_diag counter set compare error "
				    "in set %i\n", ctrstart->set);
			return 0;
		}
		auth &= ~cpumf_ctr_ctl[ctrstart->set];
		if (ctrstart->def == CF_DIAG_CTRSET_DEF) {
			cfdiag_diffctrset((u64 *)(ctrstart + 1),
					  (u64 *)(ctrstop + 1), ctrstart->ctr);
			offset += ctrstart->ctr * sizeof(u64) +
							sizeof(*ctrstart);
		}
	} while (ctrstart->def && auth);

	/* Save time_stamp from start of event in stop's trailer */
	trailer_start = (struct cf_trailer_entry *)(cpuhw->start + offset);
	trailer_stop = (struct cf_trailer_entry *)(cpuhw->stop + offset);
	trailer_stop->progusage[0] = trailer_start->timestamp;

	return 1;
}
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

static enum cpumf_ctr_set get_counter_set(u64 event)
{
	int set = CPUMF_CTR_SET_MAX;

	if (event < 32)
		set = CPUMF_CTR_SET_BASIC;
	else if (event < 64)
		set = CPUMF_CTR_SET_USER;
	else if (event < 128)
		set = CPUMF_CTR_SET_CRYPTO;
	else if (event < 288)
		set = CPUMF_CTR_SET_EXT;
	else if (event >= 448 && event < 496)
		set = CPUMF_CTR_SET_MT_DIAG;

	return set;
}

static int validate_ctr_version(const struct hw_perf_event *hwc)
{
	struct cpu_cf_events *cpuhw;
	int err = 0;
	u16 mtdiag_ctl;

	cpuhw = &get_cpu_var(cpu_cf_events);

	/* check required version for counter sets */
	switch (hwc->config_base) {
	case CPUMF_CTR_SET_BASIC:
	case CPUMF_CTR_SET_USER:
		if (cpuhw->info.cfvn < 1)
			err = -EOPNOTSUPP;
		break;
	case CPUMF_CTR_SET_CRYPTO:
		if ((cpuhw->info.csvn >= 1 && cpuhw->info.csvn <= 5 &&
		     hwc->config > 79) ||
		    (cpuhw->info.csvn >= 6 && hwc->config > 83))
			err = -EOPNOTSUPP;
		break;
	case CPUMF_CTR_SET_EXT:
		if (cpuhw->info.csvn < 1)
			err = -EOPNOTSUPP;
		if ((cpuhw->info.csvn == 1 && hwc->config > 159) ||
		    (cpuhw->info.csvn == 2 && hwc->config > 175) ||
		    (cpuhw->info.csvn >= 3 && cpuhw->info.csvn <= 5
		     && hwc->config > 255) ||
		    (cpuhw->info.csvn >= 6 && hwc->config > 287))
			err = -EOPNOTSUPP;
		break;
	case CPUMF_CTR_SET_MT_DIAG:
		if (cpuhw->info.csvn <= 3)
			err = -EOPNOTSUPP;
		/*
		 * MT-diagnostic counters are read-only.  The counter set
		 * is automatically enabled and activated on all CPUs with
		 * multithreading (SMT).  Deactivation of multithreading
		 * also disables the counter set.  State changes are ignored
		 * by lcctl().	Because Linux controls SMT enablement through
		 * a kernel parameter only, the counter set is either disabled
		 * or enabled and active.
		 *
		 * Thus, the counters can only be used if SMT is on and the
		 * counter set is enabled and active.
		 */
		mtdiag_ctl = cpumf_ctr_ctl[CPUMF_CTR_SET_MT_DIAG];
		if (!((cpuhw->info.auth_ctl & mtdiag_ctl) &&
		      (cpuhw->info.enable_ctl & mtdiag_ctl) &&
		      (cpuhw->info.act_ctl & mtdiag_ctl)))
			err = -EOPNOTSUPP;
		break;
	}

	put_cpu_var(cpu_cf_events);
	return err;
}

static int validate_ctr_auth(const struct hw_perf_event *hwc)
{
	struct cpu_cf_events *cpuhw;
	u64 ctrs_state;
	int err = 0;

	cpuhw = &get_cpu_var(cpu_cf_events);

	/* Check authorization for cpu counter sets.
	 * If the particular CPU counter set is not authorized,
	 * return with -ENOENT in order to fall back to other
	 * PMUs that might suffice the event request.
	 */
	ctrs_state = cpumf_ctr_ctl[hwc->config_base];
	if (!(ctrs_state & cpuhw->info.auth_ctl))
		err = -ENOENT;

	put_cpu_var(cpu_cf_events);
	return err;
}

/*
 * Change the CPUMF state to active.
 * Enable and activate the CPU-counter sets according
 * to the per-cpu control state.
 */
static void cpumf_pmu_enable(struct pmu *pmu)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	int err;

	if (cpuhw->flags & PMU_F_ENABLED)
		return;

	err = lcctl(cpuhw->state);
	if (err) {
		pr_err("Enabling the performance measuring unit "
		       "failed with rc=%x\n", err);
		return;
	}

	cpuhw->flags |= PMU_F_ENABLED;
}

/*
 * Change the CPUMF state to inactive.
 * Disable and enable (inactive) the CPU-counter sets according
 * to the per-cpu control state.
 */
static void cpumf_pmu_disable(struct pmu *pmu)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	int err;
	u64 inactive;

	if (!(cpuhw->flags & PMU_F_ENABLED))
		return;

	inactive = cpuhw->state & ~((1 << CPUMF_LCCTL_ENABLE_SHIFT) - 1);
	err = lcctl(inactive);
	if (err) {
		pr_err("Disabling the performance measuring unit "
		       "failed with rc=%x\n", err);
		return;
	}

	cpuhw->flags &= ~PMU_F_ENABLED;
}


/* Number of perf events counting hardware events */
static atomic_t num_events = ATOMIC_INIT(0);
/* Used to avoid races in calling reserve/release_cpumf_hardware */
static DEFINE_MUTEX(pmc_reserve_mutex);

/* Release the PMU if event is the last perf event */
static void hw_perf_event_destroy(struct perf_event *event)
{
	if (!atomic_add_unless(&num_events, -1, 1)) {
		mutex_lock(&pmc_reserve_mutex);
		if (atomic_dec_return(&num_events) == 0)
			__kernel_cpumcf_end();
		mutex_unlock(&pmc_reserve_mutex);
	}
}

/* CPUMF <-> perf event mappings for kernel+userspace (basic set) */
static const int cpumf_generic_events_basic[] = {
	[PERF_COUNT_HW_CPU_CYCLES]	    = 0,
	[PERF_COUNT_HW_INSTRUCTIONS]	    = 1,
	[PERF_COUNT_HW_CACHE_REFERENCES]    = -1,
	[PERF_COUNT_HW_CACHE_MISSES]	    = -1,
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = -1,
	[PERF_COUNT_HW_BRANCH_MISSES]	    = -1,
	[PERF_COUNT_HW_BUS_CYCLES]	    = -1,
};
/* CPUMF <-> perf event mappings for userspace (problem-state set) */
static const int cpumf_generic_events_user[] = {
	[PERF_COUNT_HW_CPU_CYCLES]	    = 32,
	[PERF_COUNT_HW_INSTRUCTIONS]	    = 33,
	[PERF_COUNT_HW_CACHE_REFERENCES]    = -1,
	[PERF_COUNT_HW_CACHE_MISSES]	    = -1,
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = -1,
	[PERF_COUNT_HW_BRANCH_MISSES]	    = -1,
	[PERF_COUNT_HW_BUS_CYCLES]	    = -1,
};

static int __hw_perf_event_init(struct perf_event *event, unsigned int type)
{
	struct perf_event_attr *attr = &event->attr;
	struct hw_perf_event *hwc = &event->hw;
	enum cpumf_ctr_set set;
	int err = 0;
	u64 ev;

	switch (type) {
	case PERF_TYPE_RAW:
		/* Raw events are used to access counters directly,
		 * hence do not permit excludes */
		if (attr->exclude_kernel || attr->exclude_user ||
		    attr->exclude_hv)
			return -EOPNOTSUPP;
		ev = attr->config;
		break;

	case PERF_TYPE_HARDWARE:
		if (is_sampling_event(event))	/* No sampling support */
			return -ENOENT;
		ev = attr->config;
		/* Count user space (problem-state) only */
		if (!attr->exclude_user && attr->exclude_kernel) {
			if (ev >= ARRAY_SIZE(cpumf_generic_events_user))
				return -EOPNOTSUPP;
			ev = cpumf_generic_events_user[ev];

		/* No support for kernel space counters only */
		} else if (!attr->exclude_kernel && attr->exclude_user) {
			return -EOPNOTSUPP;

		/* Count user and kernel space */
		} else {
			if (ev >= ARRAY_SIZE(cpumf_generic_events_basic))
				return -EOPNOTSUPP;
			ev = cpumf_generic_events_basic[ev];
		}
		break;

	default:
		return -ENOENT;
	}

	if (ev == -1)
		return -ENOENT;

	if (ev > PERF_CPUM_CF_MAX_CTR)
		return -ENOENT;

	/* Obtain the counter set to which the specified counter belongs */
	set = get_counter_set(ev);
	switch (set) {
	case CPUMF_CTR_SET_BASIC:
	case CPUMF_CTR_SET_USER:
	case CPUMF_CTR_SET_CRYPTO:
	case CPUMF_CTR_SET_EXT:
	case CPUMF_CTR_SET_MT_DIAG:
		/*
		 * Use the hardware perf event structure to store the
		 * counter number in the 'config' member and the counter
		 * set number in the 'config_base'.  The counter set number
		 * is then later used to enable/disable the counter(s).
		 */
		hwc->config = ev;
		hwc->config_base = set;
		break;
	case CPUMF_CTR_SET_MAX:
		/* The counter could not be associated to a counter set */
		return -EINVAL;
	};

	/* Initialize for using the CPU-measurement counter facility */
	if (!atomic_inc_not_zero(&num_events)) {
		mutex_lock(&pmc_reserve_mutex);
		if (atomic_read(&num_events) == 0 && __kernel_cpumcf_begin())
			err = -EBUSY;
		else
			atomic_inc(&num_events);
		mutex_unlock(&pmc_reserve_mutex);
	}
	if (err)
		return err;
	event->destroy = hw_perf_event_destroy;

	/* Finally, validate version and authorization of the counter set */
	err = validate_ctr_auth(hwc);
	if (!err)
		err = validate_ctr_version(hwc);

	return err;
}

static int cpumf_pmu_event_init(struct perf_event *event)
{
	unsigned int type = event->attr.type;
	int err;

	if (type == PERF_TYPE_HARDWARE || type == PERF_TYPE_RAW)
		err = __hw_perf_event_init(event, type);
	else if (event->pmu->type == type)
		/* Registered as unknown PMU */
		err = __hw_perf_event_init(event, PERF_TYPE_RAW);
	else
		return -ENOENT;

	if (unlikely(err) && event->destroy)
		event->destroy(event);

	return err;
}

static int hw_perf_event_reset(struct perf_event *event)
{
	u64 prev, new;
	int err;

	do {
		prev = local64_read(&event->hw.prev_count);
		err = ecctr(event->hw.config, &new);
		if (err) {
			if (err != 3)
				break;
			/* The counter is not (yet) available. This
			 * might happen if the counter set to which
			 * this counter belongs is in the disabled
			 * state.
			 */
			new = 0;
		}
	} while (local64_cmpxchg(&event->hw.prev_count, prev, new) != prev);

	return err;
}

static void hw_perf_event_update(struct perf_event *event)
{
	u64 prev, new, delta;
	int err;

	do {
		prev = local64_read(&event->hw.prev_count);
		err = ecctr(event->hw.config, &new);
		if (err)
			return;
	} while (local64_cmpxchg(&event->hw.prev_count, prev, new) != prev);

	delta = (prev <= new) ? new - prev
			      : (-1ULL - prev) + new + 1;	 /* overflow */
	local64_add(delta, &event->count);
}

static void cpumf_pmu_read(struct perf_event *event)
{
	if (event->hw.state & PERF_HES_STOPPED)
		return;

	hw_perf_event_update(event);
}

static void cpumf_pmu_start(struct perf_event *event, int flags)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	struct hw_perf_event *hwc = &event->hw;

	if (WARN_ON_ONCE(!(hwc->state & PERF_HES_STOPPED)))
		return;

	if (WARN_ON_ONCE(hwc->config == -1))
		return;

	if (flags & PERF_EF_RELOAD)
		WARN_ON_ONCE(!(hwc->state & PERF_HES_UPTODATE));

	hwc->state = 0;

	/* (Re-)enable and activate the counter set */
	ctr_set_enable(&cpuhw->state, hwc->config_base);
	ctr_set_start(&cpuhw->state, hwc->config_base);

	/* The counter set to which this counter belongs can be already active.
	 * Because all counters in a set are active, the event->hw.prev_count
	 * needs to be synchronized.  At this point, the counter set can be in
	 * the inactive or disabled state.
	 */
	hw_perf_event_reset(event);

	/* increment refcount for this counter set */
	atomic_inc(&cpuhw->ctr_set[hwc->config_base]);
}

static void cpumf_pmu_stop(struct perf_event *event, int flags)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	struct hw_perf_event *hwc = &event->hw;

	if (!(hwc->state & PERF_HES_STOPPED)) {
		/* Decrement reference count for this counter set and if this
		 * is the last used counter in the set, clear activation
		 * control and set the counter set state to inactive.
		 */
		if (!atomic_dec_return(&cpuhw->ctr_set[hwc->config_base]))
			ctr_set_stop(&cpuhw->state, hwc->config_base);
		event->hw.state |= PERF_HES_STOPPED;
	}

	if ((flags & PERF_EF_UPDATE) && !(hwc->state & PERF_HES_UPTODATE)) {
		hw_perf_event_update(event);
		event->hw.state |= PERF_HES_UPTODATE;
	}
}

static int cpumf_pmu_add(struct perf_event *event, int flags)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);

	/* Check authorization for the counter set to which this
	 * counter belongs.
	 * For group events transaction, the authorization check is
	 * done in cpumf_pmu_commit_txn().
	 */
	if (!(cpuhw->txn_flags & PERF_PMU_TXN_ADD))
		if (validate_ctr_auth(&event->hw))
			return -ENOENT;

	ctr_set_enable(&cpuhw->state, event->hw.config_base);
	event->hw.state = PERF_HES_UPTODATE | PERF_HES_STOPPED;

	if (flags & PERF_EF_START)
		cpumf_pmu_start(event, PERF_EF_RELOAD);

	perf_event_update_userpage(event);

	return 0;
}

static void cpumf_pmu_del(struct perf_event *event, int flags)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);

	cpumf_pmu_stop(event, PERF_EF_UPDATE);

	/* Check if any counter in the counter set is still used.  If not used,
	 * change the counter set to the disabled state.  This also clears the
	 * content of all counters in the set.
	 *
	 * When a new perf event has been added but not yet started, this can
	 * clear enable control and resets all counters in a set.  Therefore,
	 * cpumf_pmu_start() always has to reenable a counter set.
	 */
	if (!atomic_read(&cpuhw->ctr_set[event->hw.config_base]))
		ctr_set_disable(&cpuhw->state, event->hw.config_base);

	perf_event_update_userpage(event);
}

/*
 * Start group events scheduling transaction.
 * Set flags to perform a single test at commit time.
 *
 * We only support PERF_PMU_TXN_ADD transactions. Save the
 * transaction flags but otherwise ignore non-PERF_PMU_TXN_ADD
 * transactions.
 */
static void cpumf_pmu_start_txn(struct pmu *pmu, unsigned int txn_flags)
{
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);

	WARN_ON_ONCE(cpuhw->txn_flags);		/* txn already in flight */

	cpuhw->txn_flags = txn_flags;
	if (txn_flags & ~PERF_PMU_TXN_ADD)
		return;

	perf_pmu_disable(pmu);
	cpuhw->tx_state = cpuhw->state;
}

/*
 * Stop and cancel a group events scheduling tranctions.
 * Assumes cpumf_pmu_del() is called for each successful added
 * cpumf_pmu_add() during the transaction.
 */
static void cpumf_pmu_cancel_txn(struct pmu *pmu)
{
	unsigned int txn_flags;
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);

	WARN_ON_ONCE(!cpuhw->txn_flags);	/* no txn in flight */

	txn_flags = cpuhw->txn_flags;
	cpuhw->txn_flags = 0;
	if (txn_flags & ~PERF_PMU_TXN_ADD)
		return;

	WARN_ON(cpuhw->tx_state != cpuhw->state);

	perf_pmu_enable(pmu);
}

/*
 * Commit the group events scheduling transaction.  On success, the
 * transaction is closed.   On error, the transaction is kept open
 * until cpumf_pmu_cancel_txn() is called.
 */
static int cpumf_pmu_commit_txn(struct pmu *pmu)
{
<<<<<<< HEAD
	struct cpu_cf_events *cpuhw = this_cpu_ptr(&cpu_cf_events);
	u64 state;
=======
	int ret;

	get_online_cpus();
	mutex_lock(&cfset_ctrset_mutex);
	switch (cmd) {
	case S390_HWCTR_START:
		ret = cfset_ioctl_start(arg);
		break;
	case S390_HWCTR_STOP:
		ret = cfset_ioctl_stop();
		break;
	case S390_HWCTR_READ:
		ret = cfset_ioctl_read(arg);
		break;
	default:
		ret = -ENOTTY;
		break;
	}
	mutex_unlock(&cfset_ctrset_mutex);
	put_online_cpus();
	return ret;
}

static const struct file_operations cfset_fops = {
	.owner = THIS_MODULE,
	.open = cfset_open,
	.release = cfset_release,
	.unlocked_ioctl	= cfset_ioctl,
	.compat_ioctl = cfset_ioctl,
	.llseek = no_llseek
};

static struct miscdevice cfset_dev = {
	.name	= S390_HWCTR_DEVICE,
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &cfset_fops,
};
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

	WARN_ON_ONCE(!cpuhw->txn_flags);	/* no txn in flight */

	if (cpuhw->txn_flags & ~PERF_PMU_TXN_ADD) {
		cpuhw->txn_flags = 0;
		return 0;
	}

	/* check if the updated state can be scheduled */
	state = cpuhw->state & ~((1 << CPUMF_LCCTL_ENABLE_SHIFT) - 1);
	state >>= CPUMF_LCCTL_ENABLE_SHIFT;
	if ((state & cpuhw->info.auth_ctl) != state)
		return -ENOENT;

	cpuhw->txn_flags = 0;
	perf_pmu_enable(pmu);
	return 0;
}

/* Performance monitoring unit for s390x */
static struct pmu cpumf_pmu = {
	.task_ctx_nr  = perf_sw_context,
	.capabilities = PERF_PMU_CAP_NO_INTERRUPT,
	.pmu_enable   = cpumf_pmu_enable,
	.pmu_disable  = cpumf_pmu_disable,
	.event_init   = cpumf_pmu_event_init,
	.add	      = cpumf_pmu_add,
	.del	      = cpumf_pmu_del,
	.start	      = cpumf_pmu_start,
	.stop	      = cpumf_pmu_stop,
	.read	      = cpumf_pmu_read,
	.start_txn    = cpumf_pmu_start_txn,
	.commit_txn   = cpumf_pmu_commit_txn,
	.cancel_txn   = cpumf_pmu_cancel_txn,
};

static int __init cpumf_pmu_init(void)
{
	int rc;

	if (!kernel_cpumcf_avail())
		return -ENODEV;

	cpumf_pmu.attr_groups = cpumf_cf_event_group();
	rc = perf_pmu_register(&cpumf_pmu, "cpum_cf", -1);
	if (rc)
		pr_err("Registering the cpum_cf PMU failed with rc=%i\n", rc);
	return rc;
}
subsys_initcall(cpumf_pmu_init);
