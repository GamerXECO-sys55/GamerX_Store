/* bz-backend-transaction-op-progress-payload.h
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

G_BEGIN_DECLS

#define GX_TYPE_BACKEND_TRANSACTION_OP_PROGRESS_PAYLOAD (gx_backend_transaction_op_progress_payload_get_type ())
G_DECLARE_FINAL_TYPE (GxBackendTransactionOpProgressPayload, gx_backend_transaction_op_progress_payload, GX, BACKEND_TRANSACTION_OP_PROGRESS_PAYLOAD, GObject)

GxBackendTransactionOpProgressPayload *
gx_backend_transaction_op_progress_payload_new (void);

GxBackendTransactionOpPayload *
gx_backend_transaction_op_progress_payload_get_op (GxBackendTransactionOpProgressPayload *self);

const char *
gx_backend_transaction_op_progress_payload_get_status (GxBackendTransactionOpProgressPayload *self);

gboolean
gx_backend_transaction_op_progress_payload_get_is_estimating (GxBackendTransactionOpProgressPayload *self);

double
gx_backend_transaction_op_progress_payload_get_progress (GxBackendTransactionOpProgressPayload *self);

double
gx_backend_transaction_op_progress_payload_get_total_progress (GxBackendTransactionOpProgressPayload *self);

guint64
gx_backend_transaction_op_progress_payload_get_bytes_transferred (GxBackendTransactionOpProgressPayload *self);

guint64
gx_backend_transaction_op_progress_payload_get_start_time (GxBackendTransactionOpProgressPayload *self);

void
gx_backend_transaction_op_progress_payload_set_op (GxBackendTransactionOpProgressPayload *self,
                                                   GxBackendTransactionOpPayload         *op);

void
gx_backend_transaction_op_progress_payload_set_status (GxBackendTransactionOpProgressPayload *self,
                                                       const char                            *status);

void
gx_backend_transaction_op_progress_payload_set_is_estimating (GxBackendTransactionOpProgressPayload *self,
                                                              gboolean                               is_estimating);

void
gx_backend_transaction_op_progress_payload_set_progress (GxBackendTransactionOpProgressPayload *self,
                                                         double                                 progress);

void
gx_backend_transaction_op_progress_payload_set_total_progress (GxBackendTransactionOpProgressPayload *self,
                                                               double                                 total_progress);

void
gx_backend_transaction_op_progress_payload_set_bytes_transferred (GxBackendTransactionOpProgressPayload *self,
                                                                  guint64                                bytes_transferred);

void
gx_backend_transaction_op_progress_payload_set_start_time (GxBackendTransactionOpProgressPayload *self,
                                                           guint64                                start_time);

G_END_DECLS

/* End of bz-backend-transaction-op-progress-payload.h */
