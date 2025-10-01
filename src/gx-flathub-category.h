/* bz-flathub-category.h
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

#include "gx-application-map-factory.h"

G_BEGIN_DECLS

#define GX_TYPE_FLATHUB_CATEGORY (gx_flathub_category_get_type ())
G_DECLARE_FINAL_TYPE (GxFlathubCategory, gx_flathub_category, GX, FLATHUB_CATEGORY, GObject)

GxFlathubCategory *
gx_flathub_category_new (void);

GxApplicationMapFactory *
gx_flathub_category_get_map_factory (GxFlathubCategory *self);

const char *
gx_flathub_category_get_name (GxFlathubCategory *self);

GListModel *
gx_flathub_category_dup_applications (GxFlathubCategory *self);

void
gx_flathub_category_set_map_factory (GxFlathubCategory       *self,
                                     GxApplicationMapFactory *map_factory);

void
gx_flathub_category_set_name (GxFlathubCategory *self,
                              const char        *name);

void
gx_flathub_category_set_applications (GxFlathubCategory *self,
                                      GListModel        *applications);

const char *
gx_flathub_category_get_display_name (GxFlathubCategory *self);

const char *
gx_flathub_category_get_icon_name (GxFlathubCategory *self);

G_END_DECLS

/* End of bz-flathub-category.h */
