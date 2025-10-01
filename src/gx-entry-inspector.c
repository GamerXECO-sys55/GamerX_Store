/* bz-entry-inspector.c
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

#include <json-glib/json-glib.h>

#include "gx-entry-inspector.h"
#include "gx-entry.h"
#include "gx-serializable.h"

struct _GxEntryInspector
{
  AdwWindow parent_instance;

  GxResult *result;

  /* Template widgets */
  GtkTextBuffer  *text_buffer;
  GtkCheckButton *convert_to_json;
};

G_DEFINE_FINAL_TYPE (GxEntryInspector, gx_entry_inspector, ADW_TYPE_WINDOW);

enum
{
  PROP_0,

  PROP_RESULT,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_entry_inspector_dispose (GObject *object)
{
  GxEntryInspector *self = GX_ENTRY_INSPECTOR (object);

  g_clear_pointer (&self->result, g_object_unref);

  G_OBJECT_CLASS (gx_entry_inspector_parent_class)->dispose (object);
}

static void
gx_entry_inspector_get_property (GObject    *object,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  GxEntryInspector *self = GX_ENTRY_INSPECTOR (object);

  switch (prop_id)
    {
    case PROP_RESULT:
      g_value_set_object (value, gx_entry_inspector_get_result (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_entry_inspector_set_property (GObject      *object,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  GxEntryInspector *self = GX_ENTRY_INSPECTOR (object);

  switch (prop_id)
    {
    case PROP_RESULT:
      gx_entry_inspector_set_result (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gen_serialized (GxEntryInspector *self,
                GtkButton        *button)
{
  if (gx_result_get_resolved (self->result))
    {
      GxEntry *entry                      = NULL;
      g_autoptr (GVariantBuilder) builder = NULL;
      g_autoptr (GVariant) variant        = NULL;
      g_autofree char *string             = NULL;

      entry   = gx_result_get_object (self->result);
      builder = g_variant_builder_new (G_VARIANT_TYPE_VARDICT);
      gx_serializable_serialize (GX_SERIALIZABLE (entry), builder);
      variant = g_variant_builder_end (builder);

      if (gtk_check_button_get_active (self->convert_to_json))
        {
          g_autoptr (JsonNode) node           = NULL;
          g_autoptr (JsonGenerator) generator = NULL;

          node = json_gvariant_serialize (variant);

          generator = json_generator_new ();
          json_generator_set_pretty (generator, TRUE);
          json_generator_set_root (generator, node);

          string = json_generator_to_data (generator, NULL);
        }
      else
        string = g_variant_print (variant, FALSE);

      gtk_text_buffer_set_text (self->text_buffer, string, -1);
    }
  else
    gtk_text_buffer_set_text (self->text_buffer, "!!! The entry has not resolved", -1);
}

static void
gx_entry_inspector_class_init (GxEntryInspectorClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = gx_entry_inspector_set_property;
  object_class->get_property = gx_entry_inspector_get_property;
  object_class->dispose      = gx_entry_inspector_dispose;

  props[PROP_RESULT] =
      g_param_spec_object (
          "result",
          NULL, NULL,
          GX_TYPE_RESULT,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/gx-entry-inspector.ui");
  gtk_widget_class_bind_template_child (widget_class, GxEntryInspector, text_buffer);
  gtk_widget_class_bind_template_child (widget_class, GxEntryInspector, convert_to_json);
  gtk_widget_class_bind_template_callback (widget_class, gen_serialized);
}

static void
gx_entry_inspector_init (GxEntryInspector *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

GxEntryInspector *
gx_entry_inspector_new (void)
{
  return g_object_new (GX_TYPE_ENTRY_INSPECTOR, NULL);
}

GxResult *
gx_entry_inspector_get_result (GxEntryInspector *self)
{
  g_return_val_if_fail (GX_IS_ENTRY_INSPECTOR (self), NULL);
  return self->result;
}

void
gx_entry_inspector_set_result (GxEntryInspector *self,
                               GxResult         *result)
{
  g_return_if_fail (GX_IS_ENTRY_INSPECTOR (self));

  g_clear_pointer (&self->result, g_object_unref);
  if (result != NULL)
    self->result = g_object_ref (result);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_RESULT]);
}

/* End of bz-entry-inspector.c */
