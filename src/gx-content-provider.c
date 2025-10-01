/* bz-content-provider.c
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

#define G_LOG_DOMAIN  "GAMERX_STORE::CURATED-PROVIDER"
#define GAMERX_MODULE "curated-provider"

#include "config.h"

#include <libdex.h>
#include <yaml.h>

#include "gx-async-texture.h"
#include "gx-content-provider.h"
#include "gx-content-section.h"
#include "gx-env.h"
#include "gx-io.h"
#include "gx-util.h"
#include "gx-yaml-parser.h"

/* clang-format off */
G_DEFINE_QUARK (gx-content-yaml-error-quark, gx_content_yaml_error);
/* clang-format on */

struct _GxContentProvider
{
  GObject parent_instance;

  GxYamlParser *yaml_parser;

  GListModel              *input_files;
  GxApplicationMapFactory *factory;

  GListStore *input_mirror;
  GHashTable *input_tracking;

  GListStore          *outputs;
  GtkFlattenListModel *impl_model;
};

static void list_model_iface_init (GListModelInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (
    GxContentProvider,
    gx_content_provider,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL, list_model_iface_init))

enum
{
  PROP_0,

  PROP_INPUT_FILES,
  PROP_FACTORY,
  PROP_HAS_INPUTS,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

GX_DEFINE_DATA (
    input_init,
    InputInit,
    { GFile *file; },
    GX_RELEASE_DATA (file, g_object_unref))
static DexFuture *
input_init_fiber (InputInitData *data);

GX_DEFINE_DATA (
    input_load,
    InputLoad,
    {
      GFile        *file;
      GxYamlParser *parser;
    },
    GX_RELEASE_DATA (file, g_object_unref);
    GX_RELEASE_DATA (parser, g_object_unref))
static DexFuture *
input_load_fiber (InputLoadData *data);

static void
destroy_css (GtkCssProvider *css);

GX_DEFINE_DATA (
    input_tracking,
    InputTracking,
    {
      GxContentProvider *self;
      char              *path;
      GFileMonitor      *monitor;
      DexFuture         *task;
      GListStore        *output;
      GtkCssProvider    *css;
    },
    GX_RELEASE_DATA (path, g_free);
    GX_RELEASE_DATA (monitor, g_object_unref);
    GX_RELEASE_DATA (task, dex_unref);
    GX_RELEASE_DATA (output, g_object_unref);
    GX_RELEASE_DATA (css, destroy_css))
static DexFuture *
input_init_finally (DexFuture         *future,
                    InputTrackingData *data);
static DexFuture *
input_load_finally (DexFuture         *future,
                    InputTrackingData *data);

static void
impl_model_changed (GListModel        *impl_model,
                    guint              position,
                    guint              removed,
                    guint              added,
                    GxContentProvider *self);

static void
input_files_changed (GListModel        *input_files,
                     guint              position,
                     guint              removed,
                     guint              added,
                     GxContentProvider *self);

static void
input_file_changed_on_disk (GFileMonitor      *self,
                            GFile             *file,
                            GFile             *other_file,
                            GFileMonitorEvent  event_type,
                            InputTrackingData *data);

static gboolean
commence_reload (InputTrackingData *data);

static void
gx_content_provider_dispose (GObject *object)
{
  GxContentProvider *self = GX_CONTENT_PROVIDER (object);

  g_clear_object (&self->yaml_parser);
  g_clear_object (&self->input_files);
  g_clear_object (&self->factory);
  g_clear_object (&self->input_mirror);
  g_clear_pointer (&self->input_tracking, g_hash_table_unref);
  g_clear_object (&self->outputs);
  g_clear_object (&self->impl_model);

  G_OBJECT_CLASS (gx_content_provider_parent_class)->dispose (object);
}

static void
gx_content_provider_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  GxContentProvider *self = GX_CONTENT_PROVIDER (object);

