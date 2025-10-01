/* bz-entry-cache-manager.h
 *
 * Copyright 2025 Adam Masciola
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <libdex.h>

#include "gx-entry.h"

G_BEGIN_DECLS

#define GX_ENTRY_CACHE_ERROR (gx_entry_cache_error_quark ())
GQuark gx_entry_cache_error_quark (void);

typedef enum
{
  GX_ENTRY_CACHE_ERROR_CACHE_FAILED = 0,
  GX_ENTRY_CACHE_ERROR_DECACHE_FAILED,
} GxEntry_CacheError;

#define GX_TYPE_ENTRY_CACHE_MANAGER (gx_entry_cache_manager_get_type ())
G_DECLARE_FINAL_TYPE (GxEntryCacheManager, gx_entry_cache_manager, GX, ENTRY_CACHE_MANAGER, GObject)

GxEntryCacheManager *
gx_entry_cache_manager_new (void);

guint64
gx_entry_cache_manager_get_max_memory_usage (GxEntryCacheManager *self);

void
gx_entry_cache_manager_set_max_memory_usage (GxEntryCacheManager *self,
                                             guint64              max_memory_usage);

DexFuture *
gx_entry_cache_manager_add (GxEntryCacheManager *self,
                            GxEntry             *entry);

DexFuture *
gx_entry_cache_manager_get (GxEntryCacheManager *self,
                            const char          *unique_id);

G_END_DECLS

/* End of bz-entry-cache-manager.h */
