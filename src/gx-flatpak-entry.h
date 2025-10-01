/* bz-flatpak-entry.h
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
#include "gx-flatpak-instance.h"

G_BEGIN_DECLS

#define GX_TYPE_FLATPAK_ENTRY (gx_flatpak_entry_get_type ())
G_DECLARE_FINAL_TYPE (GxFlatpakEntry, gx_flatpak_entry, GX, FLATPAK_ENTRY, GxEntry)

gboolean
gx_flatpak_entry_is_user (GxFlatpakEntry *self);

const char *
gx_flatpak_entry_get_flatpak_id (GxFlatpakEntry *self);

const char *
gx_flatpak_entry_get_application_name (GxFlatpakEntry *self);

const char *
gx_flatpak_entry_get_runtime_name (GxFlatpakEntry *self);

const char *
gx_flatpak_entry_get_addon_extension_of_ref (GxFlatpakEntry *self);

char *
gx_flatpak_entry_extract_id_from_unique_id (const char *unique_id);

gboolean
gx_flatpak_entry_launch (GxFlatpakEntry    *self,
                         GxFlatpakInstance *flatpak,
                         GError           **error);

G_END_DECLS
