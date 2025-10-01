/* bz-flatpak-instance.h
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

#define GX_FLATPAK_ERROR (gx_flatpak_error_quark ())
GQuark gx_flatpak_error_quark (void);

typedef enum
{
  GX_FLATPAK_ERROR_CANNOT_INITIALIZE = 0,
  GX_FLATPAK_ERROR_LOCAL_SYNCHRONIZATION_FAILURE,
  GX_FLATPAK_ERROR_REMOTE_SYNCHRONIZATION_FAILURE,
  GX_FLATPAK_ERROR_TRANSACTION_FAILURE,
  GX_FLATPAK_ERROR_IO_MISBEHAVIOR,
  GX_FLATPAK_ERROR_APPSTREAM_FAILURE,
  GX_FLATPAK_ERROR_GLYCIN_FAILURE,
} GxFlatpakError;

#define GX_TYPE_FLATPAK_INSTANCE (gx_flatpak_instance_get_type ())
G_DECLARE_FINAL_TYPE (GxFlatpakInstance, gx_flatpak_instance, GX, FLATPAK_INSTANCE, GObject)

DexFuture *
gx_flatpak_instance_new (void);

DexFuture *
gx_flatpak_instance_has_flathub (GxFlatpakInstance *self,
                                 GCancellable      *cancellable);

DexFuture *
gx_flatpak_instance_ensure_has_flathub (GxFlatpakInstance *self,
                                        GCancellable      *cancellable);

G_END_DECLS
