/* bz-comet.h
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

#define GX_TYPE_COMET (gx_comet_get_type ())
G_DECLARE_FINAL_TYPE (GxComet, gx_comet, GX, COMET, GObject)

const char *
gx_comet_get_name (GxComet *self);

GtkWidget *
gx_comet_get_from (GxComet *self);

GtkWidget *
gx_comet_get_to (GxComet *self);

GdkPaintable *
gx_comet_get_paintable (GxComet *self);

GskPath *
gx_comet_get_path (GxComet *self);

double
gx_comet_get_path_length (GxComet *self);

double
gx_comet_get_progress (GxComet *self);

void
gx_comet_set_name (GxComet    *self,
                   const char *name);

void
gx_comet_set_from (GxComet   *self,
                   GtkWidget *from);

void
gx_comet_set_to (GxComet   *self,
                 GtkWidget *to);

void
gx_comet_set_paintable (GxComet      *self,
                        GdkPaintable *paintable);

void
gx_comet_set_path (GxComet *self,
                   GskPath *path);

void
gx_comet_set_path_length (GxComet *self,
                          double   path_length);

void
gx_comet_set_progress (GxComet *self,
                       double   progress);

G_END_DECLS

/* End of bz-comet.h */
