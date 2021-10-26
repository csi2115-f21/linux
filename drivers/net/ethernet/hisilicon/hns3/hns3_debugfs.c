// SPDX-License-Identifier: GPL-2.0+
/* Copyright (c) 2018-2019 Hisilicon Limited. */

#include <linux/debugfs.h>
#include <linux/device.h>

#include "hnae3.h"
#include "hns3_enet.h"

<<<<<<< HEAD
#define HNS3_DBG_READ_LEN 65536
#define HNS3_DBG_WRITE_LEN 1024
=======
static struct dentry *hns3_dbgfs_root;

static struct hns3_dbg_dentry_info hns3_dbg_dentry[] = {
	{
		.name = "tm"
	},
	{
		.name = "tx_bd_info"
	},
	{
		.name = "rx_bd_info"
	},
	{
		.name = "mac_list"
	},
	{
		.name = "reg"
	},
	{
		.name = "queue"
	},
	{
		.name = "fd"
	},
	/* keep common at the bottom and add new directory above */
	{
		.name = "common"
	},
};

static int hns3_dbg_bd_file_init(struct hnae3_handle *handle, unsigned int cmd);
static int hns3_dbg_common_file_init(struct hnae3_handle *handle,
				     unsigned int cmd);

static struct hns3_dbg_cmd_info hns3_dbg_cmd[] = {
	{
		.name = "tm_nodes",
		.cmd = HNAE3_DBG_CMD_TM_NODES,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tm_priority",
		.cmd = HNAE3_DBG_CMD_TM_PRI,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tm_qset",
		.cmd = HNAE3_DBG_CMD_TM_QSET,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tm_map",
		.cmd = HNAE3_DBG_CMD_TM_MAP,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN_1MB,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tm_pg",
		.cmd = HNAE3_DBG_CMD_TM_PG,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tm_port",
		.cmd = HNAE3_DBG_CMD_TM_PORT,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tc_sch_info",
		.cmd = HNAE3_DBG_CMD_TC_SCH_INFO,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "qos_pause_cfg",
		.cmd = HNAE3_DBG_CMD_QOS_PAUSE_CFG,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "qos_pri_map",
		.cmd = HNAE3_DBG_CMD_QOS_PRI_MAP,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "qos_buf_cfg",
		.cmd = HNAE3_DBG_CMD_QOS_BUF_CFG,
		.dentry = HNS3_DBG_DENTRY_TM,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "dev_info",
		.cmd = HNAE3_DBG_CMD_DEV_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tx_bd_queue",
		.cmd = HNAE3_DBG_CMD_TX_BD,
		.dentry = HNS3_DBG_DENTRY_TX_BD,
		.buf_len = HNS3_DBG_READ_LEN_4MB,
		.init = hns3_dbg_bd_file_init,
	},
	{
		.name = "rx_bd_queue",
		.cmd = HNAE3_DBG_CMD_RX_BD,
		.dentry = HNS3_DBG_DENTRY_RX_BD,
		.buf_len = HNS3_DBG_READ_LEN_4MB,
		.init = hns3_dbg_bd_file_init,
	},
	{
		.name = "uc",
		.cmd = HNAE3_DBG_CMD_MAC_UC,
		.dentry = HNS3_DBG_DENTRY_MAC,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "mc",
		.cmd = HNAE3_DBG_CMD_MAC_MC,
		.dentry = HNS3_DBG_DENTRY_MAC,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "mng_tbl",
		.cmd = HNAE3_DBG_CMD_MNG_TBL,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "loopback",
		.cmd = HNAE3_DBG_CMD_LOOPBACK,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "interrupt_info",
		.cmd = HNAE3_DBG_CMD_INTERRUPT_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "reset_info",
		.cmd = HNAE3_DBG_CMD_RESET_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "imp_info",
		.cmd = HNAE3_DBG_CMD_IMP_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "ncl_config",
		.cmd = HNAE3_DBG_CMD_NCL_CONFIG,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN_128KB,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "mac_tnl_status",
		.cmd = HNAE3_DBG_CMD_MAC_TNL_STATUS,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "bios_common",
		.cmd = HNAE3_DBG_CMD_REG_BIOS_COMMON,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "ssu",
		.cmd = HNAE3_DBG_CMD_REG_SSU,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "igu_egu",
		.cmd = HNAE3_DBG_CMD_REG_IGU_EGU,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "rpu",
		.cmd = HNAE3_DBG_CMD_REG_RPU,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "ncsi",
		.cmd = HNAE3_DBG_CMD_REG_NCSI,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "rtc",
		.cmd = HNAE3_DBG_CMD_REG_RTC,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "ppp",
		.cmd = HNAE3_DBG_CMD_REG_PPP,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "rcb",
		.cmd = HNAE3_DBG_CMD_REG_RCB,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tqp",
		.cmd = HNAE3_DBG_CMD_REG_TQP,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "mac",
		.cmd = HNAE3_DBG_CMD_REG_MAC,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "dcb",
		.cmd = HNAE3_DBG_CMD_REG_DCB,
		.dentry = HNS3_DBG_DENTRY_REG,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "queue_map",
		.cmd = HNAE3_DBG_CMD_QUEUE_MAP,
		.dentry = HNS3_DBG_DENTRY_QUEUE,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "rx_queue_info",
		.cmd = HNAE3_DBG_CMD_RX_QUEUE_INFO,
		.dentry = HNS3_DBG_DENTRY_QUEUE,
		.buf_len = HNS3_DBG_READ_LEN_1MB,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "tx_queue_info",
		.cmd = HNAE3_DBG_CMD_TX_QUEUE_INFO,
		.dentry = HNS3_DBG_DENTRY_QUEUE,
		.buf_len = HNS3_DBG_READ_LEN_1MB,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "fd_tcam",
		.cmd = HNAE3_DBG_CMD_FD_TCAM,
		.dentry = HNS3_DBG_DENTRY_FD,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "service_task_info",
		.cmd = HNAE3_DBG_CMD_SERV_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "vlan_config",
		.cmd = HNAE3_DBG_CMD_VLAN_CONFIG,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "ptp_info",
		.cmd = HNAE3_DBG_CMD_PTP_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "fd_counter",
		.cmd = HNAE3_DBG_CMD_FD_COUNTER,
		.dentry = HNS3_DBG_DENTRY_FD,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
	{
		.name = "umv_info",
		.cmd = HNAE3_DBG_CMD_UMV_INFO,
		.dentry = HNS3_DBG_DENTRY_COMMON,
		.buf_len = HNS3_DBG_READ_LEN,
		.init = hns3_dbg_common_file_init,
	},
};

static struct hns3_dbg_cap_info hns3_dbg_cap[] = {
	{
		.name = "support FD",
		.cap_bit = HNAE3_DEV_SUPPORT_FD_B,
	}, {
		.name = "support GRO",
		.cap_bit = HNAE3_DEV_SUPPORT_GRO_B,
	}, {
		.name = "support FEC",
		.cap_bit = HNAE3_DEV_SUPPORT_FEC_B,
	}, {
		.name = "support UDP GSO",
		.cap_bit = HNAE3_DEV_SUPPORT_UDP_GSO_B,
	}, {
		.name = "support PTP",
		.cap_bit = HNAE3_DEV_SUPPORT_PTP_B,
	}, {
		.name = "support INT QL",
		.cap_bit = HNAE3_DEV_SUPPORT_INT_QL_B,
	}, {
		.name = "support HW TX csum",
		.cap_bit = HNAE3_DEV_SUPPORT_HW_TX_CSUM_B,
	}, {
		.name = "support UDP tunnel csum",
		.cap_bit = HNAE3_DEV_SUPPORT_UDP_TUNNEL_CSUM_B,
	}, {
		.name = "support TX push",
		.cap_bit = HNAE3_DEV_SUPPORT_TX_PUSH_B,
	}, {
		.name = "support imp-controlled PHY",
		.cap_bit = HNAE3_DEV_SUPPORT_PHY_IMP_B,
	}, {
		.name = "support imp-controlled RAS",
		.cap_bit = HNAE3_DEV_SUPPORT_RAS_IMP_B,
	}, {
		.name = "support rxd advanced layout",
		.cap_bit = HNAE3_DEV_SUPPORT_RXD_ADV_LAYOUT_B,
	}, {
		.name = "support port vlan bypass",
		.cap_bit = HNAE3_DEV_SUPPORT_PORT_VLAN_BYPASS_B,
	}, {
		.name = "support modify vlan filter state",
		.cap_bit = HNAE3_DEV_SUPPORT_VLAN_FLTR_MDF_B,
	}
};

static void hns3_dbg_fill_content(char *content, u16 len,
				  const struct hns3_dbg_item *items,
				  const char **result, u16 size)
{
	char *pos = content;
	u16 i;

	memset(content, ' ', len);
	for (i = 0; i < size; i++) {
		if (result)
			strncpy(pos, result[i], strlen(result[i]));
		else
			strncpy(pos, items[i].name, strlen(items[i].name));

		pos += strlen(items[i].name) + items[i].interval;
	}

	*pos++ = '\n';
	*pos++ = '\0';
}

static const struct hns3_dbg_item tx_spare_info_items[] = {
	{ "QUEUE_ID", 2 },
	{ "COPYBREAK", 2 },
	{ "LEN", 7 },
	{ "NTU", 4 },
	{ "NTC", 4 },
	{ "LTC", 4 },
	{ "DMA", 17 },
};

static void hns3_dbg_tx_spare_info(struct hns3_enet_ring *ring, char *buf,
				   int len, u32 ring_num, int *pos)
{
	char data_str[ARRAY_SIZE(tx_spare_info_items)][HNS3_DBG_DATA_STR_LEN];
	struct hns3_tx_spare *tx_spare = ring->tx_spare;
	char *result[ARRAY_SIZE(tx_spare_info_items)];
	char content[HNS3_DBG_INFO_LEN];
	u32 i, j;

	if (!tx_spare) {
		*pos += scnprintf(buf + *pos, len - *pos,
				  "tx spare buffer is not enabled\n");
		return;
	}

	for (i = 0; i < ARRAY_SIZE(tx_spare_info_items); i++)
		result[i] = &data_str[i][0];

	*pos += scnprintf(buf + *pos, len - *pos, "tx spare buffer info\n");
	hns3_dbg_fill_content(content, sizeof(content), tx_spare_info_items,
			      NULL, ARRAY_SIZE(tx_spare_info_items));
	*pos += scnprintf(buf + *pos, len - *pos, "%s", content);

	for (i = 0; i < ring_num; i++) {
		j = 0;
		sprintf(result[j++], "%8u", i);
		sprintf(result[j++], "%9u", ring->tx_copybreak);
		sprintf(result[j++], "%3u", tx_spare->len);
		sprintf(result[j++], "%3u", tx_spare->next_to_use);
		sprintf(result[j++], "%3u", tx_spare->next_to_clean);
		sprintf(result[j++], "%3u", tx_spare->last_to_clean);
		sprintf(result[j++], "%pad", &tx_spare->dma);
		hns3_dbg_fill_content(content, sizeof(content),
				      tx_spare_info_items,
				      (const char **)result,
				      ARRAY_SIZE(tx_spare_info_items));
		*pos += scnprintf(buf + *pos, len - *pos, "%s", content);
	}
}

static const struct hns3_dbg_item rx_queue_info_items[] = {
	{ "QUEUE_ID", 2 },
	{ "BD_NUM", 2 },
	{ "BD_LEN", 2 },
	{ "TAIL", 2 },
	{ "HEAD", 2 },
	{ "FBDNUM", 2 },
	{ "PKTNUM", 2 },
	{ "COPYBREAK", 2 },
	{ "RING_EN", 2 },
	{ "RX_RING_EN", 2 },
	{ "BASE_ADDR", 10 },
};
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

static struct dentry *hns3_dbgfs_root;

static int hns3_dbg_queue_info(struct hnae3_handle *h,
			       const char *cmd_buf)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(h->pdev);
	struct hns3_nic_priv *priv = h->priv;
	struct hns3_enet_ring *ring;
	u32 base_add_l, base_add_h;
	u32 queue_num, queue_max;
	u32 value, i;
	int cnt;

