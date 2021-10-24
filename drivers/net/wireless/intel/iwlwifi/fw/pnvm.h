/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/******************************************************************************
 *
 * Copyright(c) 2020 Intel Corporation
 *
 *****************************************************************************/

#ifndef __IWL_PNVM_H__
#define __IWL_PNVM_H__

#include "fw/notif-wait.h"

<<<<<<< HEAD
#define MVM_UCODE_PNVM_TIMEOUT	(HZ / 10)
=======
#define MVM_UCODE_PNVM_TIMEOUT	(HZ / 4)
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

int iwl_pnvm_load(struct iwl_trans *trans,
		  struct iwl_notif_wait_data *notif_wait);

#endif /* __IWL_PNVM_H__ */
