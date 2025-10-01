/* bz-comet-overlay.h
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

#include "gx-comet.h"

G_BEGIN_DECLS

#define GX_TYPE_COMET_OVERLAY (gx_comet_overlay_get_type ())
G_DECLARE_FINAL_TYPE (GxCometOverlay, gx_comet_overlay, GX, COMET_OVERLAY, GtkWidget)

GtkWidget *
gx_comet_overlay_new (void);

void
gx_comet_overlay_set_child (GxCometOverlay *self,
                            GtkWidget      *child);

GtkWidget *
gx_comet_overlay_get_child (GxCometOverlay *self);

void
gx_comet_overlay_spawn (GxCometOverlay *self,
                        GxComet        *comet);

void
gx_comet_overlay_pulse_child (GxCometOverlay *self,
                              GtkWidget      *child);

G_END_DECLS