	if (!priv->ring) {
		dev_err(&h->pdev->dev, "priv->ring is NULL\n");
		return -EFAULT;
	}

	queue_max = h->kinfo.num_tqps;
	cnt = kstrtouint(&cmd_buf[11], 0, &queue_num);
	if (cnt)
		queue_num = 0;
	else
		queue_max = queue_num + 1;

	dev_info(&h->pdev->dev, "queue info\n");

	if (queue_num >= h->kinfo.num_tqps) {
		dev_err(&h->pdev->dev,
			"Queue number(%u) is out of range(0-%u)\n", queue_num,
			h->kinfo.num_tqps - 1);
		return -EINVAL;
	}

	for (i = queue_num; i < queue_max; i++) {
		/* Each cycle needs to determine whether the instance is reset,
		 * to prevent reference to invalid memory. And need to ensure
		 * that the following code is executed within 100ms.
		 */
		if (!test_bit(HNS3_NIC_STATE_INITED, &priv->state) ||
		    test_bit(HNS3_NIC_STATE_RESETTING, &priv->state))
			return -EPERM;

		ring = &priv->ring[(u32)(i + h->kinfo.num_tqps)];
		base_add_h = readl_relaxed(ring->tqp->io_base +
					   HNS3_RING_RX_RING_BASEADDR_H_REG);
		base_add_l = readl_relaxed(ring->tqp->io_base +
					   HNS3_RING_RX_RING_BASEADDR_L_REG);
		dev_info(&h->pdev->dev, "RX(%u) BASE ADD: 0x%08x%08x\n", i,
			 base_add_h, base_add_l);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_BD_NUM_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING BD NUM: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_BD_LEN_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING BD LEN: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_TAIL_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING TAIL: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_HEAD_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING HEAD: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_FBDNUM_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING FBDNUM: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_RX_RING_PKTNUM_RECORD_REG);
		dev_info(&h->pdev->dev, "RX(%u) RING PKTNUM: %u\n", i, value);

