/* bz-backend-transaction-op-payload.h
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

#include <gtk/gtk.h>

#include "gx-entry.h"

G_BEGIN_DECLS

#define GX_TYPE_BACKEND_TRANSACTION_OP_PAYLOAD (gx_backend_transaction_op_payload_get_type ())
G_DECLARE_FINAL_TYPE (GxBackendTransactionOpPayload, gx_backend_transaction_op_payload, GX, BACKEND_TRANSACTION_OP_PAYLOAD, GObject)

GxBackendTransactionOpPayload *
gx_backend_transaction_op_payload_new (void);

const char *
gx_backend_transaction_op_payload_get_name (GxBackendTransactionOpPayload *self);

GxEntry *
gx_backend_transaction_op_payload_get_entry (GxBackendTransactionOpPayload *self);

guint64
gx_backend_transaction_op_payload_get_download_size (GxBackendTransactionOpPayload *self);

guint64
gx_backend_transaction_op_payload_get_installed_size (GxBackendTransactionOpPayload *self);

void
gx_backend_transaction_op_payload_set_name (GxBackendTransactionOpPayload *self,
                                            const char                    *name);

void
gx_backend_transaction_op_payload_set_entry (GxBackendTransactionOpPayload *self,
                                             GxEntry                       *entry);

void
gx_backend_transaction_op_payload_set_download_size (GxBackendTransactionOpPayload *self,
                                                     guint64                        download_size);

void
gx_backend_transaction_op_payload_set_installed_size (GxBackendTransactionOpPayload *self,
                                                      guint64                        installed_size);

G_END_DECLS

/* End of bz-backend-transaction-op-payload.h */
