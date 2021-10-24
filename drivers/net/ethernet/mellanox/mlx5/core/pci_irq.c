// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/* Copyright (c) 2019 Mellanox Technologies. */

#include <linux/interrupt.h>
#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/mlx5/driver.h>
#include "mlx5_core.h"
#ifdef CONFIG_RFS_ACCEL
#include <linux/cpu_rmap.h>
#endif

#define MLX5_MAX_IRQ_NAME (32)
<<<<<<< HEAD
=======
/* max irq_index is 255. three chars */
#define MLX5_MAX_IRQ_IDX_CHARS (3)

#define MLX5_SFS_PER_CTRL_IRQ 64
#define MLX5_IRQ_CTRL_SF_MAX 8
/* min num of vectores for SFs to be enabled */
#define MLX5_IRQ_VEC_COMP_BASE_SF 2

#define MLX5_EQ_SHARE_IRQ_MAX_COMP (8)
#define MLX5_EQ_SHARE_IRQ_MAX_CTRL (UINT_MAX)
#define MLX5_EQ_SHARE_IRQ_MIN_COMP (1)
#define MLX5_EQ_SHARE_IRQ_MIN_CTRL (4)
#define MLX5_EQ_REFS_PER_IRQ (2)
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

struct mlx5_irq {
	u32 index;
	struct atomic_notifier_head nh;
	cpumask_var_t mask;
	char name[MLX5_MAX_IRQ_NAME];
<<<<<<< HEAD
=======
	struct kref kref;
	int irqn;
	struct mlx5_irq_pool *pool;
};

struct mlx5_irq_pool {
	char name[MLX5_MAX_IRQ_NAME - MLX5_MAX_IRQ_IDX_CHARS];
	struct xa_limit xa_num_irqs;
	struct mutex lock; /* sync IRQs creations */
	struct xarray irqs;
	u32 max_threshold;
	u32 min_threshold;
	struct mlx5_core_dev *dev;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
};

struct mlx5_irq_table {
	struct mlx5_irq *irq;
	int nvec;
#ifdef CONFIG_RFS_ACCEL
	struct cpu_rmap *rmap;
#endif
};

int mlx5_irq_table_init(struct mlx5_core_dev *dev)
{
	struct mlx5_irq_table *irq_table;

	if (mlx5_core_is_sf(dev))
		return 0;

	irq_table = kvzalloc(sizeof(*irq_table), GFP_KERNEL);
	if (!irq_table)
		return -ENOMEM;

	dev->priv.irq_table = irq_table;
	return 0;
}

void mlx5_irq_table_cleanup(struct mlx5_core_dev *dev)
{
	if (mlx5_core_is_sf(dev))
		return;

	kvfree(dev->priv.irq_table);
}

