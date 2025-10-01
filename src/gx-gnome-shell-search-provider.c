/* bz-gnome-shell-search-provider.c
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

#include "gx-gnome-shell-search-provider.h"
#include "gx-entry-group.h"
#include "gx-search-result.h"
#include "gx-util.h"
#include "gs-shell-search-provider-generated.h"

struct _GxGnomeShellSearchProvider
{
  GObject parent_instance;

  GxSearchEngine  *engine;
  GDBusConnection *connection;

  GxShellSearchProvider2 *skeleton;
  DexFuture              *task;

  GHashTable *last_results;
};

G_DEFINE_FINAL_TYPE (GxGnomeShellSearchProvider, gx_gnome_shell_search_provider, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_ENGINE,
  PROP_CONNECTION,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

GX_DEFINE_DATA (
    request,
    Request,
    {
      GxGnomeShellSearchProvider *self;
      GDBusMethodInvocation      *invocation;
      GApplication               *application;
    },
    GX_RELEASE_DATA (invocation, g_object_unref);
    GX_RELEASE_DATA (application, g_application_release);)
static DexFuture *
request_finally (DexFuture   *future,
                 RequestData *data);

static void
start_request (GxGnomeShellSearchProvider *self,
               GDBusMethodInvocation      *invocation,
               const char *const          *terms);

static void
gx_gnome_shell_search_provider_dispose (GObject *object)
{
  GxGnomeShellSearchProvider *self = GX_GNOME_SHELL_SEARCH_PROVIDER (object);

  dex_clear (&self->task);

  g_clear_object (&self->engine);
  g_clear_object (&self->connection);
  g_clear_object (&self->skeleton);
  g_clear_pointer (&self->last_results, g_hash_table_unref);

  G_OBJECT_CLASS (gx_gnome_shell_search_provider_parent_class)->dispose (object);
}

static void
gx_gnome_shell_search_provider_get_property (GObject    *object,
                                             guint       prop_id,
                                             GValue     *value,
                                             GParamSpec *pspec)
{
  GxGnomeShellSearchProvider *self = GX_GNOME_SHELL_SEARCH_PROVIDER (object);

  switch (prop_id)
    {
    case PROP_ENGINE:
      g_value_set_object (value, gx_gnome_shell_search_provider_get_engine (self));
      break;
    case PROP_CONNECTION:
      g_value_set_object (value, gx_gnome_shell_search_provider_get_connection (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_gnome_shell_search_provider_set_property (GObject      *object,
                                             guint         prop_id,
                                             const GValue *value,
                                             GParamSpec   *pspec)
{
  GxGnomeShellSearchProvider *self = GX_GNOME_SHELL_SEARCH_PROVIDER (object);

  switch (prop_id)
    {
    case PROP_ENGINE:
      gx_gnome_shell_search_provider_set_engine (self, g_value_get_object (value));
      break;
    case PROP_CONNECTION:
      gx_gnome_shell_search_provider_set_connection (self, g_value_get_object (value), NULL);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_gnome_shell_search_provider_class_init (GxGnomeShellSearchProviderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_gnome_shell_search_provider_set_property;
  object_class->get_property = gx_gnome_shell_search_provider_get_property;
  object_class->dispose      = gx_gnome_shell_search_provider_dispose;

  props[PROP_ENGINE] =
      g_param_spec_object (
          "engine",
          NULL, NULL,
          GX_TYPE_SEARCH_ENGINE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_CONNECTION] =
      g_param_spec_object (
          "connection",
          NULL, NULL,
          G_TYPE_DBUS_CONNECTION,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static gboolean
get_initial_result_set (GxShellSearchProvider2     *skeleton,
                        GDBusMethodInvocation      *invocation,
                        gchar                     **terms,
                        GxGnomeShellSearchProvider *self)
{
  start_request (self, invocation, (const char *const *) terms);
  return TRUE;
}

static gboolean
get_subsearch_result_set (GxShellSearchProvider2     *skeleton,
                          GDBusMethodInvocation      *invocation,
                          gchar                     **previous_results,
                          gchar                     **terms,
                          GxGnomeShellSearchProvider *self)
{
  start_request (self, invocation, (const char *const *) terms);
  return TRUE;
}

static gboolean
get_result_metas (GxShellSearchProvider2     *skeleton,
                  GDBusMethodInvocation      *invocation,
                  gchar                     **results,
                  GxGnomeShellSearchProvider *self)
{
  g_autoptr (GVariantBuilder) builder = NULL;

  builder = g_variant_builder_new (G_VARIANT_TYPE ("aa{sv}"));

  for (char **result = results; *result != NULL; result++)
    {
      GxEntryGroup *group                      = NULL;
      g_autoptr (GVariantBuilder) meta_builder = NULL;
      const char *title                        = NULL;
      const char *description                  = NULL;
      GIcon      *icon                         = NULL;

      group = g_hash_table_lookup (self->last_results, *result);
      if (group == NULL)
        {
          g_critical ("failed to find '%s' in gnome-shell search result cache", *result);
          continue;
        }

      meta_builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
      g_variant_builder_add (meta_builder, "{sv}", "id", g_variant_new_string (*result));

      title = gx_entry_group_get_title (group);
      g_variant_builder_add (meta_builder, "{sv}", "name", g_variant_new_string (title));

      description = gx_entry_group_get_description (group);
      if (description != NULL)
        g_variant_builder_add (meta_builder, "{sv}", "description", g_variant_new_string (description));

      icon = gx_entry_group_get_mini_icon (group);
      if (icon != NULL)
        {
          g_autofree gchar *icon_str = g_icon_to_string (icon);
          if (icon_str != NULL)
            g_variant_builder_add (meta_builder, "{sv}", "gicon", g_variant_new_string (icon_str));
          else
            {
              g_autoptr (GVariant) icon_serialized = NULL;

              icon_serialized = g_icon_serialize (icon);
              if (icon_serialized != NULL)
                g_variant_builder_add (meta_builder, "{sv}", "icon", icon_serialized);
            }
        }

      g_variant_builder_add_value (builder, g_variant_builder_end (meta_builder));
    }

  g_dbus_method_invocation_return_value (invocation, g_variant_new ("(aa{sv})", builder));
  return TRUE;
}

static gboolean
activate_result (GxShellSearchProvider2     *skeleton,
                 GDBusMethodInvocation      *invocation,
                 gchar                      *result,
                 gchar                     **terms,
                 guint32                     timestamp,
                 GxGnomeShellSearchProvider *self)
{
  g_action_group_activate_action (
      G_ACTION_GROUP (g_application_get_default ()),
      "search",
      g_variant_new ("s", result));

  gx_shell_search_provider2_complete_activate_result (skeleton, invocation);
  return TRUE;
}

static gboolean
launch_search (GxShellSearchProvider2     *skeleton,
               GDBusMethodInvocation      *invocation,
               gchar                     **terms,
               guint32                     timestamp,
               GxGnomeShellSearchProvider *self)
{
  g_autofree gchar *string = NULL;

  string = g_strjoinv (" ", terms);
  g_action_group_activate_action (
      G_ACTION_GROUP (g_application_get_default ()),
      "search",
      g_variant_new ("s", string));

  gx_shell_search_provider2_complete_launch_search (skeleton, invocation);
  return TRUE;
}

static void
gx_gnome_shell_search_provider_init (GxGnomeShellSearchProvider *self)
{
  self->skeleton     = gx_shell_search_provider2_skeleton_new ();
  self->last_results = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);

  g_signal_connect (
      self->skeleton, "handle-get-initial-result-set",
      G_CALLBACK (get_initial_result_set), self);
  g_signal_connect (
      self->skeleton, "handle-get-subsearch-result-set",
      G_CALLBACK (get_subsearch_result_set), self);
  g_signal_connect (
      self->skeleton, "handle-get-result-metas",
      G_CALLBACK (get_result_metas), self);
  g_signal_connect (
      self->skeleton, "handle-activate-result",
      G_CALLBACK (activate_result), self);
  g_signal_connect (
      self->skeleton, "handle-launch-search",
      G_CALLBACK (launch_search), self);
}

GxGnomeShellSearchProvider *
gx_gnome_shell_search_provider_new (void)
{
  return g_object_new (GX_TYPE_GNOME_SHELL_SEARCH_PROVIDER, NULL);
}

GxSearchEngine *
gx_gnome_shell_search_provider_get_engine (GxGnomeShellSearchProvider *self)
{
  g_return_val_if_fail (GX_IS_GNOME_SHELL_SEARCH_PROVIDER (self), NULL);
  return self->engine;
}

void
gx_gnome_shell_search_provider_set_engine (GxGnomeShellSearchProvider *self,
                                           GxSearchEngine             *engine)
{
  g_return_if_fail (GX_IS_GNOME_SHELL_SEARCH_PROVIDER (self));
  g_return_if_fail (engine == NULL || GX_IS_SEARCH_ENGINE (engine));

  g_clear_pointer (&self->engine, g_object_unref);
  if (engine != NULL)
    self->engine = g_object_ref (engine);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ENGINE]);
}

GDBusConnection *
gx_gnome_shell_search_provider_get_connection (GxGnomeShellSearchProvider *self)
{
  g_return_val_if_fail (GX_IS_GNOME_SHELL_SEARCH_PROVIDER (self), NULL);
  return self->connection;
}

gboolean
gx_gnome_shell_search_provider_set_connection (GxGnomeShellSearchProvider *self,
                                               GDBusConnection            *connection,
                                               GError                    **error)
{
  gboolean success = TRUE;

  g_return_val_if_fail (GX_IS_GNOME_SHELL_SEARCH_PROVIDER (self), FALSE);
  g_return_val_if_fail (connection == NULL || G_IS_DBUS_CONNECTION (connection), FALSE);

  if (self->connection != NULL)
    g_dbus_interface_skeleton_unexport (G_DBUS_INTERFACE_SKELETON (self->skeleton));
  g_clear_pointer (&self->connection, g_object_unref);

  if (connection != NULL)
    {
      g_autoptr (GError) local_error = NULL;

      self->connection = g_object_ref (connection);

      success = g_dbus_interface_skeleton_export (
          G_DBUS_INTERFACE_SKELETON (self->skeleton),
          connection,
          "/io/github/gamerx/GamerXStore/SearchProvider",
          &local_error);
      if (!success)
        {
          if (error != NULL)
            g_propagate_error (error, g_steal_pointer (&local_error));
          else
            g_critical ("Could not register gnome shell search provider: %s", local_error->message);
        }
    }

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_CONNECTION]);
  return success;
}

static DexFuture *
request_finally (DexFuture   *future,
                 RequestData *data)
{
  GxGnomeShellSearchProvider *self       = data->self;
  GDBusMethodInvocation      *invocation = data->invocation;
  g_autoptr (GError) local_error         = NULL;
  const GValue *value                    = NULL;
  GPtrArray    *results                  = NULL;
  g_autoptr (GVariantBuilder) builder    = NULL;

  value = dex_future_get_value (future, &local_error);
  if (value != NULL)
    {
      results = g_value_get_boxed (value);
      builder = g_variant_builder_new (G_VARIANT_TYPE ("as"));

      for (guint i = 0; i < results->len; i++)
        {
          GxSearchResult *result = NULL;
          GxEntryGroup   *group  = NULL;
          const char     *id     = NULL;

          result = g_ptr_array_index (results, i);
          group  = gx_search_result_get_group (result);
          id     = gx_entry_group_get_id (group);

          g_variant_builder_add (builder, "s", id);
          g_hash_table_replace (
              self->last_results,
              g_strdup (id),
              g_object_ref (group));
        }

      g_dbus_method_invocation_return_value (
          invocation,
          g_variant_new ("(as)", builder));
    }
  else
    {
      g_critical ("search engine reported an error to the search provider, "
                  "returning an empty response to invocation: %s",
                  local_error->message);
      g_dbus_method_invocation_return_value (invocation, g_variant_new ("(as)", NULL));
    }

  return NULL;
}

static void
start_request (GxGnomeShellSearchProvider *self,
               GDBusMethodInvocation      *invocation,
               const char *const          *terms)
{
  g_autoptr (RequestData) data = NULL;
  g_autoptr (DexFuture) future = NULL;

  dex_clear (&self->task);
  g_hash_table_remove_all (self->last_results);

  if (g_strv_length ((gchar **) terms) == 1 &&
      g_utf8_strlen (terms[0], -1) == 1)
    {
      g_dbus_method_invocation_return_value (
          invocation,
          g_variant_new ("(as)", NULL));
      return;
    }

  if (self->engine == NULL)
    {
      g_critical ("search provider does not have an engine, "
                  "returning empty response to invocation");
      g_dbus_method_invocation_return_value (
          invocation,
          g_variant_new ("(as)", NULL));
      return;
    }

  data              = request_data_new ();
  data->self        = self;
  data->invocation  = g_object_ref (invocation);
  data->application = g_application_get_default ();
  g_application_hold (data->application);

  future = gx_search_engine_query (self->engine, terms);
  future = dex_future_finally (
      future, (DexFutureCallback) request_finally,
      request_data_ref (data), request_data_unref);
  self->task = g_steal_pointer (&future);
}

/* End of bz-gnome-shell-search-provider.c */
