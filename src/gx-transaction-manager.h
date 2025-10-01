/* bz-transaction-manager.h
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

#include "gx-backend.h"
#include "gx-transaction.h"

G_BEGIN_DECLS

#define GX_TRANSACTION_MGR_ERROR (gx_transaction_mgr_error_quark ())
GQuark gx_transaction_mgr_error_quark (void);

typedef enum
{
  GX_TRANSACTION_MGR_ERROR_CANCELLED_BY_HOOK = 0,
} GxTransaction_MgrError;

#define GX_TYPE_TRANSACTION_MANAGER (gx_transaction_manager_get_type ())
G_DECLARE_FINAL_TYPE (GxTransactionManager, gx_transaction_manager, GX, TRANSACTION_MANAGER, GObject)

GxTransactionManager *
gx_transaction_manager_new (void);

void
gx_transaction_manager_set_config (GxTransactionManager *self,
                                   GHashTable           *config);

GHashTable *
gx_transaction_manager_get_config (GxTransactionManager *self);

void
gx_transaction_manager_set_backend (GxTransactionManager *self,
                                    GxBackend            *backend);

GxBackend *
gx_transaction_manager_get_backend (GxTransactionManager *self);

void
gx_transaction_manager_set_paused (GxTransactionManager *self,
                                   gboolean              paused);

gboolean
gx_transaction_manager_get_paused (GxTransactionManager *self);

gboolean
gx_transaction_manager_get_active (GxTransactionManager *self);

gboolean
gx_transaction_manager_get_has_transactions (GxTransactionManager *self);

void
gx_transaction_manager_add (GxTransactionManager *self,
                            GxTransaction        *transaction);

DexFuture *
gx_transaction_manager_cancel_current (GxTransactionManager *self);

void
gx_transaction_manager_clear_finished (GxTransactionManager *self);

G_END_DECLS
