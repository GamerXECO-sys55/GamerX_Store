/* bz-serializable.c
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

#include "gx-serializable.h"

G_DEFINE_INTERFACE (GxSerializable, gx_serializable, G_TYPE_OBJECT)

static void
gx_serializable_real_serialize (GxSerializable  *self,
                                GVariantBuilder *builder)
{
  return;
}

static gboolean
gx_serializable_real_deserialize (GxSerializable *self,
                                  GVariant       *import,
                                  GError        **error)
{
  return TRUE;
}

static void
gx_serializable_default_init (GxSerializableInterface *iface)
{
  iface->serialize   = gx_serializable_real_serialize;
  iface->deserialize = gx_serializable_real_deserialize;
}

void
gx_serializable_serialize (GxSerializable  *self,
                           GVariantBuilder *builder)
{
  g_return_if_fail (GX_IS_SERIALIZABLE (self));
  g_return_if_fail (builder != NULL);

  GX_SERIALIZABLE_GET_IFACE (self)->serialize (
      self,
      builder);
}

gboolean
gx_serializable_deserialize (GxSerializable *self,
                             GVariant       *import,
                             GError        **error)
{
  g_return_val_if_fail (GX_IS_SERIALIZABLE (self), FALSE);
  g_return_val_if_fail (import != NULL, FALSE);

  return GX_SERIALIZABLE_GET_IFACE (self)->deserialize (
      self,
      import,
      error);
}
