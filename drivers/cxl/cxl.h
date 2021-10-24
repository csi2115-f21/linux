/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright(c) 2020 Intel Corporation. */

#ifndef __CXL_H__
#define __CXL_H__

#include <linux/bitfield.h>
#include <linux/bitops.h>
#include <linux/io.h>

/* CXL 2.0 8.2.8.1 Device Capabilities Array Register */
#define CXLDEV_CAP_ARRAY_OFFSET 0x0
#define   CXLDEV_CAP_ARRAY_CAP_ID 0
#define   CXLDEV_CAP_ARRAY_ID_MASK GENMASK_ULL(15, 0)
#define   CXLDEV_CAP_ARRAY_COUNT_MASK GENMASK_ULL(47, 32)
/* CXL 2.0 8.2.8.2 CXL Device Capability Header Register */
#define CXLDEV_CAP_HDR_CAP_ID_MASK GENMASK(15, 0)
/* CXL 2.0 8.2.8.2.1 CXL Device Capabilities */
#define CXLDEV_CAP_CAP_ID_DEVICE_STATUS 0x1
#define CXLDEV_CAP_CAP_ID_PRIMARY_MAILBOX 0x2
#define CXLDEV_CAP_CAP_ID_SECONDARY_MAILBOX 0x3
#define CXLDEV_CAP_CAP_ID_MEMDEV 0x4000

/* CXL 2.0 8.2.8.4 Mailbox Registers */
#define CXLDEV_MBOX_CAPS_OFFSET 0x00
#define   CXLDEV_MBOX_CAP_PAYLOAD_SIZE_MASK GENMASK(4, 0)
#define CXLDEV_MBOX_CTRL_OFFSET 0x04
#define   CXLDEV_MBOX_CTRL_DOORBELL BIT(0)
#define CXLDEV_MBOX_CMD_OFFSET 0x08
#define   CXLDEV_MBOX_CMD_COMMAND_OPCODE_MASK GENMASK_ULL(15, 0)
#define   CXLDEV_MBOX_CMD_PAYLOAD_LENGTH_MASK GENMASK_ULL(36, 16)
#define CXLDEV_MBOX_STATUS_OFFSET 0x10
#define   CXLDEV_MBOX_STATUS_RET_CODE_MASK GENMASK_ULL(47, 32)
#define CXLDEV_MBOX_BG_CMD_STATUS_OFFSET 0x18
#define CXLDEV_MBOX_PAYLOAD_OFFSET 0x20

<<<<<<< HEAD
/* CXL 2.0 8.2.8.5.1.1 Memory Device Status Register */
#define CXLMDEV_STATUS_OFFSET 0x0
#define   CXLMDEV_DEV_FATAL BIT(0)
#define   CXLMDEV_FW_HALT BIT(1)
#define   CXLMDEV_STATUS_MEDIA_STATUS_MASK GENMASK(3, 2)
#define     CXLMDEV_MS_NOT_READY 0
#define     CXLMDEV_MS_READY 1
#define     CXLMDEV_MS_ERROR 2
#define     CXLMDEV_MS_DISABLED 3
#define CXLMDEV_READY(status)                                                  \
	(FIELD_GET(CXLMDEV_STATUS_MEDIA_STATUS_MASK, status) ==                \
	 CXLMDEV_MS_READY)
#define   CXLMDEV_MBOX_IF_READY BIT(4)
#define   CXLMDEV_RESET_NEEDED_MASK GENMASK(7, 5)
#define     CXLMDEV_RESET_NEEDED_NOT 0
#define     CXLMDEV_RESET_NEEDED_COLD 1
#define     CXLMDEV_RESET_NEEDED_WARM 2
#define     CXLMDEV_RESET_NEEDED_HOT 3
#define     CXLMDEV_RESET_NEEDED_CXL 4
#define CXLMDEV_RESET_NEEDED(status)                                           \
	(FIELD_GET(CXLMDEV_RESET_NEEDED_MASK, status) !=                       \
	 CXLMDEV_RESET_NEEDED_NOT)

struct cxl_memdev;
=======
#define CXL_COMPONENT_REGS() \
	void __iomem *hdm_decoder

#define CXL_DEVICE_REGS() \
	void __iomem *status; \
	void __iomem *mbox; \
	void __iomem *memdev

/* See note for 'struct cxl_regs' for the rationale of this organization */
/*
 * CXL_COMPONENT_REGS - Common set of CXL Component register block base pointers
 * @hdm_decoder: CXL 2.0 8.2.5.12 CXL HDM Decoder Capability Structure
 */
