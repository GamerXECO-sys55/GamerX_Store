/* bz-result.h
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

G_BEGIN_DECLS

#define GX_TYPE_RESULT (gx_result_get_type ())
G_DECLARE_FINAL_TYPE (GxResult, gx_result, GX, RESULT, GObject)

GxResult *
gx_result_new (DexFuture *future);

gboolean
gx_result_get_pending (GxResult *self);

gboolean
gx_result_get_resolved (GxResult *self);

gboolean
gx_result_get_rejected (GxResult *self);

gpointer
gx_result_get_object (GxResult *self);

const char *
gx_result_get_message (GxResult *self);

DexFuture *
gx_result_dup_future (GxResult *self);

G_END_DECLS

/* End of bz-result.h */
