/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2019 Intel Corporation
 */

#ifndef _I915_GLOBALS_H_
#define _I915_GLOBALS_H_

#include <linux/types.h>

typedef void (*i915_global_func_t)(void);

struct i915_global {
	struct list_head link;

	i915_global_func_t shrink;
	i915_global_func_t exit;
};

void i915_global_register(struct i915_global *global);

int i915_globals_init(void);
void i915_globals_park(void);
void i915_globals_unpark(void);
void i915_globals_exit(void);

/* constructors */
int i915_global_active_init(void);
<<<<<<< HEAD
int i915_global_buddy_init(void);
=======
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
int i915_global_context_init(void);
int i915_global_gem_context_init(void);
int i915_global_objects_init(void);
int i915_global_request_init(void);
int i915_global_scheduler_init(void);
int i915_global_vma_init(void);

#endif /* _I915_GLOBALS_H_ */
