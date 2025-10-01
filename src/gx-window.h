/* bz-window.h
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

#include "gx-state-info.h"

G_BEGIN_DECLS

#define GX_TYPE_WINDOW (gx_window_get_type ())
G_DECLARE_FINAL_TYPE (GxWindow, gx_window, GX, WINDOW, AdwApplicationWindow)

GxWindow *
gx_window_new (GxStateInfo *state);

void
gx_window_search (GxWindow   *self,
                  const char *text);

void
gx_window_toggle_transactions (GxWindow *self);

void
gx_window_push_update_dialog (GxWindow *self);

void
gx_window_show_entry (GxWindow *self,
                      GxEntry  *entry);

void
gx_window_show_group (GxWindow     *self,
                      GxEntryGroup *group);

void
gx_window_set_category_view_mode (GxWindow *self,
                                  gboolean  enabled);

void
gx_window_add_toast (GxWindow *self,
                     AdwToast *toast);

GxStateInfo *
gx_window_get_state_info (GxWindow *self);

G_END_DECLS