  switch (prop_id)
    {
    case PROP_INPUT_FILES:
      g_value_set_object (value, gx_content_provider_get_input_files (self));
      break;
    case PROP_FACTORY:
      g_value_set_object (value, gx_content_provider_get_factory (self));
      break;
    case PROP_HAS_INPUTS:
      g_value_set_boolean (value, gx_content_provider_get_has_inputs (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_content_provider_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  GxContentProvider *self = GX_CONTENT_PROVIDER (object);

  switch (prop_id)
    {
    case PROP_INPUT_FILES:
      gx_content_provider_set_input_files (self, g_value_get_object (value));
      break;
    case PROP_FACTORY:
      gx_content_provider_set_factory (self, g_value_get_object (value));
      break;
    case PROP_HAS_INPUTS:
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_content_provider_class_init (GxContentProviderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_content_provider_set_property;
  object_class->get_property = gx_content_provider_get_property;
  object_class->dispose      = gx_content_provider_dispose;

  props[PROP_INPUT_FILES] =
      g_param_spec_object (
          "input-files",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_FACTORY] =
      g_param_spec_object (
          "factory",
          NULL, NULL,
          GX_TYPE_APPLICATION_MAP_FACTORY,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_HAS_INPUTS] =
      g_param_spec_boolean (
          "has-inputs",
          NULL, NULL, FALSE,
          G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_content_provider_init (GxContentProvider *self)
{
  g_type_ensure (GX_TYPE_CONTENT_SECTION);
  self->yaml_parser = gx_yaml_parser_new_for_resource_schema (
      "/io/github/gamerx/GamerXStore/gx-content-provider-config-schema.xml");

  self->input_mirror   = g_list_store_new (G_TYPE_FILE);
  self->input_tracking = g_hash_table_new_full (
      g_direct_hash, g_direct_equal,
      g_object_unref, input_tracking_data_unref);

  self->outputs    = g_list_store_new (G_TYPE_LIST_MODEL);
  self->impl_model = gtk_flatten_list_model_new (g_object_ref (G_LIST_MODEL (self->outputs)));

  g_signal_connect (
      self->impl_model, "items-changed",
      G_CALLBACK (impl_model_changed), self);
}

static GType
list_model_get_item_type (GListModel *list)
{
  return GX_TYPE_CONTENT_SECTION;
}

static guint
list_model_get_n_items (GListModel *list)
{
  GxContentProvider *self = GX_CONTENT_PROVIDER (list);
  return g_list_model_get_n_items (G_LIST_MODEL (self->impl_model));
}

static gpointer
list_model_get_item (GListModel *list,
                     guint       position)
{
  GxContentProvider *self = GX_CONTENT_PROVIDER (list);
  return g_list_model_get_item (G_LIST_MODEL (self->impl_model), position);
}

static void
list_model_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = list_model_get_item_type;
  iface->get_n_items   = list_model_get_n_items;
  iface->get_item      = list_model_get_item;
}

GxContentProvider *
gx_content_provider_new (void)
{
  return g_object_new (GX_TYPE_CONTENT_PROVIDER, NULL);
}

void
gx_content_provider_set_input_files (GxContentProvider *self,
                                     GListModel        *input_files)
{
  guint old_length = 0;

  g_return_if_fail (GX_IS_CONTENT_PROVIDER (self));
  g_return_if_fail (input_files == NULL || G_IS_LIST_MODEL (input_files));

  if (self->input_files != NULL)
    {
      old_length = g_list_model_get_n_items (self->input_files);
      g_signal_handlers_disconnect_by_func (
          self->input_files, input_files_changed, self);
    }
  g_clear_object (&self->input_files);

  g_hash_table_remove_all (self->input_tracking);
  g_list_store_remove_all (self->input_mirror);
  g_list_store_remove_all (self->outputs);

  if (input_files != NULL)
    {
      self->input_files = g_object_ref (input_files);
      g_signal_connect (
          input_files, "items-changed",
          G_CALLBACK (input_files_changed), self);

      input_files_changed (
          input_files, 0, old_length,
          g_list_model_get_n_items (input_files),
          self);
    }

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_INPUT_FILES]);
}

GListModel *
gx_content_provider_get_input_files (GxContentProvider *self)
{
  g_return_val_if_fail (GX_IS_CONTENT_PROVIDER (self), NULL);
  return self->input_files;
}

void
gx_content_provider_set_factory (GxContentProvider       *self,
                                 GxApplicationMapFactory *factory)
{
  g_return_if_fail (GX_IS_CONTENT_PROVIDER (self));
  g_return_if_fail (factory == NULL || GX_IS_APPLICATION_MAP_FACTORY (factory));

  g_clear_object (&self->factory);
  if (factory != NULL)
    self->factory = g_object_ref (factory);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_INPUT_FILES]);
}

GxApplicationMapFactory *
gx_content_provider_get_factory (GxContentProvider *self)
{
  g_return_val_if_fail (GX_IS_CONTENT_PROVIDER (self), NULL);
  return self->factory;
}

gboolean
gx_content_provider_get_has_inputs (GxContentProvider *self)
{
  g_return_val_if_fail (GX_IS_CONTENT_PROVIDER (self), FALSE);
  return self->input_files != NULL &&
         g_list_model_get_n_items (self->input_files) > 0 &&
         g_list_model_get_n_items (G_LIST_MODEL (self)) > 0;
}

static void
impl_model_changed (GListModel        *impl_model,
                    guint              position,
                    guint              removed,
                    guint              added,
                    GxContentProvider *self)
{
  g_list_model_items_changed (
      G_LIST_MODEL (self), position, removed, added);
}

static void
input_files_changed (GListModel        *input_files,
                     guint              position,
                     guint              removed,
                     guint              added,
                     GxContentProvider *self)
{
  gboolean                emit_has_inputs = FALSE;
  g_autofree GFile      **additions       = NULL;
  g_autofree GListStore **new_outputs     = NULL;

  emit_has_inputs = g_list_model_get_n_items (input_files) == 0 ||
                    g_list_model_get_n_items (G_LIST_MODEL (self->input_mirror)) == 0;

  if (removed > 0)
    {
      for (guint i = 0; i < removed; i++)
        {
          g_autoptr (GFile) removal = NULL;
          InputTrackingData *data   = NULL;

          removal = g_list_model_get_item (
              G_LIST_MODEL (self->input_mirror), position + i);
          data = g_hash_table_lookup (self->input_tracking, removal);
          g_assert (data != NULL);

          dex_clear (&data->task);
          g_hash_table_remove (self->input_tracking, removal);
        }
    }

  if (added > 0)
    {
      additions = g_malloc0_n (added, sizeof (*additions));
      for (guint i = 0; i < added; i++)
        additions[i] = g_list_model_get_item (
            G_LIST_MODEL (self->input_files), position + i);

      new_outputs = g_malloc0_n (added, sizeof (*new_outputs));
      for (guint i = 0; i < added; i++)
        new_outputs[i] = g_list_store_new (GX_TYPE_CONTENT_SECTION);
    }

  g_list_store_splice (self->input_mirror, position, removed,
                       (gpointer *) additions, added);
  g_list_store_splice (self->outputs, position, removed,
                       (gpointer *) new_outputs, added);

  for (guint i = 0; i < added; i++)
    {
      g_autoptr (InputInitData) init_data = NULL;
      g_autoptr (InputTrackingData) data  = NULL;
      g_autoptr (DexFuture) future        = NULL;

      init_data       = input_init_data_new ();
      init_data->file = g_object_ref (additions[i]);

      future = dex_scheduler_spawn (
          gx_get_io_scheduler (),
          gx_get_dex_stack_size (),
          (DexFiberFunc) input_init_fiber,
          input_init_data_ref (init_data),
          input_init_data_unref);

      data         = input_tracking_data_new ();
      data->self   = self;
      data->path   = g_file_get_path (additions[i]);
      data->output = g_steal_pointer (&new_outputs[i]);

      future = dex_future_finally (
          future,
          (DexFutureCallback) input_init_finally,
          input_tracking_data_ref (data),
          input_tracking_data_unref);
      dex_future_disown (g_steal_pointer (&future));

      g_hash_table_replace (
          self->input_tracking,
          g_steal_pointer (&additions[i]),
          input_tracking_data_ref (data));
    }

  if (emit_has_inputs)
    g_object_notify_by_pspec (G_OBJECT (self), props[PROP_HAS_INPUTS]);
}

static void
input_file_changed_on_disk (GFileMonitor      *self,
                            GFile             *file,
                            GFile             *other_file,
                            GFileMonitorEvent  event_type,
                            InputTrackingData *data)
{
  if (event_type == G_FILE_MONITOR_EVENT_CHANGED ||
      event_type == G_FILE_MONITOR_EVENT_CREATED ||
      event_type == G_FILE_MONITOR_EVENT_DELETED)
    g_idle_add_full (
        G_PRIORITY_DEFAULT,
        (GSourceFunc) commence_reload,
        input_tracking_data_ref (data),
        input_tracking_data_unref);
}

static DexFuture *
input_init_fiber (InputInitData *data)
{
  GFile *file                      = data->file;
  g_autoptr (GError) local_error   = NULL;
  g_autoptr (GFileMonitor) monitor = NULL;

  monitor = g_file_monitor_file (
      file, G_FILE_MONITOR_NONE, NULL, &local_error);
  if (monitor == NULL)
    return dex_future_new_for_error (g_steal_pointer (&local_error));

  return dex_future_new_for_object (monitor);
}

static DexFuture *
input_init_finally (DexFuture         *future,
                    InputTrackingData *data)
{
  g_autoptr (GError) local_error = NULL;
  const GValue *value            = NULL;

  g_list_store_remove_all (data->output);

  value = dex_future_get_value (future, &local_error);
  if (value != NULL)
    {
      data->monitor = g_value_dup_object (value);
      g_signal_connect (
          data->monitor, "changed",
          G_CALLBACK (input_file_changed_on_disk),
          data);

      commence_reload (data);
    }
  else
    {
      // g_autoptr (GxContentSection) error_section = NULL;

      g_critical ("Could not init curated config watch at path %s: %s",
                  data->path, local_error->message);

      // error_section = g_object_new (
      //     GX_TYPE_CONTENT_SECTION,
      //     "error", local_error->message,
      //     NULL);
      // g_list_store_append (data->output, error_section);
    }

  return NULL;
}

static DexFuture *
input_load_fiber (InputLoadData *data)
{
  GFile        *file                   = data->file;
  GxYamlParser *parser                 = data->parser;
  g_autoptr (GPtrArray) css            = NULL;
  g_autoptr (GPtrArray) sections       = NULL;
  g_autoptr (GError) local_error       = NULL;
  g_autoptr (GBytes) bytes             = NULL;
  g_autoptr (GHashTable) parse_results = NULL;
  g_autoptr (GHashTable) dict          = NULL;

  css      = g_ptr_array_new_with_free_func (g_free);
  sections = g_ptr_array_new_with_free_func (g_object_unref);

  bytes = dex_await_boxed (dex_file_load_contents_bytes (file), &local_error);
  if (bytes == NULL)
    return dex_future_new_for_error (g_steal_pointer (&local_error));

  parse_results = gx_yaml_parser_process_bytes (parser, bytes, &local_error);
  if (parse_results == NULL)
    return dex_future_new_for_error (g_steal_pointer (&local_error));

  if (g_hash_table_contains (parse_results, "/css"))
    {
      const char *css_string = NULL;

      css_string = g_variant_get_string (
          g_value_get_variant (
              g_hash_table_lookup (
                  parse_results, "/css")),
          NULL);
      g_ptr_array_add (css, g_strdup (css_string));
    }

  if (g_hash_table_contains (parse_results, "/sections"))
    {
      GPtrArray *list = NULL;

      list = g_value_get_boxed (g_hash_table_lookup (parse_results, "/sections"));

      for (guint i = 0; i < list->len; i++)
        {
          GHashTable *section_props            = NULL;
          g_autoptr (GxContentSection) section = NULL;

          section_props = g_value_get_boxed (
              g_hash_table_lookup (
                  g_value_get_boxed (
                      g_ptr_array_index (list, i)),
                  "/"));
          section = g_object_new (GX_TYPE_CONTENT_SECTION, NULL);

#define GRAB_STRING(s)                            \
  if (g_hash_table_contains (section_props, (s))) \
    g_object_set (                                \
        section,                                  \
        (s),                                      \
        g_variant_get_string (                    \
            g_value_get_variant (                 \
                g_hash_table_lookup (             \
                    section_props, (s))),         \
            NULL),                                \
        NULL);

          GRAB_STRING ("title");
          GRAB_STRING ("subtitle");
          GRAB_STRING ("description");

#undef GRAB_STRING

#define GRAB_INT(s)                               \
  if (g_hash_table_contains (section_props, (s))) \
    g_object_set (                                \
        section,                                  \
        (s),                                      \
        g_variant_get_int32 (                     \
            g_value_get_variant (                 \
                g_hash_table_lookup (             \
                    section_props, (s)))),        \
        NULL);

          GRAB_INT ("banner-height");
          GRAB_INT ("rows");

#undef GRAB_INT

#define GRAB_DOUBLE(s)                            \
  if (g_hash_table_contains (section_props, (s))) \
    g_object_set (                                \
        section,                                  \
        (s),                                      \
        g_variant_get_double (                    \
            g_value_get_variant (                 \
                g_hash_table_lookup (             \
                    section_props, (s)))),        \
        NULL);

          GRAB_DOUBLE ("banner-text-label-xalign");

#undef GRAB_DOUBLE

#define GRAB_ENUM(s)                              \
  if (g_hash_table_contains (section_props, (s))) \
    g_object_set (                                \
        section,                                  \
        (s),                                      \
        g_value_get_enum (                        \
            g_hash_table_lookup (                 \
                section_props, (s))),             \
        NULL);

          GRAB_ENUM ("banner-text-valign");
          GRAB_ENUM ("banner-text-halign");
          GRAB_ENUM ("banner-fit");

#undef GRAB_ENUM

          if (g_hash_table_contains (section_props, "banner"))
            {
              const char *string       = NULL;
              g_autoptr (GFile) source = NULL;

              string = g_variant_get_string (
                  g_value_get_variant (
                      g_hash_table_lookup (
                          section_props, "banner")),
                  NULL);
              source = g_file_new_for_uri (string);

              if (source != NULL)
                {
                  g_autoptr (GxAsyncTexture) texture = NULL;

                  texture = gx_async_texture_new (source, NULL);
                  g_object_set (section, "banner", texture, NULL);
                }
            }

          if (g_hash_table_contains (section_props, "appids"))
            {
              GPtrArray *appids                 = NULL;
              g_autoptr (GtkStringList) strlist = NULL;

              appids = g_value_get_boxed (
                  g_hash_table_lookup (section_props, "appids"));
              strlist = gtk_string_list_new (NULL);

              for (guint j = 0; j < appids->len; j++)
                {
                  const char *appid = NULL;

                  appid = g_variant_get_string (
                      g_value_get_variant (
                          g_ptr_array_index (appids, j)),
                      NULL);

                  gtk_string_list_append (strlist, appid);
                }

              g_object_set (section, "appids", strlist, NULL);
            }

#define GRAB_CLASSES(key)                              \
  if (g_hash_table_contains (section_props, (key)))    \
    {                                                  \
      GPtrArray *classes                = NULL;        \
      g_autoptr (GtkStringList) strlist = NULL;        \
                                                       \
      classes = g_value_get_boxed (                    \
          g_hash_table_lookup (section_props, (key))); \
      strlist = gtk_string_list_new (NULL);            \
                                                       \
      for (guint j = 0; j < classes->len; j++)         \
        {                                              \
          const char *class = NULL;                    \
                                                       \
          class = g_variant_get_string (               \
              g_value_get_variant (                    \
                  g_ptr_array_index (classes, j)),     \
              NULL);                                   \
                                                       \
          gtk_string_list_append (strlist, class);     \
        }                                              \
                                                       \
      g_object_set (section, (key), strlist, NULL);    \
    }

          GRAB_CLASSES ("classes")
          GRAB_CLASSES ("dark-classes")
          GRAB_CLASSES ("light-classes")

#undef GRAB_CLASSES

          g_ptr_array_add (sections, g_steal_pointer (&section));
        }
    }

  dict = g_hash_table_new_full (
      g_str_hash, g_str_equal,
      g_free, (GDestroyNotify) g_ptr_array_unref);
  g_hash_table_replace (
      dict, g_strdup ("css"),
      g_steal_pointer (&css));
  g_hash_table_replace (
      dict, g_strdup ("sections"),
      g_steal_pointer (&sections));

  return dex_future_new_take_boxed (
      G_TYPE_HASH_TABLE,
      g_steal_pointer (&dict));
}

static DexFuture *
input_load_finally (DexFuture         *future,
                    InputTrackingData *data)
{
  g_autoptr (GError) local_error = NULL;
  const GValue *value            = NULL;

  g_list_store_remove_all (data->output);
  g_clear_pointer (&data->css, destroy_css);

  value = dex_future_get_value (future, &local_error);
  if (value != NULL)
    {
      GHashTable *dict     = NULL;
      GPtrArray  *css      = NULL;
      GPtrArray  *sections = NULL;

      dict     = g_value_get_boxed (value);
      css      = g_hash_table_lookup (dict, "css");
      sections = g_hash_table_lookup (dict, "sections");

      g_assert (css != NULL);
      g_assert (sections != NULL);

      if (css->pdata != NULL && css->len > 0)
        {
          const char *css_string = NULL;

          css_string = g_ptr_array_index (css, 0);

          data->css = gtk_css_provider_new ();
          gtk_css_provider_load_from_string (
              data->css, css_string);
          gtk_style_context_add_provider_for_display (
              gdk_display_get_default (),
              GTK_STYLE_PROVIDER (data->css),
              GTK_STYLE_PROVIDER_PRIORITY_USER);
        }

      if (sections->pdata != NULL && sections->len > 0)
        {
          if (data->self->factory != NULL)
            {
              for (guint i = 0; i < sections->len; i++)
                {
                  GxContentSection *section     = NULL;
                  g_autoptr (GListModel) appids = NULL;

                  section = g_ptr_array_index (sections, i);
                  g_object_get (section, "appids", &appids, NULL);

                  if (appids != NULL)
                    {
                      g_autoptr (GListModel) converted = NULL;

                      converted = gx_application_map_factory_generate (data->self->factory, appids);
                      g_object_set (section, "appids", converted, NULL);
                    }
                }
            }

          g_list_store_splice (data->output, 0, 0, sections->pdata, sections->len);
        }
    }
  else
    {
      // g_autoptr (GxContentSection) error_section = NULL;

      g_critical ("Could not load curated config at path %s: %s",
                  data->path, local_error->message);

      // error_section = g_object_new (
      //     GX_TYPE_CONTENT_SECTION,
      //     "error", local_error->message,
      //     NULL);
      // g_list_store_append (data->output, error_section);
    }

  g_object_notify_by_pspec (G_OBJECT (data->self), props[PROP_HAS_INPUTS]);
  return NULL;
}

static gboolean
commence_reload (InputTrackingData *data)
{
  g_autoptr (InputLoadData) load_data = NULL;
  DexFuture *future                   = NULL;

  dex_clear (&data->task);

  load_data         = input_load_data_new ();
  load_data->file   = g_file_new_for_path (data->path);
  load_data->parser = g_object_ref (data->self->yaml_parser);

  future = dex_scheduler_spawn (
      gx_get_io_scheduler (),
      gx_get_dex_stack_size (),
      (DexFiberFunc) input_load_fiber,
      input_load_data_ref (load_data),
      input_load_data_unref);
  future = dex_future_finally (
      future,
      (DexFutureCallback) input_load_finally,
      input_tracking_data_ref (data),
      input_tracking_data_unref);
  data->task = future;

  return G_SOURCE_REMOVE;
}

static void
destroy_css (GtkCssProvider *css)
{
  gtk_style_context_remove_provider_for_display (
      gdk_display_get_default (),
      GTK_STYLE_PROVIDER (css));
  g_object_unref (css);
}
