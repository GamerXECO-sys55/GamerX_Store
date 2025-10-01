/* bz-state-info.h
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

#include "gx-application-map-factory.h"
#include "gx-backend.h"
#include "gx-content-provider.h"
#include "gx-entry-cache-manager.h"
#include "gx-flathub-state.h"
#include "gx-search-engine.h"
#include "gx-transaction-manager.h"

G_BEGIN_DECLS

#define GX_TYPE_STATE_INFO (gx_state_info_get_type ())
G_DECLARE_FINAL_TYPE (GxStateInfo, gx_state_info, GX, STATE_INFO, GObject)

GxStateInfo *
gx_state_info_new (void);

GSettings *
gx_state_info_get_settings (GxStateInfo *self);

GHashTable *
gx_state_info_get_main_config (GxStateInfo *self);

GListModel *
gx_state_info_get_blocklists (GxStateInfo *self);

GListModel *
gx_state_info_get_curated_configs (GxStateInfo *self);

GxBackend *
gx_state_info_get_backend (GxStateInfo *self);

GxEntryCacheManager *
gx_state_info_get_cache_manager (GxStateInfo *self);

GxTransactionManager *
gx_state_info_get_transaction_manager (GxStateInfo *self);

GListModel *
gx_state_info_get_available_updates (GxStateInfo *self);

GxApplicationMapFactory *
gx_state_info_get_entry_factory (GxStateInfo *self);

GxApplicationMapFactory *
gx_state_info_get_application_factory (GxStateInfo *self);

GListModel *
gx_state_info_get_all_entries (GxStateInfo *self);

GListModel *
gx_state_info_get_all_entry_groups (GxStateInfo *self);

GListModel *
gx_state_info_get_all_installed_entry_groups (GxStateInfo *self);

GxSearchEngine *
gx_state_info_get_search_engine (GxStateInfo *self);

GxContentProvider *
gx_state_info_get_curated_provider (GxStateInfo *self);

GxFlathubState *
gx_state_info_get_flathub (GxStateInfo *self);

gboolean
gx_state_info_get_busy (GxStateInfo *self);

const char *
gx_state_info_get_busy_step_label (GxStateInfo *self);

const char *
gx_state_info_get_busy_progress_label (GxStateInfo *self);

double
gx_state_info_get_busy_progress (GxStateInfo *self);

gboolean
gx_state_info_get_online (GxStateInfo *self);

gboolean
gx_state_info_get_checking_for_updates (GxStateInfo *self);

const char *
gx_state_info_get_background_task_label (GxStateInfo *self);

void
gx_state_info_set_settings (GxStateInfo *self,
                            GSettings   *settings);

void
gx_state_info_set_main_config (GxStateInfo *self,
                               GHashTable  *main_config);

void
gx_state_info_set_blocklists (GxStateInfo *self,
                              GListModel  *blocklists);

void
gx_state_info_set_curated_configs (GxStateInfo *self,
                                   GListModel  *curated_configs);

void
gx_state_info_set_backend (GxStateInfo *self,
                           GxBackend   *backend);

void
gx_state_info_set_cache_manager (GxStateInfo         *self,
                                 GxEntryCacheManager *cache_manager);

void
gx_state_info_set_transaction_manager (GxStateInfo          *self,
                                       GxTransactionManager *transaction_manager);

void
gx_state_info_set_available_updates (GxStateInfo *self,
                                     GListModel  *available_updates);

void
gx_state_info_set_entry_factory (GxStateInfo             *self,
                                 GxApplicationMapFactory *entry_factory);

void
gx_state_info_set_application_factory (GxStateInfo             *self,
                                       GxApplicationMapFactory *application_factory);

void
gx_state_info_set_all_entries (GxStateInfo *self,
                               GListModel  *all_entries);

void
gx_state_info_set_all_entry_groups (GxStateInfo *self,
                                    GListModel  *all_entry_groups);

void
gx_state_info_set_all_installed_entry_groups (GxStateInfo *self,
                                              GListModel  *all_installed_entry_groups);

void
gx_state_info_set_search_engine (GxStateInfo    *self,
                                 GxSearchEngine *search_engine);

void
gx_state_info_set_curated_provider (GxStateInfo       *self,
                                    GxContentProvider *curated_provider);

void
gx_state_info_set_flathub (GxStateInfo    *self,
                           GxFlathubState *flathub);

void
gx_state_info_set_busy (GxStateInfo *self,
                        gboolean     busy);

void
gx_state_info_set_busy_step_label (GxStateInfo *self,
                                   const char  *busy_step_label);

void
gx_state_info_set_busy_progress_label (GxStateInfo *self,
                                       const char  *busy_progress_label);

void
gx_state_info_set_busy_progress (GxStateInfo *self,
                                 double       busy_progress);

void
gx_state_info_set_online (GxStateInfo *self,
                          gboolean     online);

void
gx_state_info_set_checking_for_updates (GxStateInfo *self,
                                        gboolean     checking_for_updates);

void
gx_state_info_set_background_task_label (GxStateInfo *self,
                                         const char  *background_task_label);

G_END_DECLS

/* End of bz-state-info.h */
