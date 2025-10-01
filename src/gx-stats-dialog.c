/* bz-stats-dialog.c
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

#include "config.h"

#include "gx-data-graph.h"
#include "gx-stats-dialog.h"

struct _GxStatsDialog
{
  AdwDialog parent_instance;

  GListModel *model;

  /* Template widgets */
  GxDataGraph *graph;
};

G_DEFINE_FINAL_TYPE (GxStatsDialog, gx_stats_dialog, ADW_TYPE_DIALOG)

enum
{
  PROP_0,

  PROP_MODEL,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_stats_dialog_dispose (GObject *object)
{
  GxStatsDialog *self = GX_STATS_DIALOG (object);

  g_clear_object (&self->model);

  G_OBJECT_CLASS (gx_stats_dialog_parent_class)->dispose (object);
}

static void
gx_stats_dialog_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GxStatsDialog *self = GX_STATS_DIALOG (object);

  switch (prop_id)
    {
    case PROP_MODEL:
      g_value_set_object (value, self->model);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_stats_dialog_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GxStatsDialog *self = GX_STATS_DIALOG (object);

  switch (prop_id)
    {
    case PROP_MODEL:
      g_clear_object (&self->model);
      self->model = g_value_dup_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_stats_dialog_class_init (GxStatsDialogClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose      = gx_stats_dialog_dispose;
  object_class->get_property = gx_stats_dialog_get_property;
  object_class->set_property = gx_stats_dialog_set_property;

  props[PROP_MODEL] =
      g_param_spec_object (
          "model",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  g_type_ensure (GX_TYPE_DATA_GRAPH);

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/bz-stats-dialog.ui");
  gtk_widget_class_bind_template_child (widget_class, GxStatsDialog, graph);
}

static void
gx_stats_dialog_init (GxStatsDialog *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

AdwDialog *
gx_stats_dialog_new (GListModel *model)
{
  GxStatsDialog *stats_dialog = NULL;

  stats_dialog = g_object_new (
      GX_TYPE_STATS_DIALOG,
      "model", model,
      NULL);

  return ADW_DIALOG (stats_dialog);
}

void
gx_stats_dialog_animate_open (GxStatsDialog *self)
{
  g_return_if_fail (GX_IS_STATS_DIALOG (self));
  gx_data_graph_animate_open (self->graph);
}
