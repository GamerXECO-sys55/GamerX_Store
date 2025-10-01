/* bz-app-tile.h
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

#define GX_TYPE_APP_TILE (gx_app_tile_get_type ())
G_DECLARE_FINAL_TYPE (GxAppTile, gx_app_tile, GX, APP_TILE, GtkButton)

GtkWidget *
gx_app_tile_new (void);

GxEntryGroup *
gx_app_tile_get_group (GxAppTile *self);

void
gx_app_tile_set_group (GxAppTile    *self,
                       GxEntryGroup *group);

G_END_DECLS

/* End of bz-app-tile.h */
