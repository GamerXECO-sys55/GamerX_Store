/* bz-transaction-task.c
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

#include "gx-transaction-task.h"

struct _GxTransactionTask
{
  GObject parent_instance;

  GxBackendTransactionOpPayload         *op;
  GxBackendTransactionOpProgressPayload *last_progress;
  char                                  *error;
};

G_DEFINE_FINAL_TYPE (GxTransactionTask, gx_transaction_task, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_OP,
  PROP_LAST_PROGRESS,
  PROP_ERROR,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_transaction_task_dispose (GObject *object)
{
  GxTransactionTask *self = GX_TRANSACTION_TASK (object);

  g_clear_pointer (&self->op, g_object_unref);
  g_clear_pointer (&self->last_progress, g_object_unref);
  g_clear_pointer (&self->error, g_free);

  G_OBJECT_CLASS (gx_transaction_task_parent_class)->dispose (object);
}

static void
gx_transaction_task_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  GxTransactionTask *self = GX_TRANSACTION_TASK (object);

  switch (prop_id)
    {
    case PROP_OP:
      g_value_set_object (value, gx_transaction_task_get_op (self));
      break;
    case PROP_LAST_PROGRESS:
      g_value_set_object (value, gx_transaction_task_get_last_progress (self));
      break;
    case PROP_ERROR:
      g_value_set_string (value, gx_transaction_task_get_error (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_transaction_task_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  GxTransactionTask *self = GX_TRANSACTION_TASK (object);

  switch (prop_id)
    {
    case PROP_OP:
      gx_transaction_task_set_op (self, g_value_get_object (value));
      break;
    case PROP_LAST_PROGRESS:
      gx_transaction_task_set_last_progress (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_transaction_task_class_init (GxTransactionTaskClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_transaction_task_set_property;
  object_class->get_property = gx_transaction_task_get_property;
  object_class->dispose      = gx_transaction_task_dispose;

  props[PROP_OP] =
      g_param_spec_object (
          "op",
          NULL, NULL,
          GX_TYPE_BACKEND_TRANSACTION_OP_PAYLOAD,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_LAST_PROGRESS] =
      g_param_spec_object (
          "last-progress",
          NULL, NULL,
          GX_TYPE_BACKEND_TRANSACTION_OP_PROGRESS_PAYLOAD,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ERROR] =
      g_param_spec_string (
          "error",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_transaction_task_init (GxTransactionTask *self)
{
}

GxTransactionTask *
gx_transaction_task_new (void)
{
  return g_object_new (GX_TYPE_TRANSACTION_TASK, NULL);
}

GxBackendTransactionOpPayload *
gx_transaction_task_get_op (GxTransactionTask *self)
{
  g_return_val_if_fail (GX_IS_TRANSACTION_TASK (self), NULL);
  return self->op;
}

GxBackendTransactionOpProgressPayload *
gx_transaction_task_get_last_progress (GxTransactionTask *self)
{
  g_return_val_if_fail (GX_IS_TRANSACTION_TASK (self), NULL);
  return self->last_progress;
}

const char *
gx_transaction_task_get_error (GxTransactionTask *self)
{
  g_return_val_if_fail (GX_IS_TRANSACTION_TASK (self), NULL);
  return self->error;
}

void
gx_transaction_task_set_op (GxTransactionTask             *self,
                            GxBackendTransactionOpPayload *op)
{
  g_return_if_fail (GX_IS_TRANSACTION_TASK (self));

  g_clear_pointer (&self->op, g_object_unref);
  if (op != NULL)
    self->op = g_object_ref (op);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_OP]);
}

void
gx_transaction_task_set_last_progress (GxTransactionTask                     *self,
                                       GxBackendTransactionOpProgressPayload *last_progress)
{
  g_return_if_fail (GX_IS_TRANSACTION_TASK (self));

  g_clear_pointer (&self->last_progress, g_object_unref);
  if (last_progress != NULL)
    self->last_progress = g_object_ref (last_progress);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_LAST_PROGRESS]);
}

void
gx_transaction_task_set_error (GxTransactionTask *self,
                               const char        *error)
{
  g_return_if_fail (GX_IS_TRANSACTION_TASK (self));

  g_clear_pointer (&self->error, g_object_unref);
  if (error != NULL)
    self->error = g_strdup (error);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ERROR]);
}

/* End of bz-transaction-task.c */
