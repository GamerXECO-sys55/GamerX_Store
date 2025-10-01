/* bz-progress-bar.h
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

G_BEGIN_DECLS

#define GX_TYPE_PROGRESS_BAR (gx_progress_bar_get_type ())
G_DECLARE_FINAL_TYPE (GxProgressBar, gx_progress_bar, GX, PROGRESS_BAR, AdwBin)

GtkWidget *
gx_progress_bar_new (void);

void
gx_progress_bar_set_fraction (GxProgressBar *self,
                              double         fraction);

double
gx_progress_bar_get_fraction (GxProgressBar *self);

G_END_DECLS
