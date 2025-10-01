/* bz-data-point.h
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

#define GX_TYPE_DATA_POINT (gx_data_point_get_type ())
G_DECLARE_FINAL_TYPE (GxDataPoint, gx_data_point, GX, DATA_POINT, GObject)

GxDataPoint *
gx_data_point_new (void);

double
gx_data_point_get_independent (GxDataPoint *self);

double
gx_data_point_get_dependent (GxDataPoint *self);

const char *
gx_data_point_get_label (GxDataPoint *self);

void
gx_data_point_set_independent (GxDataPoint *self,
    double independent);

void
gx_data_point_set_dependent (GxDataPoint *self,
    double dependent);

void
gx_data_point_set_label (GxDataPoint *self,
    const char *label);

G_END_DECLS

/* End of bz-data-point.h */
