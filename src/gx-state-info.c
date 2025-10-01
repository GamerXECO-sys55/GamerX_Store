/* bz-state-info.c
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

#include "gx-state-info.h"

struct _GxStateInfo
{
  GObject parent_instance;

  GSettings               *settings;
  GHashTable              *main_config;
  GListModel              *blocklists;
  GListModel              *curated_configs;
  GxBackend               *backend;
  GxEntryCacheManager     *cache_manager;
  GxTransactionManager    *transaction_manager;
  GListModel              *available_updates;
  GxApplicationMapFactory *entry_factory;
  GxApplicationMapFactory *application_factory;
  GListModel              *all_entries;
  GListModel              *all_entry_groups;
  GListModel              *all_installed_entry_groups;
  GxSearchEngine          *search_engine;
  GxContentProvider       *curated_provider;
  GxFlathubState          *flathub;
  gboolean                 busy;
  char                    *busy_step_label;
  char                    *busy_progress_label;
  double                   busy_progress;
  gboolean                 online;
  gboolean                 checking_for_updates;
  char                    *background_task_label;
};

G_DEFINE_FINAL_TYPE (GxStateInfo, gx_state_info, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_SETTINGS,
  PROP_MAIN_CONFIG,
  PROP_BLOCKLISTS,
  PROP_CURATED_CONFIGS,
  PROP_BACKEND,
  PROP_CACHE_MANAGER,
  PROP_TRANSACTION_MANAGER,
  PROP_AVAILABLE_UPDATES,
  PROP_ENTRY_FACTORY,
  PROP_APPLICATION_FACTORY,
  PROP_ALL_ENTRIES,
  PROP_ALL_ENTRY_GROUPS,
  PROP_ALL_INSTALLED_ENTRY_GROUPS,
  PROP_SEARCH_ENGINE,
  PROP_CURATED_PROVIDER,
  PROP_FLATHUB,
  PROP_BUSY,
  PROP_BUSY_STEP_LABEL,
  PROP_BUSY_PROGRESS_LABEL,
  PROP_BUSY_PROGRESS,
  PROP_ONLINE,
  PROP_CHECKING_FOR_UPDATES,
  PROP_BACKGROUND_TASK_LABEL,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_state_info_dispose (GObject *object)
{
  GxStateInfo *self = GX_STATE_INFO (object);

  g_clear_pointer (&self->settings, g_object_unref);
  g_clear_pointer (&self->main_config, g_hash_table_unref);
  g_clear_pointer (&self->blocklists, g_object_unref);
  g_clear_pointer (&self->curated_configs, g_object_unref);
  g_clear_pointer (&self->backend, g_object_unref);
  g_clear_pointer (&self->cache_manager, g_object_unref);
  g_clear_pointer (&self->transaction_manager, g_object_unref);
  g_clear_pointer (&self->available_updates, g_object_unref);
  g_clear_pointer (&self->entry_factory, g_object_unref);
  g_clear_pointer (&self->application_factory, g_object_unref);
  g_clear_pointer (&self->all_entries, g_object_unref);
  g_clear_pointer (&self->all_entry_groups, g_object_unref);
  g_clear_pointer (&self->all_installed_entry_groups, g_object_unref);
  g_clear_pointer (&self->search_engine, g_object_unref);
  g_clear_pointer (&self->curated_provider, g_object_unref);
  g_clear_pointer (&self->flathub, g_object_unref);
  g_clear_pointer (&self->busy_step_label, g_free);
  g_clear_pointer (&self->busy_progress_label, g_free);
  g_clear_pointer (&self->background_task_label, g_free);

  G_OBJECT_CLASS (gx_state_info_parent_class)->dispose (object);
}

static void
gx_state_info_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  GxStateInfo *self = GX_STATE_INFO (object);

  switch (prop_id)
    {
    case PROP_SETTINGS:
      g_value_set_object (value, gx_state_info_get_settings (self));
      break;
    case PROP_MAIN_CONFIG:
      g_value_set_boxed (value, gx_state_info_get_main_config (self));
      break;
    case PROP_BLOCKLISTS:
      g_value_set_object (value, gx_state_info_get_blocklists (self));
      break;
    case PROP_CURATED_CONFIGS:
      g_value_set_object (value, gx_state_info_get_curated_configs (self));
      break;
    case PROP_BACKEND:
      g_value_set_object (value, gx_state_info_get_backend (self));
      break;
    case PROP_CACHE_MANAGER:
      g_value_set_object (value, gx_state_info_get_cache_manager (self));
      break;
    case PROP_TRANSACTION_MANAGER:
      g_value_set_object (value, gx_state_info_get_transaction_manager (self));
      break;
    case PROP_AVAILABLE_UPDATES:
      g_value_set_object (value, gx_state_info_get_available_updates (self));
      break;
    case PROP_ENTRY_FACTORY:
      g_value_set_object (value, gx_state_info_get_entry_factory (self));
      break;
    case PROP_APPLICATION_FACTORY:
      g_value_set_object (value, gx_state_info_get_application_factory (self));
      break;
    case PROP_ALL_ENTRIES:
      g_value_set_object (value, gx_state_info_get_all_entries (self));
      break;
    case PROP_ALL_ENTRY_GROUPS:
      g_value_set_object (value, gx_state_info_get_all_entry_groups (self));
      break;
    case PROP_ALL_INSTALLED_ENTRY_GROUPS:
      g_value_set_object (value, gx_state_info_get_all_installed_entry_groups (self));
      break;
    case PROP_SEARCH_ENGINE:
      g_value_set_object (value, gx_state_info_get_search_engine (self));
      break;
    case PROP_CURATED_PROVIDER:
      g_value_set_object (value, gx_state_info_get_curated_provider (self));
      break;
    case PROP_FLATHUB:
      g_value_set_object (value, gx_state_info_get_flathub (self));
      break;
    case PROP_BUSY:
      g_value_set_boolean (value, gx_state_info_get_busy (self));
      break;
    case PROP_BUSY_STEP_LABEL:
      g_value_set_string (value, gx_state_info_get_busy_step_label (self));
      break;
    case PROP_BUSY_PROGRESS_LABEL:
      g_value_set_string (value, gx_state_info_get_busy_progress_label (self));
      break;
    case PROP_BUSY_PROGRESS:
      g_value_set_double (value, gx_state_info_get_busy_progress (self));
      break;
    case PROP_ONLINE:
      g_value_set_boolean (value, gx_state_info_get_online (self));
      break;
    case PROP_CHECKING_FOR_UPDATES:
      g_value_set_boolean (value, gx_state_info_get_checking_for_updates (self));
      break;
    case PROP_BACKGROUND_TASK_LABEL:
      g_value_set_string (value, gx_state_info_get_background_task_label (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_state_info_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  GxStateInfo *self = GX_STATE_INFO (object);

  switch (prop_id)
    {
    case PROP_SETTINGS:
      gx_state_info_set_settings (self, g_value_get_object (value));
      break;
    case PROP_MAIN_CONFIG:
      gx_state_info_set_main_config (self, g_value_get_boxed (value));
      break;
    case PROP_BLOCKLISTS:
      gx_state_info_set_blocklists (self, g_value_get_object (value));
      break;
    case PROP_CURATED_CONFIGS:
      gx_state_info_set_curated_configs (self, g_value_get_object (value));
      break;
    case PROP_BACKEND:
      gx_state_info_set_backend (self, g_value_get_object (value));
      break;
    case PROP_CACHE_MANAGER:
      gx_state_info_set_cache_manager (self, g_value_get_object (value));
      break;
    case PROP_TRANSACTION_MANAGER:
      gx_state_info_set_transaction_manager (self, g_value_get_object (value));
      break;
    case PROP_AVAILABLE_UPDATES:
      gx_state_info_set_available_updates (self, g_value_get_object (value));
      break;
    case PROP_ENTRY_FACTORY:
      gx_state_info_set_entry_factory (self, g_value_get_object (value));
      break;
    case PROP_APPLICATION_FACTORY:
      gx_state_info_set_application_factory (self, g_value_get_object (value));
      break;
    case PROP_ALL_ENTRIES:
      gx_state_info_set_all_entries (self, g_value_get_object (value));
      break;
    case PROP_ALL_ENTRY_GROUPS:
      gx_state_info_set_all_entry_groups (self, g_value_get_object (value));
      break;
    case PROP_ALL_INSTALLED_ENTRY_GROUPS:
      gx_state_info_set_all_installed_entry_groups (self, g_value_get_object (value));
      break;
    case PROP_SEARCH_ENGINE:
      gx_state_info_set_search_engine (self, g_value_get_object (value));
      break;
    case PROP_CURATED_PROVIDER:
      gx_state_info_set_curated_provider (self, g_value_get_object (value));
      break;
    case PROP_FLATHUB:
      gx_state_info_set_flathub (self, g_value_get_object (value));
      break;
    case PROP_BUSY:
      gx_state_info_set_busy (self, g_value_get_boolean (value));
      break;
    case PROP_BUSY_STEP_LABEL:
      gx_state_info_set_busy_step_label (self, g_value_get_string (value));
      break;
    case PROP_BUSY_PROGRESS_LABEL:
      gx_state_info_set_busy_progress_label (self, g_value_get_string (value));
      break;
    case PROP_BUSY_PROGRESS:
      gx_state_info_set_busy_progress (self, g_value_get_double (value));
      break;
    case PROP_ONLINE:
      gx_state_info_set_online (self, g_value_get_boolean (value));
      break;
    case PROP_CHECKING_FOR_UPDATES:
      gx_state_info_set_checking_for_updates (self, g_value_get_boolean (value));
      break;
    case PROP_BACKGROUND_TASK_LABEL:
      gx_state_info_set_background_task_label (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_state_info_class_init (GxStateInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_state_info_set_property;
  object_class->get_property = gx_state_info_get_property;
  object_class->dispose      = gx_state_info_dispose;

  props[PROP_SETTINGS] =
      g_param_spec_object (
          "settings",
          NULL, NULL,
          G_TYPE_SETTINGS,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_MAIN_CONFIG] =
      g_param_spec_boxed (
          "main-config",
          NULL, NULL,
          G_TYPE_HASH_TABLE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BLOCKLISTS] =
      g_param_spec_object (
          "blocklists",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_CURATED_CONFIGS] =
      g_param_spec_object (
          "curated-configs",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BACKEND] =
      g_param_spec_object (
          "backend",
          NULL, NULL,
          GX_TYPE_BACKEND,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_CACHE_MANAGER] =
      g_param_spec_object (
          "cache-manager",
          NULL, NULL,
          GX_TYPE_ENTRY_CACHE_MANAGER,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_TRANSACTION_MANAGER] =
      g_param_spec_object (
          "transaction-manager",
          NULL, NULL,
          GX_TYPE_TRANSACTION_MANAGER,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_AVAILABLE_UPDATES] =
      g_param_spec_object (
          "available-updates",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ENTRY_FACTORY] =
      g_param_spec_object (
          "entry-factory",
          NULL, NULL,
          GX_TYPE_APPLICATION_MAP_FACTORY,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_APPLICATION_FACTORY] =
      g_param_spec_object (
          "application-factory",
          NULL, NULL,
          GX_TYPE_APPLICATION_MAP_FACTORY,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ALL_ENTRIES] =
      g_param_spec_object (
          "all-entries",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ALL_ENTRY_GROUPS] =
      g_param_spec_object (
          "all-entry-groups",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ALL_INSTALLED_ENTRY_GROUPS] =
      g_param_spec_object (
          "all-installed-entry-groups",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_SEARCH_ENGINE] =
      g_param_spec_object (
          "search-engine",
          NULL, NULL,
          GX_TYPE_SEARCH_ENGINE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_CURATED_PROVIDER] =
      g_param_spec_object (
          "curated-provider",
          NULL, NULL,
          GX_TYPE_CONTENT_PROVIDER,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_FLATHUB] =
      g_param_spec_object (
          "flathub",
          NULL, NULL,
          GX_TYPE_FLATHUB_STATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BUSY] =
      g_param_spec_boolean (
          "busy",
          NULL, NULL, FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BUSY_STEP_LABEL] =
      g_param_spec_string (
          "busy-step-label",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BUSY_PROGRESS_LABEL] =
      g_param_spec_string (
          "busy-progress-label",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BUSY_PROGRESS] =
      g_param_spec_double (
          "busy-progress",
          NULL, NULL,
          0.0, G_MAXDOUBLE, 0.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ONLINE] =
      g_param_spec_boolean (
          "online",
          NULL, NULL, FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_CHECKING_FOR_UPDATES] =
      g_param_spec_boolean (
          "checking-for-updates",
          NULL, NULL, FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_BACKGROUND_TASK_LABEL] =
      g_param_spec_string (
          "background-task-label",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_state_info_init (GxStateInfo *self)
{
}

GxStateInfo *
gx_state_info_new (void)
{
  return g_object_new (GX_TYPE_STATE_INFO, NULL);
}

GSettings *
gx_state_info_get_settings (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->settings;
}

GHashTable *
gx_state_info_get_main_config (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->main_config;
}

GListModel *
gx_state_info_get_blocklists (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->blocklists;
}

GListModel *
gx_state_info_get_curated_configs (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->curated_configs;
}

GxBackend *
gx_state_info_get_backend (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->backend;
}

GxEntryCacheManager *
gx_state_info_get_cache_manager (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->cache_manager;
}

GxTransactionManager *
gx_state_info_get_transaction_manager (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->transaction_manager;
}

GListModel *
gx_state_info_get_available_updates (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->available_updates;
}

GxApplicationMapFactory *
gx_state_info_get_entry_factory (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->entry_factory;
}

GxApplicationMapFactory *
gx_state_info_get_application_factory (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->application_factory;
}

GListModel *
gx_state_info_get_all_entries (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->all_entries;
}

GListModel *
gx_state_info_get_all_entry_groups (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->all_entry_groups;
}

GListModel *
gx_state_info_get_all_installed_entry_groups (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->all_installed_entry_groups;
}

GxSearchEngine *
gx_state_info_get_search_engine (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->search_engine;
}

GxContentProvider *
gx_state_info_get_curated_provider (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->curated_provider;
}

GxFlathubState *
gx_state_info_get_flathub (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->flathub;
}

gboolean
gx_state_info_get_busy (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), FALSE);
  return self->busy;
}

const char *
gx_state_info_get_busy_step_label (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->busy_step_label;
}

const char *
gx_state_info_get_busy_progress_label (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->busy_progress_label;
}

double
gx_state_info_get_busy_progress (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), 0.0);
  return self->busy_progress;
}

gboolean
gx_state_info_get_online (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), FALSE);
  return self->online;
}

gboolean
gx_state_info_get_checking_for_updates (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), FALSE);
  return self->checking_for_updates;
}

const char *
gx_state_info_get_background_task_label (GxStateInfo *self)
{
  g_return_val_if_fail (GX_IS_STATE_INFO (self), NULL);
  return self->background_task_label;
}

void
gx_state_info_set_settings (GxStateInfo *self,
                            GSettings   *settings)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->settings, g_object_unref);
  if (settings != NULL)
    self->settings = g_object_ref (settings);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_SETTINGS]);
}

void
gx_state_info_set_main_config (GxStateInfo *self,
                               GHashTable  *main_config)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->main_config, g_hash_table_unref);
  if (main_config != NULL)
    self->main_config = g_hash_table_ref (main_config);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_MAIN_CONFIG]);
}

void
gx_state_info_set_blocklists (GxStateInfo *self,
                              GListModel  *blocklists)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->blocklists, g_object_unref);
  if (blocklists != NULL)
    self->blocklists = g_object_ref (blocklists);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BLOCKLISTS]);
}

void
gx_state_info_set_curated_configs (GxStateInfo *self,
                                   GListModel  *curated_configs)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->curated_configs, g_object_unref);
  if (curated_configs != NULL)
    self->curated_configs = g_object_ref (curated_configs);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_CURATED_CONFIGS]);
}

void
gx_state_info_set_backend (GxStateInfo *self,
                           GxBackend   *backend)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->backend, g_object_unref);
  if (backend != NULL)
    self->backend = g_object_ref (backend);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BACKEND]);
}

void
gx_state_info_set_cache_manager (GxStateInfo         *self,
                                 GxEntryCacheManager *cache_manager)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->cache_manager, g_object_unref);
  if (cache_manager != NULL)
    self->cache_manager = g_object_ref (cache_manager);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_CACHE_MANAGER]);
}

void
gx_state_info_set_transaction_manager (GxStateInfo          *self,
                                       GxTransactionManager *transaction_manager)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->transaction_manager, g_object_unref);
  if (transaction_manager != NULL)
    self->transaction_manager = g_object_ref (transaction_manager);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_TRANSACTION_MANAGER]);
}

void
gx_state_info_set_available_updates (GxStateInfo *self,
                                     GListModel  *available_updates)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->available_updates, g_object_unref);
  if (available_updates != NULL)
    self->available_updates = g_object_ref (available_updates);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_AVAILABLE_UPDATES]);
}

void
gx_state_info_set_entry_factory (GxStateInfo             *self,
                                 GxApplicationMapFactory *entry_factory)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->entry_factory, g_object_unref);
  if (entry_factory != NULL)
    self->entry_factory = g_object_ref (entry_factory);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ENTRY_FACTORY]);
}

void
gx_state_info_set_application_factory (GxStateInfo             *self,
                                       GxApplicationMapFactory *application_factory)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->application_factory, g_object_unref);
  if (application_factory != NULL)
    self->application_factory = g_object_ref (application_factory);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_APPLICATION_FACTORY]);
}

void
gx_state_info_set_all_entries (GxStateInfo *self,
                               GListModel  *all_entries)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->all_entries, g_object_unref);
  if (all_entries != NULL)
    self->all_entries = g_object_ref (all_entries);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ALL_ENTRIES]);
}

void
gx_state_info_set_all_entry_groups (GxStateInfo *self,
                                    GListModel  *all_entry_groups)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->all_entry_groups, g_object_unref);
  if (all_entry_groups != NULL)
    self->all_entry_groups = g_object_ref (all_entry_groups);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ALL_ENTRY_GROUPS]);
}

void
gx_state_info_set_all_installed_entry_groups (GxStateInfo *self,
                                              GListModel  *all_installed_entry_groups)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->all_installed_entry_groups, g_object_unref);
  if (all_installed_entry_groups != NULL)
    self->all_installed_entry_groups = g_object_ref (all_installed_entry_groups);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ALL_INSTALLED_ENTRY_GROUPS]);
}

void
gx_state_info_set_search_engine (GxStateInfo    *self,
                                 GxSearchEngine *search_engine)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->search_engine, g_object_unref);
  if (search_engine != NULL)
    self->search_engine = g_object_ref (search_engine);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_SEARCH_ENGINE]);
}

void
gx_state_info_set_curated_provider (GxStateInfo       *self,
                                    GxContentProvider *curated_provider)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->curated_provider, g_object_unref);
  if (curated_provider != NULL)
    self->curated_provider = g_object_ref (curated_provider);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_CURATED_PROVIDER]);
}

void
gx_state_info_set_flathub (GxStateInfo    *self,
                           GxFlathubState *flathub)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->flathub, g_object_unref);
  if (flathub != NULL)
    self->flathub = g_object_ref (flathub);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_FLATHUB]);
}

void
gx_state_info_set_busy (GxStateInfo *self,
                        gboolean     busy)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  self->busy = busy;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BUSY]);
}

void
gx_state_info_set_busy_step_label (GxStateInfo *self,
                                   const char  *busy_step_label)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->busy_step_label, g_free);
  if (busy_step_label != NULL)
    self->busy_step_label = g_strdup (busy_step_label);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BUSY_STEP_LABEL]);
}

void
gx_state_info_set_busy_progress_label (GxStateInfo *self,
                                       const char  *busy_progress_label)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->busy_progress_label, g_free);
  if (busy_progress_label != NULL)
    self->busy_progress_label = g_strdup (busy_progress_label);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BUSY_PROGRESS_LABEL]);
}

void
gx_state_info_set_busy_progress (GxStateInfo *self,
                                 double       busy_progress)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  self->busy_progress = busy_progress;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BUSY_PROGRESS]);
}

void
gx_state_info_set_online (GxStateInfo *self,
                          gboolean     online)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  self->online = online;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ONLINE]);
}

void
gx_state_info_set_checking_for_updates (GxStateInfo *self,
                                        gboolean     checking_for_updates)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  self->checking_for_updates = checking_for_updates;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_CHECKING_FOR_UPDATES]);
}

void
gx_state_info_set_background_task_label (GxStateInfo *self,
                                         const char  *background_task_label)
{
  g_return_if_fail (GX_IS_STATE_INFO (self));

  g_clear_pointer (&self->background_task_label, g_free);
  if (background_task_label != NULL)
    self->background_task_label = g_strdup (background_task_label);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_BACKGROUND_TASK_LABEL]);
}

/* End of bz-state-info.c */
