/* bz-gnome-shell-search-provider.h
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

#include "gx-search-engine.h"

G_BEGIN_DECLS

#define GX_TYPE_GNOME_SHELL_SEARCH_PROVIDER (gx_gnome_shell_search_provider_get_type ())
G_DECLARE_FINAL_TYPE (GxGnomeShellSearchProvider, gx_gnome_shell_search_provider, GX, GNOME_SHELL_SEARCH_PROVIDER, GObject)

GxGnomeShellSearchProvider *
gx_gnome_shell_search_provider_new (void);

GxSearchEngine *
gx_gnome_shell_search_provider_get_engine (GxGnomeShellSearchProvider *self);

void
gx_gnome_shell_search_provider_set_engine (GxGnomeShellSearchProvider *self,
                                           GxSearchEngine             *engine);

GDBusConnection *
gx_gnome_shell_search_provider_get_connection (GxGnomeShellSearchProvider *self);

gboolean
gx_gnome_shell_search_provider_set_connection (GxGnomeShellSearchProvider *self,
                                               GDBusConnection            *connection,
                                               GError                    **error);

G_END_DECLS

/* End of bz-gnome-shell-search-provider.h */
