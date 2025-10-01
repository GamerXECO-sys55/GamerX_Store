/* bz-transaction.h
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

#include "gx-entry.h"
#include "gx-transaction-task.h"

G_BEGIN_DECLS

#define GX_TYPE_TRANSACTION (gx_transaction_get_type ())
G_DECLARE_DERIVABLE_TYPE (GxTransaction, gx_transaction, GX, TRANSACTION, GObject)

struct _GxTransactionClass
{
  GObjectClass parent_class;
};

GxTransaction *
gx_transaction_new_full (GxEntry **installs,
                         guint     n_installs,
                         GxEntry **updates,
                         guint     n_updates,
                         GxEntry **removals,
                         guint     n_removals);

GxTransaction *
gx_transaction_new_merged (GxTransaction **transactions,
                           guint           n_transactions);

GListModel *
gx_transaction_get_installs (GxTransaction *self);

GListModel *
gx_transaction_get_updates (GxTransaction *self);

GListModel *
gx_transaction_get_removals (GxTransaction *self);

void
gx_transaction_hold (GxTransaction *self);

void
gx_transaction_release (GxTransaction *self);

static inline void
gx_transaction_dismiss (GxTransaction *self)
{
  gx_transaction_release (self);
  g_object_unref (self);
}

void
gx_transaction_add_task (GxTransaction                 *self,
                         GxBackendTransactionOpPayload *payload);

void
gx_transaction_update_task (GxTransaction                         *self,
                            GxBackendTransactionOpProgressPayload *payload);

void
gx_transaction_finish_task (GxTransaction                 *self,
                            GxBackendTransactionOpPayload *payload);

void
gx_transaction_error_out_task (GxTransaction                 *self,
                               GxBackendTransactionOpPayload *payload,
                               const char                    *message);

G_END_DECLS
