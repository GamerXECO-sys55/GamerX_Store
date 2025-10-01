/* bz-browse-widget.h
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

#include "gx-content-provider.h"

G_BEGIN_DECLS

#define GX_TYPE_BROWSE_WIDGET (gx_browse_widget_get_type ())
G_DECLARE_FINAL_TYPE (GxBrowseWidget, gx_browse_widget, GX, BROWSE_WIDGET, AdwBin)

GtkWidget *
gx_browse_widget_new (void);

void
gx_browse_widget_set_content_provider (GxBrowseWidget    *self,
                                       GxContentProvider *provider);

GxContentProvider *
gx_browse_widget_get_content_provider (GxBrowseWidget *self);

G_END_DECLS
