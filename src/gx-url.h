/* bz-url.h
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

G_BEGIN_DECLS

#define GX_TYPE_URL (gx_url_get_type ())
G_DECLARE_FINAL_TYPE (GxUrl, gx_url, GX, URL, GObject)

GxUrl *
gx_url_new (void);

const char *
gx_url_get_name (GxUrl *self);

const char *
gx_url_get_url (GxUrl *self);

void
gx_url_set_name (GxUrl      *self,
                 const char *name);

void
gx_url_set_url (GxUrl      *self,
                const char *url);

G_END_DECLS

/* End of bz-url.h */
