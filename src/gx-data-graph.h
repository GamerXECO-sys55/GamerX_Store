/* bz-data-graph.h
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

#define GX_TYPE_DATA_GRAPH (gx_data_graph_get_type ())
G_DECLARE_FINAL_TYPE (GxDataGraph, gx_data_graph, GX, DATA_GRAPH, GtkWidget)

GtkWidget *
gx_data_graph_new (void);

GListModel *
gx_data_graph_get_model (GxDataGraph *self);

const char *
gx_data_graph_get_independent_axis_label (GxDataGraph *self);

const char *
gx_data_graph_get_dependent_axis_label (GxDataGraph *self);

int
gx_data_graph_get_independent_decimals (GxDataGraph *self);

int
gx_data_graph_get_dependent_decimals (GxDataGraph *self);

gboolean
gx_data_graph_get_has_dependent_min (GxDataGraph *self);

double
gx_data_graph_get_dependent_min (GxDataGraph *self);

gboolean
gx_data_graph_get_has_dependent_max (GxDataGraph *self);

double
gx_data_graph_get_dependent_max (GxDataGraph *self);

double
gx_data_graph_get_transition_progress (GxDataGraph *self);

void
gx_data_graph_set_model (GxDataGraph *self,
                         GListModel  *model);

void
gx_data_graph_set_independent_axis_label (GxDataGraph *self,
                                          const char  *independent_axis_label);

void
gx_data_graph_set_dependent_axis_label (GxDataGraph *self,
                                        const char  *dependent_axis_label);

void
gx_data_graph_set_independent_decimals (GxDataGraph *self,
                                        int          independent_decimals);

void
gx_data_graph_set_dependent_decimals (GxDataGraph *self,
                                      int          dependent_decimals);

void
gx_data_graph_set_has_dependent_min (GxDataGraph *self,
                                     gboolean     dependent_min);

void
gx_data_graph_set_dependent_min (GxDataGraph *self,
                                 double       dependent_min);

void
gx_data_graph_set_has_dependent_max (GxDataGraph *self,
                                     gboolean     dependent_max);

void
gx_data_graph_set_dependent_max (GxDataGraph *self,
                                 double       dependent_max);

void
gx_data_graph_set_transition_progress (GxDataGraph *self,
                                       double       transition_progress);

void
gx_data_graph_animate_open (GxDataGraph *self);

G_END_DECLS
