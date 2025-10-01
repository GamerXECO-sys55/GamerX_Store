/* bz-flatpak-private.h
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

#include <appstream.h>
#include <flatpak.h>

#include "gx-flatpak-entry.h"
#include "gx-flatpak-instance.h"

G_BEGIN_DECLS

/* GxFlatpakInstance */

FlatpakInstallation *
gx_flatpak_instance_get_system_installation (GxFlatpakInstance *self);

FlatpakInstallation *
gx_flatpak_instance_get_user_installation (GxFlatpakInstance *self);

/* GxFlatpakEntry */

char *
gx_flatpak_ref_format_unique (FlatpakRef *ref,
                              gboolean    user);

GxFlatpakEntry *
gx_flatpak_entry_new_for_ref (GxFlatpakInstance *instance,
                              gboolean           user,
                              FlatpakRemote     *remote,
                              FlatpakRef        *ref,
                              AsComponent       *component,
                              const char        *appstream_dir,
                              GdkPaintable      *remote_icon,
                              GError           **error);

FlatpakRef *
gx_flatpak_entry_get_ref (GxFlatpakEntry *self);

G_END_DECLS
