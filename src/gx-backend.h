/* bz-backend.h
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

#include <libdex.h>

#include "gx-entry.h"

G_BEGIN_DECLS

#define GX_TYPE_BACKEND (gx_backend_get_type ())
G_DECLARE_INTERFACE (GxBackend, gx_backend, GX, BACKEND, GObject)

struct _GxBackendInterface
{
  GTypeInterface parent_iface;

  DexChannel *(*create_notification_channel) (GxBackend *self);

  /* DexFuture* -> char*|GxEntry* */
  DexFuture *(*load_local_package) (GxBackend    *self,
                                    GFile        *file,
                                    GCancellable *cancellable);

  /* DexFuture* -> gboolean */
  DexFuture *(*retrieve_remote_entries) (GxBackend     *self,
                                         DexChannel    *channel,
                                         GPtrArray     *blocked_names,
                                         GCancellable  *cancellable,
                                         gpointer       user_data,
                                         GDestroyNotify destroy_user_data);

  /* DexFuture* -> GHashTable* */
  DexFuture *(*retrieve_install_ids) (GxBackend    *self,
                                      GCancellable *cancellable);

  /* DexFuture* -> GPtrArray* -> char* */
  DexFuture *(*retrieve_update_ids) (GxBackend    *self,
                                     GCancellable *cancellable);

  /* DexFuture* -> gboolean */
  DexFuture *(*schedule_transaction) (GxBackend    *self,
                                      GxEntry     **installs,
                                      guint         n_installs,
                                      GxEntry     **updates,
                                      guint         n_updates,
                                      GxEntry     **removals,
                                      guint         n_removals,
                                      DexChannel   *channel,
                                      GCancellable *cancellable);
};

DexChannel *
gx_backend_create_notification_channel (GxBackend *self);

DexFuture *
gx_backend_load_local_package (GxBackend    *self,
                               GFile        *file,
                               GCancellable *cancellable);

DexFuture *
gx_backend_retrieve_remote_entries (GxBackend     *self,
                                    DexChannel    *channel,
                                    GPtrArray     *blocked_names,
                                    GCancellable  *cancellable,
                                    gpointer       user_data,
                                    GDestroyNotify destroy_user_data);

DexFuture *
gx_backend_retrieve_remote_entries_with_blocklists (GxBackend     *self,
                                                    DexChannel    *channel,
                                                    GListModel    *blocklists,
                                                    GCancellable  *cancellable,
                                                    gpointer       user_data,
                                                    GDestroyNotify destroy_user_data);

DexFuture *
gx_backend_retrieve_install_ids (GxBackend    *self,
                                 GCancellable *cancellable);

DexFuture *
gx_backend_retrieve_update_ids (GxBackend    *self,
                                GCancellable *cancellable);

DexFuture *
gx_backend_schedule_transaction (GxBackend    *self,
                                 GxEntry     **installs,
                                 guint         n_installs,
                                 GxEntry     **updates,
                                 guint         n_updates,
                                 GxEntry     **removals,
                                 guint         n_removals,
                                 DexChannel   *channel,
                                 GCancellable *cancellable);

DexFuture *
gx_backend_merge_and_schedule_transactions (GxBackend    *self,
                                            GListModel   *transactions,
                                            DexChannel   *channel,
                                            GCancellable *cancellable);

G_END_DECLS
