/* bz-content-provider.h
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

G_BEGIN_DECLS

#define GX_CONTENT_YAML_ERROR (gx_content_yaml_error_quark ())
GQuark gx_content_yaml_error_quark (void);

typedef enum
{
  GX_CONTENT_YAML_ERROR_INVALID_YAML = 0,
  GX_CONTENT_YAML_ERROR_INVALID_STRUCTURE,
} GxContentYamlError;

#define GX_TYPE_CONTENT_PROVIDER (gx_content_provider_get_type ())
G_DECLARE_FINAL_TYPE (GxContentProvider, gx_content_provider, GX, CONTENT_PROVIDER, GObject)

GxContentProvider *
gx_content_provider_new (void);

void
gx_content_provider_set_input_files (GxContentProvider *self,
                                     GListModel        *input_files);

GListModel *
gx_content_provider_get_input_files (GxContentProvider *self);

void
gx_content_provider_set_factory (GxContentProvider       *self,
                                 GxApplicationMapFactory *factory);

GxApplicationMapFactory *
gx_content_provider_get_factory (GxContentProvider *self);

gboolean
gx_content_provider_get_has_inputs (GxContentProvider *self);

G_END_DECLS
