/* bz-serializable.h
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

#include <glib-object.h>

G_BEGIN_DECLS

#define GX_TYPE_SERIALIZABLE (gx_serializable_get_type ())
G_DECLARE_INTERFACE (GxSerializable, gx_serializable, GX, SERIALIZABLE, GObject)

struct _GxSerializableInterface
{
  GTypeInterface parent_iface;

  void (*serialize) (GxSerializable  *self,
                     GVariantBuilder *builder);

  gboolean (*deserialize) (GxSerializable *self,
                           GVariant       *import,
                           GError        **error);
};

void
gx_serializable_serialize (GxSerializable  *self,
                           GVariantBuilder *builder);

gboolean
gx_serializable_deserialize (GxSerializable *self,
                             GVariant       *import,
                             GError        **error);

G_END_DECLS
