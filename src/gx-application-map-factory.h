/* bz-application-map-factory.h
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

G_BEGIN_DECLS

#define GX_TYPE_APPLICATION_MAP_FACTORY (gx_application_map_factory_get_type ())
G_DECLARE_FINAL_TYPE (GxApplicationMapFactory, gx_application_map_factory, GX, APPLICATION_MAP_FACTORY, GObject)

GxApplicationMapFactory *
gx_application_map_factory_new (GtkMapListModelMapFunc func,
                                gpointer               user_data,
                                GDestroyNotify         ref_user_data,
                                GDestroyNotify         unref_user_data,
                                GtkFilter             *filter);

GListModel *
gx_application_map_factory_generate (GxApplicationMapFactory *self,
                                     GListModel              *model);

gpointer
gx_application_map_factory_convert_one (GxApplicationMapFactory *self,
                                        gpointer                 item);

G_END_DECLS

/* End of bz-application-map-factory.h */
