# SPDX-License-Identifier: GPL-2.0
#
# Makefile for Marvell's OcteonTX2 ethernet device drivers
#

obj-$(CONFIG_OCTEONTX2_PF) += rvu_nicpf.o
obj-$(CONFIG_OCTEONTX2_VF) += rvu_nicvf.o

rvu_nicpf-y := otx2_pf.o otx2_common.o otx2_txrx.o otx2_ethtool.o \
<<<<<<< HEAD
		     otx2_ptp.o otx2_flows.o cn10k.o
=======
               otx2_ptp.o otx2_flows.o otx2_tc.o cn10k.o otx2_dmac_flt.o
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
rvu_nicvf-y := otx2_vf.o

ccflags-y += -I$(srctree)/drivers/net/ethernet/marvell/octeontx2/af