		ring = &priv->ring[i];
		base_add_h = readl_relaxed(ring->tqp->io_base +
					   HNS3_RING_TX_RING_BASEADDR_H_REG);
		base_add_l = readl_relaxed(ring->tqp->io_base +
					   HNS3_RING_TX_RING_BASEADDR_L_REG);
		dev_info(&h->pdev->dev, "TX(%u) BASE ADD: 0x%08x%08x\n", i,
			 base_add_h, base_add_l);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_BD_NUM_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING BD NUM: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_TC_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING TC: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_TAIL_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING TAIL: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_HEAD_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING HEAD: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_FBDNUM_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING FBDNUM: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_OFFSET_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING OFFSET: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base +
				      HNS3_RING_TX_RING_PKTNUM_RECORD_REG);
		dev_info(&h->pdev->dev, "TX(%u) RING PKTNUM: %u\n", i, value);

		value = readl_relaxed(ring->tqp->io_base + HNS3_RING_EN_REG);
		dev_info(&h->pdev->dev, "TX/RX(%u) RING EN: %s\n", i,
			 value ? "enable" : "disable");

		if (hnae3_ae_dev_tqp_txrx_indep_supported(ae_dev)) {
			value = readl_relaxed(ring->tqp->io_base +
					      HNS3_RING_TX_EN_REG);
			dev_info(&h->pdev->dev, "TX(%u) RING EN: %s\n", i,
				 value ? "enable" : "disable");

			value = readl_relaxed(ring->tqp->io_base +
					      HNS3_RING_RX_EN_REG);
			dev_info(&h->pdev->dev, "RX(%u) RING EN: %s\n", i,
				 value ? "enable" : "disable");
		}

		dev_info(&h->pdev->dev, "\n");
	}

	return 0;
}

