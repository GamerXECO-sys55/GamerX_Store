/* bz-group-tile-css-watcher.h
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

#include "gx-entry-group.h"

G_BEGIN_DECLS

#define GX_TYPE_GROUP_TILE_CSS_WATCHER (gx_group_tile_css_watcher_get_type ())
G_DECLARE_FINAL_TYPE (GxGroupTileCssWatcher, gx_group_tile_css_watcher, GX, GROUP_TILE_CSS_WATCHER, GObject)

GxGroupTileCssWatcher *
gx_group_tile_css_watcher_new (void);

GtkWidget *
gx_group_tile_css_watcher_dup_widget (GxGroupTileCssWatcher *self);

GxEntryGroup *
gx_group_tile_css_watcher_get_group (GxGroupTileCssWatcher *self);

void
gx_group_tile_css_watcher_set_widget (GxGroupTileCssWatcher *self,
                                      GtkWidget             *widget);

void
gx_group_tile_css_watcher_set_group (GxGroupTileCssWatcher *self,
                                     GxEntryGroup          *group);

G_END_DECLS

/* End of bz-group-tile-css-watcher.h */
