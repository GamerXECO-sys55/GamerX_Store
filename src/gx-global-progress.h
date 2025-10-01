/* bz-global-progress.h
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

#define GX_TYPE_GLOBAL_PROGRESS (gx_global_progress_get_type ())
G_DECLARE_FINAL_TYPE (GxGlobalProgress, gx_global_progress, GX, GLOBAL_PROGRESS, GtkWidget)

GtkWidget *
gx_global_progress_new (void);

GtkWidget *
gx_global_progress_get_child (GxGlobalProgress *self);

void
gx_global_progress_set_child (GxGlobalProgress *self,
                              GtkWidget        *child);

void
gx_global_progress_set_active (GxGlobalProgress *self,
                               gboolean          active);

gboolean
gx_global_progress_get_active (GxGlobalProgress *self);

void
gx_global_progress_set_fraction (GxGlobalProgress *self,
                                 double            fraction);

double
gx_global_progress_get_fraction (GxGlobalProgress *self);

void
gx_global_progress_set_actual_fraction (GxGlobalProgress *self,
                                        double            fraction);

double
gx_global_progress_get_actual_fraction (GxGlobalProgress *self);

void
gx_global_progress_set_transition_progress (GxGlobalProgress *self,
                                            double            progress);

double
gx_global_progress_get_transition_progress (GxGlobalProgress *self);

void
gx_global_progress_set_expand_size (GxGlobalProgress *self,
                                    int               expand_size);

int
gx_global_progress_get_expand_size (GxGlobalProgress *self);

G_END_DECLS