static int hns3_dbg_queue_map(struct hnae3_handle *h)
{
	struct hns3_nic_priv *priv = h->priv;
	int i;

	if (!h->ae_algo->ops->get_global_queue_id)
		return -EOPNOTSUPP;

	dev_info(&h->pdev->dev, "map info for queue id and vector id\n");
	dev_info(&h->pdev->dev,
		 "local queue id | global queue id | vector id\n");
	for (i = 0; i < h->kinfo.num_tqps; i++) {
		u16 global_qid;

		global_qid = h->ae_algo->ops->get_global_queue_id(h, i);
		if (!priv->ring || !priv->ring[i].tqp_vector)
			continue;
<<<<<<< HEAD

		dev_info(&h->pdev->dev,
			 "      %4d            %4u            %4d\n",
			 i, global_qid, priv->ring[i].tqp_vector->vector_irq);
=======
		j = 0;
		sprintf(result[j++], "%u", i);
		sprintf(result[j++], "%u",
			h->ae_algo->ops->get_global_queue_id(h, i));
		sprintf(result[j++], "%u",
			priv->ring[i].tqp_vector->vector_irq);
		hns3_dbg_fill_content(content, sizeof(content), queue_map_items,
				      (const char **)result,
				      ARRAY_SIZE(queue_map_items));
		pos += scnprintf(buf + pos, len - pos, "%s", content);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	}

	return 0;
}

