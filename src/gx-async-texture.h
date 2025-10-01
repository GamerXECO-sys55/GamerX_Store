/* bz-async-texture.h
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

#include <gtk/gtk.h>
#include <libdex.h>

G_BEGIN_DECLS

#define GX_TYPE_ASYNC_TEXTURE (gx_async_texture_get_type ())
G_DECLARE_FINAL_TYPE (GxAsyncTexture, gx_async_texture, GX, ASYNC_TEXTURE, GObject)

GxAsyncTexture *
gx_async_texture_new (GFile *source,
                      GFile *cache_into);

GxAsyncTexture *
gx_async_texture_new_lazy (GFile *source,
                           GFile *cache_into);

GFile *
gx_async_texture_get_source (GxAsyncTexture *self);

const char *
gx_async_texture_get_source_uri (GxAsyncTexture *self);

GFile *
gx_async_texture_get_cache_into (GxAsyncTexture *self);

const char *
gx_async_texture_get_cache_into_path (GxAsyncTexture *self);

gboolean
gx_async_texture_get_loaded (GxAsyncTexture *self);

GdkTexture *
gx_async_texture_dup_texture (GxAsyncTexture *self);

DexFuture *
gx_async_texture_dup_future (GxAsyncTexture *self);

void
gx_async_texture_ensure (GxAsyncTexture *self);

void
gx_async_texture_cancel (GxAsyncTexture *self);

gboolean
gx_async_texture_is_loading (GxAsyncTexture *self);

G_END_DECLS
