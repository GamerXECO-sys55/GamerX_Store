/* bz-flathub-state.h
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

#include "gx-application-map-factory.h"
#include "gx-entry-group.h"

G_BEGIN_DECLS

#define GX_TYPE_FLATHUB_STATE (gx_flathub_state_get_type ())
G_DECLARE_FINAL_TYPE (GxFlathubState, gx_flathub_state, GX, FLATHUB_STATE, GObject)

GxFlathubState *
gx_flathub_state_new (void);

void
gx_flathub_state_set_for_day (GxFlathubState *self,
                              const char     *for_day);

void
gx_flathub_state_set_map_factory (GxFlathubState          *self,
                                  GxApplicationMapFactory *map_factory);

const char *
gx_flathub_state_get_for_day (GxFlathubState *self);

GxApplicationMapFactory *
gx_flathub_state_get_map_factory (GxFlathubState *self);

const char *
gx_flathub_state_get_app_of_the_day (GxFlathubState *self);

GxEntryGroup *
gx_flathub_state_dup_app_of_the_day_group (GxFlathubState *self);

GListModel *
gx_flathub_state_dup_apps_of_the_week (GxFlathubState *self);

GListModel *
gx_flathub_state_get_categories (GxFlathubState *self);

GListModel *
gx_flathub_state_dup_recently_updated (GxFlathubState *self);

GListModel *
gx_flathub_state_dup_recently_added (GxFlathubState *self);

GListModel *
gx_flathub_state_dup_popular (GxFlathubState *self);

GListModel *
gx_flathub_state_dup_trending (GxFlathubState *self);

void
gx_flathub_state_update_to_today (GxFlathubState *self);

G_END_DECLS

/* End of bz-flathub-state.h */