static int hns3_dbg_bd_info(struct hnae3_handle *h, const char *cmd_buf)
{
	struct hns3_nic_priv *priv = h->priv;
	struct hns3_desc *rx_desc, *tx_desc;
	struct device *dev = &h->pdev->dev;
	struct hns3_enet_ring *ring;
	u32 tx_index, rx_index;
	u32 q_num, value;
	dma_addr_t addr;
	u16 mss_hw_csum;
	u32 l234info;
	int cnt;

	cnt = sscanf(&cmd_buf[8], "%u %u", &q_num, &tx_index);
	if (cnt == 2) {
		rx_index = tx_index;
	} else if (cnt != 1) {
		dev_err(dev, "bd info: bad command string, cnt=%d\n", cnt);
		return -EINVAL;
	}

	if (q_num >= h->kinfo.num_tqps) {
		dev_err(dev, "Queue number(%u) is out of range(0-%u)\n", q_num,
			h->kinfo.num_tqps - 1);
		return -EINVAL;
	}

	ring = &priv->ring[q_num];
	value = readl_relaxed(ring->tqp->io_base + HNS3_RING_TX_RING_TAIL_REG);
	tx_index = (cnt == 1) ? value : tx_index;

	if (tx_index >= ring->desc_num) {
		dev_err(dev, "bd index(%u) is out of range(0-%u)\n", tx_index,
			ring->desc_num - 1);
		return -EINVAL;
	}

	tx_desc = &ring->desc[tx_index];
	addr = le64_to_cpu(tx_desc->addr);
	mss_hw_csum = le16_to_cpu(tx_desc->tx.mss_hw_csum);
	dev_info(dev, "TX Queue Num: %u, BD Index: %u\n", q_num, tx_index);
	dev_info(dev, "(TX)addr: %pad\n", &addr);
	dev_info(dev, "(TX)vlan_tag: %u\n", le16_to_cpu(tx_desc->tx.vlan_tag));
	dev_info(dev, "(TX)send_size: %u\n",
		 le16_to_cpu(tx_desc->tx.send_size));

	if (mss_hw_csum & BIT(HNS3_TXD_HW_CS_B)) {
		u32 offset = le32_to_cpu(tx_desc->tx.ol_type_vlan_len_msec);
		u32 start = le32_to_cpu(tx_desc->tx.type_cs_vlan_tso_len);

		dev_info(dev, "(TX)csum start: %u\n",
			 hnae3_get_field(start,
					 HNS3_TXD_CSUM_START_M,
					 HNS3_TXD_CSUM_START_S));
		dev_info(dev, "(TX)csum offset: %u\n",
			 hnae3_get_field(offset,
					 HNS3_TXD_CSUM_OFFSET_M,
					 HNS3_TXD_CSUM_OFFSET_S));
	} else {
		dev_info(dev, "(TX)vlan_tso: %u\n",
			 tx_desc->tx.type_cs_vlan_tso);
		dev_info(dev, "(TX)l2_len: %u\n", tx_desc->tx.l2_len);
		dev_info(dev, "(TX)l3_len: %u\n", tx_desc->tx.l3_len);
		dev_info(dev, "(TX)l4_len: %u\n", tx_desc->tx.l4_len);
		dev_info(dev, "(TX)vlan_msec: %u\n",
			 tx_desc->tx.ol_type_vlan_msec);
		dev_info(dev, "(TX)ol2_len: %u\n", tx_desc->tx.ol2_len);
		dev_info(dev, "(TX)ol3_len: %u\n", tx_desc->tx.ol3_len);
		dev_info(dev, "(TX)ol4_len: %u\n", tx_desc->tx.ol4_len);
	}

	dev_info(dev, "(TX)vlan_tag: %u\n",
		 le16_to_cpu(tx_desc->tx.outer_vlan_tag));
	dev_info(dev, "(TX)tv: %u\n", le16_to_cpu(tx_desc->tx.tv));
	dev_info(dev, "(TX)paylen_ol4cs: %u\n",
		 le32_to_cpu(tx_desc->tx.paylen_ol4cs));
	dev_info(dev, "(TX)vld_ra_ri: %u\n",
		 le16_to_cpu(tx_desc->tx.bdtp_fe_sc_vld_ra_ri));
	dev_info(dev, "(TX)mss_hw_csum: %u\n", mss_hw_csum);

	ring = &priv->ring[q_num + h->kinfo.num_tqps];
	value = readl_relaxed(ring->tqp->io_base + HNS3_RING_RX_RING_TAIL_REG);
	rx_index = (cnt == 1) ? value : tx_index;
	rx_desc = &ring->desc[rx_index];

	addr = le64_to_cpu(rx_desc->addr);
	l234info = le32_to_cpu(rx_desc->rx.l234_info);
	dev_info(dev, "RX Queue Num: %u, BD Index: %u\n", q_num, rx_index);
	dev_info(dev, "(RX)addr: %pad\n", &addr);
	dev_info(dev, "(RX)l234_info: %u\n", l234info);

	if (l234info & BIT(HNS3_RXD_L2_CSUM_B)) {
		u32 lo, hi;

		lo = hnae3_get_field(l234info, HNS3_RXD_L2_CSUM_L_M,
				     HNS3_RXD_L2_CSUM_L_S);
		hi = hnae3_get_field(l234info, HNS3_RXD_L2_CSUM_H_M,
				     HNS3_RXD_L2_CSUM_H_S);
		dev_info(dev, "(RX)csum: %u\n", lo | hi << 8);
	}

<<<<<<< HEAD
	dev_info(dev, "(RX)pkt_len: %u\n", le16_to_cpu(rx_desc->rx.pkt_len));
	dev_info(dev, "(RX)size: %u\n", le16_to_cpu(rx_desc->rx.size));
	dev_info(dev, "(RX)rss_hash: %u\n", le32_to_cpu(rx_desc->rx.rss_hash));
	dev_info(dev, "(RX)fd_id: %u\n", le16_to_cpu(rx_desc->rx.fd_id));
	dev_info(dev, "(RX)vlan_tag: %u\n", le16_to_cpu(rx_desc->rx.vlan_tag));
	dev_info(dev, "(RX)o_dm_vlan_id_fb: %u\n",
		 le16_to_cpu(rx_desc->rx.o_dm_vlan_id_fb));
	dev_info(dev, "(RX)ot_vlan_tag: %u\n",
		 le16_to_cpu(rx_desc->rx.ot_vlan_tag));
	dev_info(dev, "(RX)bd_base_info: %u\n",
		 le32_to_cpu(rx_desc->rx.bd_base_info));
=======
static const struct hns3_dbg_item tx_bd_info_items[] = {
	{ "BD_IDX", 5 },
	{ "ADDRESS", 2 },
	{ "VLAN_TAG", 2 },
	{ "SIZE", 2 },
	{ "T_CS_VLAN_TSO", 2 },
	{ "OT_VLAN_TAG", 3 },
	{ "TV", 2 },
	{ "OLT_VLAN_LEN", 2},
	{ "PAYLEN_OL4CS", 2},
	{ "BD_FE_SC_VLD", 2},
	{ "MSS_HW_CSUM", 0},
};
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

	return 0;
}

