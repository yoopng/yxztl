/* libztl: User-space Zone Translation Layer Library
 *
 * Copyright 2019 Samsung Electronics
 *
 * Written by Ivan L. Picoli <i.picoli@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef LIBZROCKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define ZNS_ALIGMENT 4096

struct zrocks_map {
    union {
	struct {

	    /* Media offset */
	    /* 4KB  sector: Max capacity: 4PB
             * 512b sector: Max capacity: 512TB */
            uint64_t offset : 40;

	    /* Number of sectors */
            /* 4KB  sector: Max entry size: 32GB
             * 512b sector: Max entry size: 4GB */
	    uint64_t nsec   : 23;

	    /* Multi-piece mapping bit */
	    uint64_t multi  : 1;
	} g;

	uint64_t addr;
    };
};

/**
 * Initialize zrocks library
 */
int zrocks_init (const char *dev_name);

/**
 * Close zrocks library
 */
int zrocks_exit (void);

/**
 * Allocate an aligned buffer for DMA
 */
void *zrocks_alloc (size_t size);

/**
 * Free a buffer allocated by zrocks_alloc
 */
void zrocks_free (void *ptr);

/**
 * Create a new variable-sized object belonging to a certain level
 */
int zrocks_new (uint64_t id, void *buf, uint32_t size, uint16_t level);

/**
 * Delete an object
 */
int zrocks_delete (uint64_t id);

/**
 * Read an offset within an object */
int zrocks_read_obj (uint64_t id, uint64_t offset, void *buf, uint32_t size);

/**
 * Write to ZNS device and return the mapping multi-piece list
 * This function is used if the application is responsible for recovery.
 *
 * NOTE: If the return value is zero, the user is responsible for
 * calling 'zrocks_free' and free 'map' by passing its value as
 * parameter.
 */
int zrocks_write (void *buf, uint32_t size, uint16_t level,
				struct zrocks_map **map, uint16_t *pieces);

/**
 * Read from ZNS device from a physical offset
 * This fuinction is used if the application is responsible for recovery
 */
int zrocks_read (uint64_t offset, void *buf, uint64_t size);

#ifdef __cplusplus
}; // closing brace for extern "C"
#endif

#endif // LIBZROCKS_H
