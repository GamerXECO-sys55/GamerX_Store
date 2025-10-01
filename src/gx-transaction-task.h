/* bz-transaction-task.h
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

#include "gx-backend-transaction-op-payload.h"
#include "gx-backend-transaction-op-progress-payload.h"

G_BEGIN_DECLS

#define GX_TYPE_TRANSACTION_TASK (gx_transaction_task_get_type ())
G_DECLARE_FINAL_TYPE (GxTransactionTask, gx_transaction_task, GX, TRANSACTION_TASK, GObject)

GxTransactionTask *
gx_transaction_task_new (void);

GxBackendTransactionOpPayload *
gx_transaction_task_get_op (GxTransactionTask *self);

GxBackendTransactionOpProgressPayload *
gx_transaction_task_get_last_progress (GxTransactionTask *self);

const char *
gx_transaction_task_get_error (GxTransactionTask *self);

void
gx_transaction_task_set_op (GxTransactionTask             *self,
                            GxBackendTransactionOpPayload *op);

void
gx_transaction_task_set_last_progress (GxTransactionTask                     *self,
                                       GxBackendTransactionOpProgressPayload *last_progress);

void
gx_transaction_task_set_error (GxTransactionTask *self,
                               const char        *error);

G_END_DECLS

/* End of bz-transaction-task.h */