static void hns3_dbg_help(struct hnae3_handle *h)
{
#define HNS3_DBG_BUF_LEN 256

	char printf_buf[HNS3_DBG_BUF_LEN];

	dev_info(&h->pdev->dev, "available commands\n");
	dev_info(&h->pdev->dev, "queue info <number>\n");
	dev_info(&h->pdev->dev, "queue map\n");
	dev_info(&h->pdev->dev, "bd info <q_num> <bd index>\n");
	dev_info(&h->pdev->dev, "dev capability\n");
	dev_info(&h->pdev->dev, "dev spec\n");

	if (!hns3_is_phys_func(h->pdev))
		return;

	dev_info(&h->pdev->dev, "dump fd tcam\n");
	dev_info(&h->pdev->dev, "dump tc\n");
	dev_info(&h->pdev->dev, "dump tm map <q_num>\n");
	dev_info(&h->pdev->dev, "dump tm\n");
	dev_info(&h->pdev->dev, "dump qos pause cfg\n");
	dev_info(&h->pdev->dev, "dump qos pri map\n");
	dev_info(&h->pdev->dev, "dump qos buf cfg\n");
	dev_info(&h->pdev->dev, "dump mng tbl\n");
	dev_info(&h->pdev->dev, "dump reset info\n");
	dev_info(&h->pdev->dev, "dump m7 info\n");
	dev_info(&h->pdev->dev, "dump ncl_config <offset> <length>(in hex)\n");
	dev_info(&h->pdev->dev, "dump mac tnl status\n");
	dev_info(&h->pdev->dev, "dump loopback\n");
	dev_info(&h->pdev->dev, "dump qs shaper [qs id]\n");
	dev_info(&h->pdev->dev, "dump uc mac list <func id>\n");
	dev_info(&h->pdev->dev, "dump mc mac list <func id>\n");
	dev_info(&h->pdev->dev, "dump intr\n");

	memset(printf_buf, 0, HNS3_DBG_BUF_LEN);
	strncat(printf_buf, "dump reg [[bios common] [ssu <port_id>]",
		HNS3_DBG_BUF_LEN - 1);
	strncat(printf_buf + strlen(printf_buf),
		" [igu egu <port_id>] [rpu <tc_queue_num>]",
		HNS3_DBG_BUF_LEN - strlen(printf_buf) - 1);
	strncat(printf_buf + strlen(printf_buf),
		" [rtc] [ppp] [rcb] [tqp <queue_num>] [mac]]\n",
		HNS3_DBG_BUF_LEN - strlen(printf_buf) - 1);
	dev_info(&h->pdev->dev, "%s", printf_buf);

	memset(printf_buf, 0, HNS3_DBG_BUF_LEN);
	strncat(printf_buf, "dump reg dcb <port_id> <pri_id> <pg_id>",
		HNS3_DBG_BUF_LEN - 1);
	strncat(printf_buf + strlen(printf_buf), " <rq_id> <nq_id> <qset_id>\n",
		HNS3_DBG_BUF_LEN - strlen(printf_buf) - 1);
	dev_info(&h->pdev->dev, "%s", printf_buf);
}

static void hns3_dbg_dev_caps(struct hnae3_handle *h)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(h->pdev);
<<<<<<< HEAD
	unsigned long *caps;

	caps = ae_dev->caps;

	dev_info(&h->pdev->dev, "support FD: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_FD_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support GRO: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_GRO_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support FEC: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_FEC_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support UDP GSO: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_UDP_GSO_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support PTP: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_PTP_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support INT QL: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_INT_QL_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support HW TX csum: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_HW_TX_CSUM_B, caps) ? "yes" : "no");
	dev_info(&h->pdev->dev, "support UDP tunnel csum: %s\n",
		 test_bit(HNAE3_DEV_SUPPORT_UDP_TUNNEL_CSUM_B, caps) ?
		 "yes" : "no");
=======
	static const char * const str[] = {"no", "yes"};
	unsigned long *caps = ae_dev->caps;
	u32 i, state;

	*pos += scnprintf(buf + *pos, len - *pos, "dev capability:\n");

	for (i = 0; i < ARRAY_SIZE(hns3_dbg_cap); i++) {
		state = test_bit(hns3_dbg_cap[i].cap_bit, caps);
		*pos += scnprintf(buf + *pos, len - *pos, "%s: %s\n",
				  hns3_dbg_cap[i].name, str[state]);
	}

	*pos += scnprintf(buf + *pos, len - *pos, "\n");
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static void hns3_dbg_dev_specs(struct hnae3_handle *h)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(h->pdev);
	struct hnae3_dev_specs *dev_specs = &ae_dev->dev_specs;
	struct hnae3_knic_private_info *kinfo = &h->kinfo;
	struct hns3_nic_priv *priv  = h->priv;

	dev_info(priv->dev, "MAC entry num: %u\n", dev_specs->mac_entry_num);
	dev_info(priv->dev, "MNG entry num: %u\n", dev_specs->mng_entry_num);
	dev_info(priv->dev, "MAX non tso bd num: %u\n",
		 dev_specs->max_non_tso_bd_num);
	dev_info(priv->dev, "RSS ind tbl size: %u\n",
		 dev_specs->rss_ind_tbl_size);
	dev_info(priv->dev, "RSS key size: %u\n", dev_specs->rss_key_size);
	dev_info(priv->dev, "RSS size: %u\n", kinfo->rss_size);
	dev_info(priv->dev, "Allocated RSS size: %u\n", kinfo->req_rss_size);
	dev_info(priv->dev, "Task queue pairs numbers: %u\n", kinfo->num_tqps);

	dev_info(priv->dev, "RX buffer length: %u\n", kinfo->rx_buf_len);
	dev_info(priv->dev, "Desc num per TX queue: %u\n", kinfo->num_tx_desc);
	dev_info(priv->dev, "Desc num per RX queue: %u\n", kinfo->num_rx_desc);
	dev_info(priv->dev, "Total number of enabled TCs: %u\n",
		 kinfo->tc_info.num_tc);
	dev_info(priv->dev, "MAX INT QL: %u\n", dev_specs->int_ql_max);
	dev_info(priv->dev, "MAX INT GL: %u\n", dev_specs->max_int_gl);
	dev_info(priv->dev, "MAX frame size: %u\n", dev_specs->max_frm_size);
	dev_info(priv->dev, "MAX TM RATE: %uMbps\n", dev_specs->max_tm_rate);
	dev_info(priv->dev, "MAX QSET number: %u\n", dev_specs->max_qset_num);
}

