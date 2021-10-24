# SPDX-License-Identifier: GPL-2.0
# Makefile for KVM support for MIPS
#

<<<<<<< HEAD
common-objs-y = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o eventfd.o)
=======
common-objs-y = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o eventfd.o binary_stats.o)
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

EXTRA_CFLAGS += -Ivirt/kvm -Iarch/mips/kvm

common-objs-$(CONFIG_CPU_HAS_MSA) += msa.o

kvm-objs := $(common-objs-y) mips.o emulate.o entry.o \
<<<<<<< HEAD
	    interrupt.o stats.o commpage.o \
=======
	    interrupt.o stats.o \
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
	    fpu.o
kvm-objs += hypcall.o
kvm-objs += mmu.o
ifdef CONFIG_CPU_LOONGSON64
kvm-objs += loongson_ipi.o
endif

<<<<<<< HEAD
ifdef CONFIG_KVM_MIPS_VZ
kvm-objs		+= vz.o
else
kvm-objs		+= dyntrans.o
kvm-objs		+= trap_emul.o
endif
=======
kvm-objs		+= vz.o
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
obj-$(CONFIG_KVM)	+= kvm.o
obj-y			+= callback.o tlb.o