<<<<<<< HEAD
int mlx5_irq_get_num_comp(struct mlx5_irq_table *table)
{
	return table->nvec - MLX5_IRQ_VEC_COMP_BASE;
=======
static void irq_release(struct kref *kref)
{
	struct mlx5_irq *irq = container_of(kref, struct mlx5_irq, kref);
	struct mlx5_irq_pool *pool = irq->pool;

	xa_erase(&pool->irqs, irq->index);
	/* free_irq requires that affinity and rmap will be cleared
	 * before calling it. This is why there is asymmetry with set_rmap
	 * which should be called after alloc_irq but before request_irq.
	 */
	irq_set_affinity_hint(irq->irqn, NULL);
	free_cpumask_var(irq->mask);
	free_irq(irq->irqn, &irq->nh);
	kfree(irq);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static struct mlx5_irq *mlx5_irq_get(struct mlx5_core_dev *dev, int vecidx)
{
	struct mlx5_irq_table *irq_table = dev->priv.irq_table;

<<<<<<< HEAD
	return &irq_table->irq[vecidx];
}

int mlx5_irq_attach_nb(struct mlx5_irq_table *irq_table, int vecidx,
		       struct notifier_block *nb)
{
	struct mlx5_irq *irq;

	irq = &irq_table->irq[vecidx];
	return atomic_notifier_chain_register(&irq->nh, nb);
}

int mlx5_irq_detach_nb(struct mlx5_irq_table *irq_table, int vecidx,
		       struct notifier_block *nb)
{
	struct mlx5_irq *irq;

	irq = &irq_table->irq[vecidx];
	return atomic_notifier_chain_unregister(&irq->nh, nb);
}

static irqreturn_t mlx5_irq_int_handler(int irq, void *nh)
=======
	mutex_lock(&pool->lock);
	kref_put(&irq->kref, irq_release);
	mutex_unlock(&pool->lock);
}

static irqreturn_t irq_int_handler(int irq, void *nh)
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
{
	atomic_notifier_call_chain(nh, 0, NULL);
	return IRQ_HANDLED;
}

static void irq_set_name(char *name, int vecidx)
{
	if (vecidx == 0) {
		snprintf(name, MLX5_MAX_IRQ_NAME, "mlx5_async");
		return;
	}

	snprintf(name, MLX5_MAX_IRQ_NAME, "mlx5_comp%d",
		 vecidx - MLX5_IRQ_VEC_COMP_BASE);
	return;
}

static int request_irqs(struct mlx5_core_dev *dev, int nvec)
{
	char name[MLX5_MAX_IRQ_NAME];
	int err;
	int i;

	for (i = 0; i < nvec; i++) {
		struct mlx5_irq *irq = mlx5_irq_get(dev, i);
		int irqn = pci_irq_vector(dev->pdev, i);

		irq_set_name(name, i);
<<<<<<< HEAD
		ATOMIC_INIT_NOTIFIER_HEAD(&irq->nh);
		snprintf(irq->name, MLX5_MAX_IRQ_NAME,
			 "%s@pci:%s", name, pci_name(dev->pdev));
		err = request_irq(irqn, mlx5_irq_int_handler, 0, irq->name,
				  &irq->nh);
		if (err) {
			mlx5_core_err(dev, "Failed to request irq\n");
			goto err_request_irq;
		}
	}
	return 0;

err_request_irq:
	while (i--) {
		struct mlx5_irq *irq = mlx5_irq_get(dev, i);
		int irqn = pci_irq_vector(dev->pdev, i);

		free_irq(irqn, &irq->nh);
	}
	return  err;
=======
	else
		irq_sf_set_name(pool, name, i);
	ATOMIC_INIT_NOTIFIER_HEAD(&irq->nh);
	snprintf(irq->name, MLX5_MAX_IRQ_NAME,
		 "%s@pci:%s", name, pci_name(dev->pdev));
	err = request_irq(irq->irqn, irq_int_handler, 0, irq->name,
			  &irq->nh);
	if (err) {
		mlx5_core_err(dev, "Failed to request irq. err = %d\n", err);
		goto err_req_irq;
	}
	if (!zalloc_cpumask_var(&irq->mask, GFP_KERNEL)) {
		mlx5_core_warn(dev, "zalloc_cpumask_var failed\n");
		err = -ENOMEM;
		goto err_cpumask;
	}
	kref_init(&irq->kref);
	irq->index = i;
	err = xa_err(xa_store(&pool->irqs, irq->index, irq, GFP_KERNEL));
	if (err) {
		mlx5_core_err(dev, "Failed to alloc xa entry for irq(%u). err = %d\n",
			      irq->index, err);
		goto err_xa;
	}
	irq->pool = pool;
	return irq;
err_xa:
	free_cpumask_var(irq->mask);
err_cpumask:
	free_irq(irq->irqn, &irq->nh);
err_req_irq:
	kfree(irq);
	return ERR_PTR(err);
}

int mlx5_irq_attach_nb(struct mlx5_irq *irq, struct notifier_block *nb)
{
	int err;

	err = kref_get_unless_zero(&irq->kref);
	if (WARN_ON_ONCE(!err))
		/* Something very bad happens here, we are enabling EQ
		 * on non-existing IRQ.
		 */
		return -ENOENT;
	err = atomic_notifier_chain_register(&irq->nh, nb);
	if (err)
		irq_put(irq);
	return err;
}

int mlx5_irq_detach_nb(struct mlx5_irq *irq, struct notifier_block *nb)
{
	irq_put(irq);
	return atomic_notifier_chain_unregister(&irq->nh, nb);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static void irq_clear_rmap(struct mlx5_core_dev *dev)
{
#ifdef CONFIG_RFS_ACCEL
	struct mlx5_irq_table *irq_table = dev->priv.irq_table;

	free_irq_cpu_rmap(irq_table->rmap);
#endif
}

static int irq_set_rmap(struct mlx5_core_dev *mdev)
{
	int err = 0;
#ifdef CONFIG_RFS_ACCEL
	struct mlx5_irq_table *irq_table = mdev->priv.irq_table;
	int num_affinity_vec;
	int vecidx;

<<<<<<< HEAD
	num_affinity_vec = mlx5_irq_get_num_comp(irq_table);
	irq_table->rmap = alloc_irq_cpu_rmap(num_affinity_vec);
	if (!irq_table->rmap) {
		err = -ENOMEM;
		mlx5_core_err(mdev, "Failed to allocate cpu_rmap. err %d", err);
		goto err_out;
=======
/* looking for the irq with the smallest refcount and the same affinity */
static struct mlx5_irq *irq_pool_find_least_loaded(struct mlx5_irq_pool *pool,
						   struct cpumask *affinity)
{
	int start = pool->xa_num_irqs.min;
	int end = pool->xa_num_irqs.max;
	struct mlx5_irq *irq = NULL;
	struct mlx5_irq *iter;
	unsigned long index;

	lockdep_assert_held(&pool->lock);
	xa_for_each_range(&pool->irqs, index, iter, start, end) {
		if (!cpumask_equal(iter->mask, affinity))
			continue;
		if (kref_read(&iter->kref) < pool->min_threshold)
			return iter;
		if (!irq || kref_read(&iter->kref) <
		    kref_read(&irq->kref))
			irq = iter;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	}

<<<<<<< HEAD
	vecidx = MLX5_IRQ_VEC_COMP_BASE;
	for (; vecidx < irq_table->nvec; vecidx++) {
		err = irq_cpu_rmap_add(irq_table->rmap,
				       pci_irq_vector(mdev->pdev, vecidx));
		if (err) {
			mlx5_core_err(mdev, "irq_cpu_rmap_add failed. err %d",
				      err);
			goto err_irq_cpu_rmap_add;
		}
=======
/* requesting an irq from a given pool according to given affinity */
static struct mlx5_irq *irq_pool_request_affinity(struct mlx5_irq_pool *pool,
						  struct cpumask *affinity)
{
	struct mlx5_irq *least_loaded_irq, *new_irq;

	mutex_lock(&pool->lock);
	least_loaded_irq = irq_pool_find_least_loaded(pool, affinity);
	if (least_loaded_irq &&
	    kref_read(&least_loaded_irq->kref) < pool->min_threshold)
		goto out;
	new_irq = irq_pool_create_irq(pool, affinity);
	if (IS_ERR(new_irq)) {
		if (!least_loaded_irq) {
			mlx5_core_err(pool->dev, "Didn't find IRQ for cpu = %u\n",
				      cpumask_first(affinity));
			mutex_unlock(&pool->lock);
			return new_irq;
		}
		/* We failed to create a new IRQ for the requested affinity,
		 * sharing existing IRQ.
		 */
		goto out;
	}
	least_loaded_irq = new_irq;
	goto unlock;
out:
	kref_get(&least_loaded_irq->kref);
	if (kref_read(&least_loaded_irq->kref) > pool->max_threshold)
		mlx5_core_dbg(pool->dev, "IRQ %u overloaded, pool_name: %s, %u EQs on this irq\n",
			      least_loaded_irq->irqn, pool->name,
			      kref_read(&least_loaded_irq->kref) / MLX5_EQ_REFS_PER_IRQ);
unlock:
	mutex_unlock(&pool->lock);
	return least_loaded_irq;
}

/* requesting an irq from a given pool according to given index */
static struct mlx5_irq *
irq_pool_request_vector(struct mlx5_irq_pool *pool, int vecidx,
			struct cpumask *affinity)
{
	struct mlx5_irq *irq;

	mutex_lock(&pool->lock);
	irq = xa_load(&pool->irqs, vecidx);
	if (irq) {
		kref_get(&irq->kref);
		goto unlock;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	}
	return 0;

err_irq_cpu_rmap_add:
	irq_clear_rmap(mdev);
err_out:
#endif
	return err;
}

/* Completion IRQ vectors */

static int set_comp_irq_affinity_hint(struct mlx5_core_dev *mdev, int i)
{
	int vecidx = MLX5_IRQ_VEC_COMP_BASE + i;
	struct mlx5_irq *irq;
	int irqn;

	irq = mlx5_irq_get(mdev, vecidx);
	irqn = pci_irq_vector(mdev->pdev, vecidx);
	if (!zalloc_cpumask_var(&irq->mask, GFP_KERNEL)) {
		mlx5_core_warn(mdev, "zalloc_cpumask_var failed");
		return -ENOMEM;
	}
<<<<<<< HEAD

	cpumask_set_cpu(cpumask_local_spread(i, mdev->priv.numa_node),
			irq->mask);
	if (IS_ENABLED(CONFIG_SMP) &&
	    irq_set_affinity_hint(irqn, irq->mask))
		mlx5_core_warn(mdev, "irq_set_affinity_hint failed, irq 0x%.4x",
			       irqn);

	return 0;
=======
pf_irq:
	pool = irq_table->pf_pool;
	irq = irq_pool_request_vector(pool, vecidx, affinity);
out:
	if (IS_ERR(irq))
		return irq;
	mlx5_core_dbg(dev, "irq %u mapped to cpu %*pbl, %u EQs on this irq\n",
		      irq->irqn, cpumask_pr_args(affinity),
		      kref_read(&irq->kref) / MLX5_EQ_REFS_PER_IRQ);
	return irq;
}

static struct mlx5_irq_pool *
irq_pool_alloc(struct mlx5_core_dev *dev, int start, int size, char *name,
	       u32 min_threshold, u32 max_threshold)
{
	struct mlx5_irq_pool *pool = kvzalloc(sizeof(*pool), GFP_KERNEL);

	if (!pool)
		return ERR_PTR(-ENOMEM);
	pool->dev = dev;
	xa_init_flags(&pool->irqs, XA_FLAGS_ALLOC);
	pool->xa_num_irqs.min = start;
	pool->xa_num_irqs.max = start + size - 1;
	if (name)
		snprintf(pool->name, MLX5_MAX_IRQ_NAME - MLX5_MAX_IRQ_IDX_CHARS,
			 name);
	pool->min_threshold = min_threshold * MLX5_EQ_REFS_PER_IRQ;
	pool->max_threshold = max_threshold * MLX5_EQ_REFS_PER_IRQ;
	mutex_init(&pool->lock);
	mlx5_core_dbg(dev, "pool->name = %s, pool->size = %d, pool->start = %d",
		      name, size, start);
	return pool;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static void clear_comp_irq_affinity_hint(struct mlx5_core_dev *mdev, int i)
{
	int vecidx = MLX5_IRQ_VEC_COMP_BASE + i;
	struct mlx5_irq *irq;
	int irqn;

<<<<<<< HEAD
	irq = mlx5_irq_get(mdev, vecidx);
	irqn = pci_irq_vector(mdev->pdev, vecidx);
	irq_set_affinity_hint(irqn, NULL);
	free_cpumask_var(irq->mask);
=======
	xa_for_each(&pool->irqs, index, irq)
		irq_release(&irq->kref);
	xa_destroy(&pool->irqs);
	kvfree(pool);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static int set_comp_irq_affinity_hints(struct mlx5_core_dev *mdev)
{
	int nvec = mlx5_irq_get_num_comp(mdev->priv.irq_table);
	int err;
	int i;

<<<<<<< HEAD
	for (i = 0; i < nvec; i++) {
		err = set_comp_irq_affinity_hint(mdev, i);
		if (err)
			goto err_out;
=======
	/* init pf_pool */
	table->pf_pool = irq_pool_alloc(dev, 0, pf_vec, NULL,
					MLX5_EQ_SHARE_IRQ_MIN_COMP,
					MLX5_EQ_SHARE_IRQ_MAX_COMP);
	if (IS_ERR(table->pf_pool))
		return PTR_ERR(table->pf_pool);
	if (!mlx5_sf_max_functions(dev))
		return 0;
	if (sf_vec < MLX5_IRQ_VEC_COMP_BASE_SF) {
		mlx5_core_err(dev, "Not enough IRQs for SFs. SF may run at lower performance\n");
		return 0;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	}

	return 0;

err_out:
	for (i--; i >= 0; i--)
		clear_comp_irq_affinity_hint(mdev, i);

	return err;
}

static void clear_comp_irqs_affinity_hints(struct mlx5_core_dev *mdev)
{
	int nvec = mlx5_irq_get_num_comp(mdev->priv.irq_table);
	int i;

	for (i = 0; i < nvec; i++)
		clear_comp_irq_affinity_hint(mdev, i);
}

struct cpumask *
mlx5_irq_get_affinity_mask(struct mlx5_irq_table *irq_table, int vecidx)
{
	return irq_table->irq[vecidx].mask;
}

#ifdef CONFIG_RFS_ACCEL
struct cpu_rmap *mlx5_irq_get_rmap(struct mlx5_irq_table *irq_table)
{
	return irq_table->rmap;
}
#endif

static void unrequest_irqs(struct mlx5_core_dev *dev)
{
	struct mlx5_irq_table *table = dev->priv.irq_table;
	int i;

	for (i = 0; i < table->nvec; i++)
		free_irq(pci_irq_vector(dev->pdev, i),
			 &mlx5_irq_get(dev, i)->nh);
}

int mlx5_irq_table_create(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	struct mlx5_irq_table *table = priv->irq_table;
	int num_eqs = MLX5_CAP_GEN(dev, max_num_eqs) ?
		      MLX5_CAP_GEN(dev, max_num_eqs) :
		      1 << MLX5_CAP_GEN(dev, log_max_eq);
	int nvec;
	int err;

	if (mlx5_core_is_sf(dev))
		return 0;

	nvec = MLX5_CAP_GEN(dev, num_ports) * num_online_cpus() +
	       MLX5_IRQ_VEC_COMP_BASE;
	nvec = min_t(int, nvec, num_eqs);
	if (nvec <= MLX5_IRQ_VEC_COMP_BASE)
		return -ENOMEM;

	table->irq = kcalloc(nvec, sizeof(*table->irq), GFP_KERNEL);
	if (!table->irq)
		return -ENOMEM;

	nvec = pci_alloc_irq_vectors(dev->pdev, MLX5_IRQ_VEC_COMP_BASE + 1,
				     nvec, PCI_IRQ_MSIX);
	if (nvec < 0) {
		err = nvec;
		goto err_free_irq;
	}

	table->nvec = nvec;

	err = irq_set_rmap(dev);
	if (err)
		goto err_set_rmap;

	err = request_irqs(dev, nvec);
	if (err)
		goto err_request_irqs;

	err = set_comp_irq_affinity_hints(dev);
	if (err) {
		mlx5_core_err(dev, "Failed to alloc affinity hint cpumask\n");
		goto err_set_affinity;
	}

	return 0;

err_set_affinity:
	unrequest_irqs(dev);
err_request_irqs:
	irq_clear_rmap(dev);
err_set_rmap:
	pci_free_irq_vectors(dev->pdev);
err_free_irq:
	kfree(table->irq);
	return err;
}

void mlx5_irq_table_destroy(struct mlx5_core_dev *dev)
{
	struct mlx5_irq_table *table = dev->priv.irq_table;
	int i;

	if (mlx5_core_is_sf(dev))
		return;

<<<<<<< HEAD
	/* free_irq requires that affinity and rmap will be cleared
	 * before calling it. This is why there is asymmetry with set_rmap
	 * which should be called after alloc_irq but before request_irq.
=======
	/* There are cases where IRQs still will be in used when we reaching
	 * to here. Hence, making sure all the irqs are realeased.
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	 */
	irq_clear_rmap(dev);
	clear_comp_irqs_affinity_hints(dev);
	for (i = 0; i < table->nvec; i++)
		free_irq(pci_irq_vector(dev->pdev, i),
			 &mlx5_irq_get(dev, i)->nh);
	pci_free_irq_vectors(dev->pdev);
<<<<<<< HEAD
	kfree(table->irq);
=======
}

int mlx5_irq_table_get_sfs_vec(struct mlx5_irq_table *table)
{
	if (table->sf_comp_pool)
		return table->sf_comp_pool->xa_num_irqs.max -
			table->sf_comp_pool->xa_num_irqs.min + 1;
	else
		return mlx5_irq_table_get_num_comp(table);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

struct mlx5_irq_table *mlx5_irq_table_get(struct mlx5_core_dev *dev)
{
#ifdef CONFIG_MLX5_SF
	if (mlx5_core_is_sf(dev))
		return dev->priv.parent_mdev->priv.irq_table;
#endif
	return dev->priv.irq_table;
}
