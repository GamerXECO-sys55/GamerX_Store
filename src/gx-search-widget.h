/* bz-search-widget.h
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

#include <adwaita.h>

#include "gx-entry-group.h"

G_BEGIN_DECLS

#define GX_TYPE_SEARCH_WIDGET (gx_search_widget_get_type ())
G_DECLARE_FINAL_TYPE (GxSearchWidget, gx_search_widget, GX, SEARCH_WIDGET, AdwBin)

GtkWidget *
gx_search_widget_new (GListModel *model,
                      const char *initial);

void
gx_search_widget_set_model (GxSearchWidget *self,
                            GListModel     *model);

GListModel *
gx_search_widget_get_model (GxSearchWidget *self);

void
gx_search_widget_set_text (GxSearchWidget *self,
                           const char     *text);

const char *
gx_search_widget_get_text (GxSearchWidget *self);

GxEntryGroup *
gx_search_widget_get_selected (GxSearchWidget *self,
                               gboolean       *remove);

GxEntryGroup *
gx_search_widget_get_previewing (GxSearchWidget *self);

void
gx_search_widget_refresh (GxSearchWidget *self);

G_END_DECLS
