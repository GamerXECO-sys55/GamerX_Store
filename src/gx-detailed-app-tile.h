/* bz-detailed-app-tile.h
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

#define GX_TYPE_DETAILED_APP_TILE (gx_detailed_app_tile_get_type ())
G_DECLARE_FINAL_TYPE (GxDetailedAppTile, gx_detailed_app_tile, GX, DETAILED_APP_TILE, GtkButton)

GxDetailedAppTile *
gx_detailed_app_tile_new (void);

GxEntryGroup *
gx_detailed_app_tile_get_group (GxDetailedAppTile *self);

void
gx_detailed_app_tile_set_group (GxDetailedAppTile *self,
                                GxEntryGroup      *group);

G_END_DECLS

/* End of bz-detailed-app-tile.h */
