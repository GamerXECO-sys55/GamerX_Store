/* bz-yaml-parser.h
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

#include <gio/gio.h>

G_BEGIN_DECLS

#define GX_YAML_ERROR (gx_yaml_error_quark ())
GQuark gx_yaml_error_quark (void);

typedef enum
{
  GX_YAML_ERROR_INVALID_YAML = 0,
  GX_YAML_ERROR_DOES_NOT_CONFORM,
  GX_YAML_ERROR_BAD_SCALAR,
} GxYamlError;

#define GX_TYPE_YAML_PARSER (gx_yaml_parser_get_type ())
G_DECLARE_FINAL_TYPE (GxYamlParser, gx_yaml_parser, GX, YAML_PARSER, GObject)

GxYamlParser *
gx_yaml_parser_new_for_resource_schema (const char *path);

GHashTable *
gx_yaml_parser_process_bytes (GxYamlParser *self,
                              GBytes       *bytes,
                              GError      **error);

G_END_DECLS
