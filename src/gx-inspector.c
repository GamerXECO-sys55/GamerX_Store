/* bz-inspector.c
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

#include "gx-inspector.h"
#include "gx-entry-inspector.h"

struct _GxInspector
{
  AdwWindow parent_instance;

  GxStateInfo *state;

  GtkEditable        *search_entry;
  GtkFilterListModel *filter_model;
};

G_DEFINE_FINAL_TYPE (GxInspector, gx_inspector, ADW_TYPE_WINDOW);

enum
{
  PROP_0,

  PROP_STATE,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static gboolean
filter_func (GxEntryGroup *group,
             GxInspector  *self);

static void
gx_inspector_dispose (GObject *object)
{
  GxInspector *self = GX_INSPECTOR (object);

  g_clear_pointer (&self->state, g_object_unref);

  G_OBJECT_CLASS (gx_inspector_parent_class)->dispose (object);
}

static void
gx_inspector_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  GxInspector *self = GX_INSPECTOR (object);

  switch (prop_id)
    {
    case PROP_STATE:
      g_value_set_object (value, gx_inspector_get_state (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_inspector_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  GxInspector *self = GX_INSPECTOR (object);

  switch (prop_id)
    {
    case PROP_STATE:
      gx_inspector_set_state (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
entry_changed (GxInspector *self,
               GtkEditable *editable)
{
  GtkFilter *filter = NULL;

  filter = gtk_filter_list_model_get_filter (self->filter_model);
  gtk_filter_changed (filter, GTK_FILTER_CHANGE_DIFFERENT);
}

static void
decache_and_inspect_cb (GtkListItem *list_item,
                        GtkButton   *button)
{
  GtkStringObject *item       = NULL;
  GxInspector     *self       = NULL;
  g_autoptr (GxResult) result = NULL;

  item = gtk_list_item_get_item (list_item);

  self = GX_INSPECTOR (gtk_widget_get_ancestor (GTK_WIDGET (button), GX_TYPE_INSPECTOR));
  g_assert (self != NULL);

  result = gx_application_map_factory_convert_one (
      gx_state_info_get_entry_factory (self->state),
      g_object_ref (item));
  if (result != NULL)
    {
      GxEntryInspector *inspector = NULL;

      inspector = gx_entry_inspector_new ();
      gx_entry_inspector_set_result (inspector, result);

      gtk_window_present (GTK_WINDOW (inspector));
    }
}

static void
gx_inspector_class_init (GxInspectorClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = gx_inspector_set_property;
  object_class->get_property = gx_inspector_get_property;
  object_class->dispose      = gx_inspector_dispose;

  props[PROP_STATE] =
      g_param_spec_object (
          "state",
          NULL, NULL,
          GX_TYPE_STATE_INFO,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/bz-inspector.ui");
  gtk_widget_class_bind_template_child (widget_class, GxInspector, search_entry);
  gtk_widget_class_bind_template_child (widget_class, GxInspector, filter_model);
  gtk_widget_class_bind_template_callback (widget_class, decache_and_inspect_cb);
  gtk_widget_class_bind_template_callback (widget_class, entry_changed);
}

static void
gx_inspector_init (GxInspector *self)
{
  GtkCustomFilter *filter = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));

  filter = gtk_custom_filter_new ((GtkCustomFilterFunc) filter_func, self, NULL);
  gtk_filter_list_model_set_filter (self->filter_model, GTK_FILTER (filter));
}

GxInspector *
gx_inspector_new (void)
{
  return g_object_new (GX_TYPE_INSPECTOR, NULL);
}

GxStateInfo *
gx_inspector_get_state (GxInspector *self)
{
  g_return_val_if_fail (GX_IS_INSPECTOR (self), NULL);
  return self->state;
}

void
gx_inspector_set_state (GxInspector *self,
                        GxStateInfo *state)
{
  g_return_if_fail (GX_IS_INSPECTOR (self));

  g_clear_pointer (&self->state, g_object_unref);
  if (state != NULL)
    self->state = g_object_ref (state);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_STATE]);
}

static gboolean
filter_func (GxEntryGroup *group,
             GxInspector  *self)
{
  const char *text  = NULL;
  const char *id    = NULL;
  const char *title = NULL;

  text  = gtk_editable_get_text (self->search_entry);
  id    = gx_entry_group_get_id (group);
  title = gx_entry_group_get_title (group);

  if (text == NULL || *text == '\0')
    return TRUE;

  if (strcasestr (id, text) != NULL)
    return TRUE;
  if (strcasestr (title, text) != NULL)
    return TRUE;

  return FALSE;
}

/* End of bz-inspector.c */
