/* bz-backend-transaction-op-payload.c
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

#include "gx-backend-transaction-op-payload.h"

struct _GxBackendTransactionOpPayload
{
  GObject parent_instance;

  char    *name;
  GxEntry *entry;
  guint64  download_size;
  guint64  installed_size;
};

G_DEFINE_FINAL_TYPE (GxBackendTransactionOpPayload, gx_backend_transaction_op_payload, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_NAME,
  PROP_ENTRY,
  PROP_DOWNLOAD_SIZE,
  PROP_INSTALLED_SIZE,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_backend_transaction_op_payload_dispose (GObject *object)
{
  GxBackendTransactionOpPayload *self = GX_BACKEND_TRANSACTION_OP_PAYLOAD (object);

  g_clear_pointer (&self->name, g_free);
  g_clear_pointer (&self->entry, g_object_unref);

  G_OBJECT_CLASS (gx_backend_transaction_op_payload_parent_class)->dispose (object);
}

static void
gx_backend_transaction_op_payload_get_property (GObject    *object,
                                                guint       prop_id,
                                                GValue     *value,
                                                GParamSpec *pspec)
{
  GxBackendTransactionOpPayload *self = GX_BACKEND_TRANSACTION_OP_PAYLOAD (object);

  switch (prop_id)
    {
    case PROP_NAME:
      g_value_set_string (value, gx_backend_transaction_op_payload_get_name (self));
      break;
    case PROP_ENTRY:
      g_value_set_object (value, gx_backend_transaction_op_payload_get_entry (self));
      break;
    case PROP_DOWNLOAD_SIZE:
      g_value_set_uint64 (value, gx_backend_transaction_op_payload_get_download_size (self));
      break;
    case PROP_INSTALLED_SIZE:
      g_value_set_uint64 (value, gx_backend_transaction_op_payload_get_installed_size (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_backend_transaction_op_payload_set_property (GObject      *object,
                                                guint         prop_id,
                                                const GValue *value,
                                                GParamSpec   *pspec)
{
  GxBackendTransactionOpPayload *self = GX_BACKEND_TRANSACTION_OP_PAYLOAD (object);

  switch (prop_id)
    {
    case PROP_NAME:
      gx_backend_transaction_op_payload_set_name (self, g_value_get_string (value));
      break;
    case PROP_ENTRY:
      gx_backend_transaction_op_payload_set_entry (self, g_value_get_object (value));
      break;
    case PROP_DOWNLOAD_SIZE:
      gx_backend_transaction_op_payload_set_download_size (self, g_value_get_uint64 (value));
      break;
    case PROP_INSTALLED_SIZE:
      gx_backend_transaction_op_payload_set_installed_size (self, g_value_get_uint64 (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_backend_transaction_op_payload_class_init (GxBackendTransactionOpPayloadClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_backend_transaction_op_payload_set_property;
  object_class->get_property = gx_backend_transaction_op_payload_get_property;
  object_class->dispose      = gx_backend_transaction_op_payload_dispose;

  props[PROP_NAME] =
      g_param_spec_string (
          "name",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ENTRY] =
      g_param_spec_object (
          "entry",
          NULL, NULL,
          GX_TYPE_ENTRY,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_DOWNLOAD_SIZE] =
      g_param_spec_uint64 (
          "download-size",
          NULL, NULL,
          0, G_MAXUINT64, (guint64) 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_INSTALLED_SIZE] =
      g_param_spec_uint64 (
          "installed-size",
          NULL, NULL,
          0, G_MAXUINT64, (guint64) 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_backend_transaction_op_payload_init (GxBackendTransactionOpPayload *self)
{
}

GxBackendTransactionOpPayload *
gx_backend_transaction_op_payload_new (void)
{
  return g_object_new (GX_TYPE_BACKEND_TRANSACTION_OP_PAYLOAD, NULL);
}

const char *
gx_backend_transaction_op_payload_get_name (GxBackendTransactionOpPayload *self)
{
  g_return_val_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self), NULL);
  return self->name;
}

GxEntry *
gx_backend_transaction_op_payload_get_entry (GxBackendTransactionOpPayload *self)
{
  g_return_val_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self), NULL);
  return self->entry;
}

guint64
gx_backend_transaction_op_payload_get_download_size (GxBackendTransactionOpPayload *self)
{
  g_return_val_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self), 0);
  return self->download_size;
}

guint64
gx_backend_transaction_op_payload_get_installed_size (GxBackendTransactionOpPayload *self)
{
  g_return_val_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self), 0);
  return self->installed_size;
}

void
gx_backend_transaction_op_payload_set_name (GxBackendTransactionOpPayload *self,
                                            const char                    *name)
{
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self));

  g_clear_pointer (&self->name, g_free);
  if (name != NULL)
    self->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_NAME]);
}

void
gx_backend_transaction_op_payload_set_entry (GxBackendTransactionOpPayload *self,
                                             GxEntry                       *entry)
{
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self));

  g_clear_pointer (&self->entry, g_object_unref);
  if (entry != NULL)
    self->entry = g_object_ref (entry);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ENTRY]);
}

void
gx_backend_transaction_op_payload_set_download_size (GxBackendTransactionOpPayload *self,
                                                     guint64                        download_size)
{
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self));

  self->download_size = download_size;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_DOWNLOAD_SIZE]);
}

void
gx_backend_transaction_op_payload_set_installed_size (GxBackendTransactionOpPayload *self,
                                                      guint64                        installed_size)
{
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (self));

  self->installed_size = installed_size;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_INSTALLED_SIZE]);
}

/* End of bz-backend-transaction-op-payload.c */
