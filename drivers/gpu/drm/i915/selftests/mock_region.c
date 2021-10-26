// SPDX-License-Identifier: MIT
/*
 * Copyright © 2019 Intel Corporation
 */

<<<<<<< HEAD
=======
#include <linux/scatterlist.h>

#include <drm/ttm/ttm_placement.h>

>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
#include "gem/i915_gem_region.h"
#include "intel_memory_region.h"

#include "mock_region.h"

<<<<<<< HEAD
=======
static void mock_region_put_pages(struct drm_i915_gem_object *obj,
				  struct sg_table *pages)
{
	intel_region_ttm_node_free(obj->mm.region, obj->mm.st_mm_node);
	sg_free_table(pages);
	kfree(pages);
}

static int mock_region_get_pages(struct drm_i915_gem_object *obj)
{
	unsigned int flags;
	struct sg_table *pages;

	flags = I915_ALLOC_MIN_PAGE_SIZE;
	if (obj->flags & I915_BO_ALLOC_CONTIGUOUS)
		flags |= I915_ALLOC_CONTIGUOUS;

	obj->mm.st_mm_node = intel_region_ttm_node_alloc(obj->mm.region,
							 obj->base.size,
							 flags);
	if (IS_ERR(obj->mm.st_mm_node))
		return PTR_ERR(obj->mm.st_mm_node);

	pages = intel_region_ttm_node_to_st(obj->mm.region, obj->mm.st_mm_node);
	if (IS_ERR(pages)) {
		intel_region_ttm_node_free(obj->mm.region, obj->mm.st_mm_node);
		return PTR_ERR(pages);
	}

	__i915_gem_object_set_pages(obj, pages, i915_sg_dma_sizes(pages->sgl));

	return 0;
}

>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
static const struct drm_i915_gem_object_ops mock_region_obj_ops = {
	.name = "mock-region",
	.get_pages = i915_gem_object_get_pages_buddy,
	.put_pages = i915_gem_object_put_pages_buddy,
	.release = i915_gem_object_release_memory_region,
};

static int mock_object_init(struct intel_memory_region *mem,
			    struct drm_i915_gem_object *obj,
			    resource_size_t size,
			    unsigned int flags)
{
	static struct lock_class_key lock_class;
	struct drm_i915_private *i915 = mem->i915;

	if (size > mem->mm.size)
		return -E2BIG;

	drm_gem_private_object_init(&i915->drm, &obj->base, size);
	i915_gem_object_init(obj, &mock_region_obj_ops, &lock_class);

	obj->read_domains = I915_GEM_DOMAIN_CPU | I915_GEM_DOMAIN_GTT;

	i915_gem_object_set_cache_coherency(obj, I915_CACHE_NONE);

	i915_gem_object_init_memory_region(obj, mem, flags);

	return 0;
}

static const struct intel_memory_region_ops mock_region_ops = {
	.init = intel_memory_region_init_buddy,
	.release = intel_memory_region_release_buddy,
	.init_object = mock_object_init,
};

struct intel_memory_region *
mock_region_create(struct drm_i915_private *i915,
		   resource_size_t start,
		   resource_size_t size,
		   resource_size_t min_page_size,
		   resource_size_t io_start)
{
	return intel_memory_region_create(i915, start, size, min_page_size,
					  io_start, &mock_region_ops);
}