struct cxl_component_regs {
	CXL_COMPONENT_REGS();
};

/* See note for 'struct cxl_regs' for the rationale of this organization */
/*
 * CXL_DEVICE_REGS - Common set of CXL Device register block base pointers
 * @status: CXL 2.0 8.2.8.3 Device Status Registers
 * @mbox: CXL 2.0 8.2.8.4 Mailbox Registers
 * @memdev: CXL 2.0 8.2.8.5 Memory Device Registers
 */
struct cxl_device_regs {
	CXL_DEVICE_REGS();
};

/*
 * Note, the anonymous union organization allows for per
 * register-block-type helper routines, without requiring block-type
 * agnostic code to include the prefix.
 */
struct cxl_regs {
	union {
		struct {
			CXL_COMPONENT_REGS();
		};
		struct cxl_component_regs component;
	};
	union {
		struct {
			CXL_DEVICE_REGS();
		};
		struct cxl_device_regs device_regs;
	};
};

struct cxl_reg_map {
	bool valid;
	unsigned long offset;
	unsigned long size;
};

struct cxl_component_reg_map {
	struct cxl_reg_map hdm_decoder;
};

struct cxl_device_reg_map {
	struct cxl_reg_map status;
	struct cxl_reg_map mbox;
	struct cxl_reg_map memdev;
};

struct cxl_register_map {
	struct list_head list;
	u64 block_offset;
	u8 reg_type;
	u8 barno;
	union {
		struct cxl_component_reg_map component_map;
		struct cxl_device_reg_map device_map;
	};
};

void cxl_probe_component_regs(struct device *dev, void __iomem *base,
			      struct cxl_component_reg_map *map);
void cxl_probe_device_regs(struct device *dev, void __iomem *base,
			   struct cxl_device_reg_map *map);
int cxl_map_component_regs(struct pci_dev *pdev,
			   struct cxl_component_regs *regs,
			   struct cxl_register_map *map);
int cxl_map_device_regs(struct pci_dev *pdev,
			struct cxl_device_regs *regs,
			struct cxl_register_map *map);

#define CXL_RESOURCE_NONE ((resource_size_t) -1)
#define CXL_TARGET_STRLEN 20

/*
 * cxl_decoder flags that define the type of memory / devices this
 * decoder supports as well as configuration lock status See "CXL 2.0
 * 8.2.5.12.7 CXL HDM Decoder 0 Control Register" for details.
 */
#define CXL_DECODER_F_RAM   BIT(0)
#define CXL_DECODER_F_PMEM  BIT(1)
#define CXL_DECODER_F_TYPE2 BIT(2)
#define CXL_DECODER_F_TYPE3 BIT(3)
#define CXL_DECODER_F_LOCK  BIT(4)
#define CXL_DECODER_F_MASK  GENMASK(4, 0)

enum cxl_decoder_type {
       CXL_DECODER_ACCELERATOR = 2,
       CXL_DECODER_EXPANDER = 3,
};

>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
/**
 * struct cxl_mem - A CXL memory device
 * @pdev: The PCI device associated with this CXL device.
 * @regs: IO mappings to the device's MMIO
 * @status_regs: CXL 2.0 8.2.8.3 Device Status Registers
 * @mbox_regs: CXL 2.0 8.2.8.4 Mailbox Registers
 * @memdev_regs: CXL 2.0 8.2.8.5 Memory Device Registers
 * @payload_size: Size of space for payload
 *                (CXL 2.0 8.2.8.4.3 Mailbox Capabilities Register)
 * @mbox_mutex: Mutex to synchronize mailbox access.
 * @firmware_version: Firmware version for the memory device.
 * @enabled_commands: Hardware commands found enabled in CEL.
 * @pmem_range: Persistent memory capacity information.
 * @ram_range: Volatile memory capacity information.
 */
struct cxl_mem {
	struct pci_dev *pdev;
	void __iomem *regs;
	struct cxl_memdev *cxlmd;

	void __iomem *status_regs;
	void __iomem *mbox_regs;
	void __iomem *memdev_regs;

	size_t payload_size;
	struct mutex mbox_mutex; /* Protects device mailbox and firmware */
	char firmware_version[0x10];
	unsigned long *enabled_cmds;

	struct range pmem_range;
	struct range ram_range;
};

extern struct bus_type cxl_bus_type;
#endif /* __CXL_H__ */