static ssize_t hns3_dbg_cmd_read(struct file *filp, char __user *buffer,
				 size_t count, loff_t *ppos)
{
	int uncopy_bytes;
	char *buf;
	int len;

	if (*ppos != 0)
		return 0;

	if (count < HNS3_DBG_READ_LEN)
		return -ENOSPC;

	buf = kzalloc(HNS3_DBG_READ_LEN, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

<<<<<<< HEAD
	len = scnprintf(buf, HNS3_DBG_READ_LEN, "%s\n",
			"Please echo help to cmd to get help information");
	uncopy_bytes = copy_to_user(buffer, buf, len);

	kfree(buf);

	if (uncopy_bytes)
		return -EFAULT;

	return (*ppos = len);
=======
static int hns3_dbg_get_cmd_index(struct hnae3_handle *handle,
				  const unsigned char *name, u32 *index)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(hns3_dbg_cmd); i++) {
		if (!strncmp(name, hns3_dbg_cmd[i].name,
			     strlen(hns3_dbg_cmd[i].name))) {
			*index = i;
			return 0;
		}
	}

	dev_err(&handle->pdev->dev, "unknown command(%s)\n", name);
	return -EINVAL;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

static int hns3_dbg_check_cmd(struct hnae3_handle *handle, char *cmd_buf)
{
	int ret = 0;

	if (strncmp(cmd_buf, "help", 4) == 0)
		hns3_dbg_help(handle);
	else if (strncmp(cmd_buf, "queue info", 10) == 0)
		ret = hns3_dbg_queue_info(handle, cmd_buf);
	else if (strncmp(cmd_buf, "queue map", 9) == 0)
		ret = hns3_dbg_queue_map(handle);
	else if (strncmp(cmd_buf, "bd info", 7) == 0)
		ret = hns3_dbg_bd_info(handle, cmd_buf);
	else if (strncmp(cmd_buf, "dev capability", 14) == 0)
		hns3_dbg_dev_caps(handle);
	else if (strncmp(cmd_buf, "dev spec", 8) == 0)
		hns3_dbg_dev_specs(handle);
	else if (handle->ae_algo->ops->dbg_run_cmd)
		ret = handle->ae_algo->ops->dbg_run_cmd(handle, cmd_buf);
	else
		ret = -EOPNOTSUPP;

	return ret;
}

static ssize_t hns3_dbg_cmd_write(struct file *filp, const char __user *buffer,
				  size_t count, loff_t *ppos)
{
	struct hnae3_handle *handle = filp->private_data;
	struct hns3_nic_priv *priv  = handle->priv;
	char *cmd_buf, *cmd_buf_tmp;
	int uncopied_bytes;
	int ret;

	if (*ppos != 0)
		return 0;

	/* Judge if the instance is being reset. */
	if (!test_bit(HNS3_NIC_STATE_INITED, &priv->state) ||
	    test_bit(HNS3_NIC_STATE_RESETTING, &priv->state))
		return 0;

	if (count > HNS3_DBG_WRITE_LEN)
		return -ENOSPC;

	cmd_buf = kzalloc(count + 1, GFP_KERNEL);
	if (!cmd_buf)
		return count;

	uncopied_bytes = copy_from_user(cmd_buf, buffer, count);
	if (uncopied_bytes) {
		kfree(cmd_buf);
		return -EFAULT;
	}

	cmd_buf[count] = '\0';

	cmd_buf_tmp = strchr(cmd_buf, '\n');
	if (cmd_buf_tmp) {
		*cmd_buf_tmp = '\0';
		count = cmd_buf_tmp - cmd_buf + 1;
	}

	ret = hns3_dbg_check_cmd(handle, cmd_buf);
	if (ret)
		hns3_dbg_help(handle);

	kfree(cmd_buf);
	cmd_buf = NULL;

	return count;
}

static ssize_t hns3_dbg_read(struct file *filp, char __user *buffer,
			     size_t count, loff_t *ppos)
{
	struct hnae3_handle *handle = filp->private_data;
	const struct hnae3_ae_ops *ops = handle->ae_algo->ops;
	struct hns3_nic_priv *priv = handle->priv;
	char *cmd_buf, *read_buf;
	ssize_t size = 0;
<<<<<<< HEAD
	int ret = 0;
=======
	char **save_buf;
	char *read_buf;
	u32 index;
	int ret;

	ret = hns3_dbg_get_cmd_index(handle, filp->f_path.dentry->d_iname,
				     &index);
	if (ret)
		return ret;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

	read_buf = kzalloc(HNS3_DBG_READ_LEN, GFP_KERNEL);
	if (!read_buf)
		return -ENOMEM;

	cmd_buf = filp->f_path.dentry->d_iname;

	if (ops->dbg_read_cmd)
		ret = ops->dbg_read_cmd(handle, cmd_buf, read_buf,
					HNS3_DBG_READ_LEN);

	if (ret) {
		dev_info(priv->dev, "unknown command\n");
		goto out;
	}

	size = simple_read_from_buffer(buffer, count, ppos, read_buf,
				       strlen(read_buf));

out:
	kfree(read_buf);
	return size;
}

static const struct file_operations hns3_dbg_cmd_fops = {
	.owner = THIS_MODULE,
	.open  = simple_open,
	.read  = hns3_dbg_cmd_read,
	.write = hns3_dbg_cmd_write,
};

static const struct file_operations hns3_dbg_fops = {
	.owner = THIS_MODULE,
	.open  = simple_open,
	.read  = hns3_dbg_read,
};

<<<<<<< HEAD
void hns3_dbg_init(struct hnae3_handle *handle)
=======
static int hns3_dbg_bd_file_init(struct hnae3_handle *handle, u32 cmd)
{
	struct dentry *entry_dir;
	struct hns3_dbg_data *data;
	u16 max_queue_num;
	unsigned int i;

	entry_dir = hns3_dbg_dentry[hns3_dbg_cmd[cmd].dentry].dentry;
	max_queue_num = hns3_get_max_available_channels(handle);
	data = devm_kzalloc(&handle->pdev->dev, max_queue_num * sizeof(*data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	for (i = 0; i < max_queue_num; i++) {
		char name[HNS3_DBG_FILE_NAME_LEN];

		data[i].handle = handle;
		data[i].qid = i;
		sprintf(name, "%s%u", hns3_dbg_cmd[cmd].name, i);
		debugfs_create_file(name, 0400, entry_dir, &data[i],
				    &hns3_dbg_fops);
	}

	return 0;
}

static int
hns3_dbg_common_file_init(struct hnae3_handle *handle, u32 cmd)
{
	struct hns3_dbg_data *data;
	struct dentry *entry_dir;

	data = devm_kzalloc(&handle->pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->handle = handle;
	entry_dir = hns3_dbg_dentry[hns3_dbg_cmd[cmd].dentry].dentry;
	debugfs_create_file(hns3_dbg_cmd[cmd].name, 0400, entry_dir,
			    data, &hns3_dbg_fops);

	return 0;
}

int hns3_dbg_init(struct hnae3_handle *handle)
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(handle->pdev);
	const char *name = pci_name(handle->pdev);
	struct dentry *entry_dir;

	handle->hnae3_dbgfs = debugfs_create_dir(name, hns3_dbgfs_root);

	debugfs_create_file("cmd", 0600, handle->hnae3_dbgfs, handle,
			    &hns3_dbg_cmd_fops);

	entry_dir = debugfs_create_dir("tm", handle->hnae3_dbgfs);
	if (ae_dev->dev_version > HNAE3_DEVICE_VERSION_V2)
		debugfs_create_file(HNAE3_DBG_TM_NODES, 0600, entry_dir, handle,
				    &hns3_dbg_fops);
	debugfs_create_file(HNAE3_DBG_TM_PRI, 0600, entry_dir, handle,
			    &hns3_dbg_fops);
	debugfs_create_file(HNAE3_DBG_TM_QSET, 0600, entry_dir, handle,
			    &hns3_dbg_fops);
}

void hns3_dbg_uninit(struct hnae3_handle *handle)
{
	debugfs_remove_recursive(handle->hnae3_dbgfs);
	handle->hnae3_dbgfs = NULL;
}

void hns3_dbg_register_debugfs(const char *debugfs_dir_name)
{
	hns3_dbgfs_root = debugfs_create_dir(debugfs_dir_name, NULL);
}

void hns3_dbg_unregister_debugfs(void)
{
	debugfs_remove_recursive(hns3_dbgfs_root);
	hns3_dbgfs_root = NULL;
}
