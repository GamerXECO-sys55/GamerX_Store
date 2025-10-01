/* bz-search-result.h
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

#include "gx-entry-group.h"

G_BEGIN_DECLS

#define GX_TYPE_SEARCH_RESULT (gx_search_result_get_type ())
G_DECLARE_FINAL_TYPE (GxSearchResult, gx_search_result, GX, SEARCH_RESULT, GObject)

GxSearchResult *
gx_search_result_new (void);

GxEntryGroup *
gx_search_result_get_group (GxSearchResult *self);

guint
gx_search_result_get_original_index (GxSearchResult *self);

double
gx_search_result_get_score (GxSearchResult *self);

const char *
gx_search_result_get_title_markup (GxSearchResult *self);

void
gx_search_result_set_group (GxSearchResult *self,
                            GxEntryGroup   *group);

void
gx_search_result_set_original_index (GxSearchResult *self,
                                     guint           original_index);

void
gx_search_result_set_score (GxSearchResult *self,
                            double          score);

void
gx_search_result_set_title_markup (GxSearchResult *self,
                                   const char     *title_markup);

G_END_DECLS

/* End of bz-search-result.h */
