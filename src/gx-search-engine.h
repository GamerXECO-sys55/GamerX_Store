/* bz-search-engine.h
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

#define GX_TYPE_SEARCH_ENGINE (gx_search_engine_get_type ())
G_DECLARE_FINAL_TYPE (GxSearchEngine, gx_search_engine, GX, SEARCH_ENGINE, GObject)

GxSearchEngine *
gx_search_engine_new (void);

GListModel *
gx_search_engine_get_model (GxSearchEngine *self);

void
gx_search_engine_set_model (GxSearchEngine *self,
                            GListModel     *model);

DexFuture *
gx_search_engine_query (GxSearchEngine    *self,
                        const char *const *terms);

G_END_DECLS

/* End of bz-search-engine.h */
