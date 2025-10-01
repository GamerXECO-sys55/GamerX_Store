/* bz-full-view.h
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

#include <adwaita.h>

#include "gx-entry-group.h"
#include "gx-transaction-manager.h"

G_BEGIN_DECLS

#define GX_TYPE_FULL_VIEW (gx_full_view_get_type ())
G_DECLARE_FINAL_TYPE (GxFullView, gx_full_view, GX, FULL_VIEW, AdwBin)

GtkWidget *
gx_full_view_new (void);

void
gx_full_view_set_transaction_manager (GxFullView           *self,
                                      GxTransactionManager *group);

GxTransactionManager *
gx_full_view_get_transaction_manager (GxFullView *self);

void
gx_full_view_set_entry_group (GxFullView   *self,
                              GxEntryGroup *group);

GxEntryGroup *
gx_full_view_get_entry_group (GxFullView *self);

G_END_DECLS
