/* bz-inhibited-scrollable.h
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

#define GX_TYPE_INHIBITED_SCROLLABLE (gx_inhibited_scrollable_get_type ())
G_DECLARE_FINAL_TYPE (GxInhibitedScrollable, gx_inhibited_scrollable, GX, INHIBITED_SCROLLABLE, GtkWidget)

GtkWidget *
gx_inhibited_scrollable_new (void);

void
gx_inhibited_scrollable_set_scrollable (GxInhibitedScrollable *self,
                                        GtkScrollable         *scrollable);

GtkScrollable *
gx_inhibited_scrollable_get_scrollable (GxInhibitedScrollable *self);

G_END_DECLS
