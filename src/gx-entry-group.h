/* bz-entry-group.h
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

#include "gx-application-map-factory.h"
#include "gx-entry.h"
#include "gx-result.h"

G_BEGIN_DECLS

#define GX_TYPE_ENTRY_GROUP (gx_entry_group_get_type ())
G_DECLARE_FINAL_TYPE (GxEntryGroup, gx_entry_group, GX, ENTRY_GROUP, GObject)

GxEntryGroup *
gx_entry_group_new (GxApplicationMapFactory *factory);

GListModel *
gx_entry_group_get_model (GxEntryGroup *self);

const char *
gx_entry_group_get_id (GxEntryGroup *self);

const char *
gx_entry_group_get_title (GxEntryGroup *self);

const char *
gx_entry_group_get_developer (GxEntryGroup *self);

const char *
gx_entry_group_get_description (GxEntryGroup *self);

GdkPaintable *
gx_entry_group_get_icon_paintable (GxEntryGroup *self);

GIcon *
gx_entry_group_get_mini_icon (GxEntryGroup *self);

gboolean
gx_entry_group_get_is_floss (GxEntryGroup *self);

const char *
gx_entry_group_get_light_accent_color (GxEntryGroup *self);

const char *
gx_entry_group_get_dark_accent_color (GxEntryGroup *self);

gboolean
gx_entry_group_get_is_flathub (GxEntryGroup *self);

GPtrArray *
gx_entry_group_get_search_tokens (GxEntryGroup *self);

GxResult *
gx_entry_group_dup_ui_entry (GxEntryGroup *self);

char *
gx_entry_group_dup_ui_entry_id (GxEntryGroup *self);

int
gx_entry_group_get_installable (GxEntryGroup *self);

int
gx_entry_group_get_updatable (GxEntryGroup *self);

int
gx_entry_group_get_removable (GxEntryGroup *self);

int
gx_entry_group_get_installable_and_available (GxEntryGroup *self);

int
gx_entry_group_get_updatable_and_available (GxEntryGroup *self);

int
gx_entry_group_get_removable_and_available (GxEntryGroup *self);

void
gx_entry_group_add (GxEntryGroup *self,
                    GxEntry      *entry);

void
gx_entry_group_connect_living (GxEntryGroup *self,
                               GxEntry      *entry);

DexFuture *
gx_entry_group_dup_all_into_model (GxEntryGroup *self);

G_END_DECLS
