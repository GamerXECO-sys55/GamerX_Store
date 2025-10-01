/* bz-transaction.c
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

#include "config.h"

#include <glib/gi18n.h>

#include "gx-transaction.h"

typedef struct
{
  GListStore *installs;
  GListStore *updates;
  GListStore *removals;

  char       *name;
  gboolean    pending;
  GListStore *current_ops;
  GListStore *finished_ops;
  char       *status;
  double      progress;
  gboolean    finished;
  gboolean    success;
  char       *error;
} GxTransactionPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GxTransaction, gx_transaction, G_TYPE_OBJECT)

enum
{
  PROP_0,

  PROP_NAME,
  PROP_INSTALLS,
  PROP_UPDATES,
  PROP_REMOVALS,
  PROP_PENDING,
  PROP_CURRENT_OPS,
  PROP_FINISHED_OPS,
  PROP_STATUS,
  PROP_PROGRESS,
  PROP_FINISHED,
  PROP_SUCCESS,
  PROP_ERROR,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static gboolean
find_payload_eq_func (GxTransactionTask             *task,
                      gpointer                       none,
                      GxBackendTransactionOpPayload *payload);

static void
gx_transaction_dispose (GObject *object)
{
  GxTransaction        *self = GX_TRANSACTION (object);
  GxTransactionPrivate *priv = gx_transaction_get_instance_private (self);

  g_clear_pointer (&priv->name, g_free);
  g_clear_object (&priv->installs);
  g_clear_object (&priv->updates);
  g_clear_object (&priv->removals);
  g_clear_object (&priv->current_ops);
  g_clear_object (&priv->finished_ops);
  g_clear_pointer (&priv->status, g_free);
  g_clear_pointer (&priv->error, g_free);

  G_OBJECT_CLASS (gx_transaction_parent_class)->dispose (object);
}

static void
gx_transaction_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  GxTransaction        *self = GX_TRANSACTION (object);
  GxTransactionPrivate *priv = gx_transaction_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_NAME:
      g_value_set_string (value, priv->name);
      break;
    case PROP_INSTALLS:
      g_value_set_object (value, priv->installs);
      break;
    case PROP_UPDATES:
      g_value_set_object (value, priv->updates);
      break;
    case PROP_REMOVALS:
      g_value_set_object (value, priv->removals);
      break;
    case PROP_PENDING:
      g_value_set_boolean (value, priv->pending);
      break;
    case PROP_CURRENT_OPS:
      g_value_set_object (value, priv->current_ops);
      break;
    case PROP_FINISHED_OPS:
      g_value_set_object (value, priv->finished_ops);
      break;
    case PROP_STATUS:
      g_value_set_string (value, priv->status);
      break;
    case PROP_PROGRESS:
      g_value_set_double (value, priv->progress);
      break;
    case PROP_FINISHED:
      g_value_set_boolean (value, priv->finished);
      break;
    case PROP_SUCCESS:
      g_value_set_boolean (value, priv->success);
      break;
    case PROP_ERROR:
      g_value_set_string (value, priv->error);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_transaction_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  GxTransaction        *self = GX_TRANSACTION (object);
  GxTransactionPrivate *priv = gx_transaction_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_NAME:
      g_clear_pointer (&priv->name, g_free);
      priv->name = g_value_dup_string (value);
      break;
    case PROP_INSTALLS:
      g_clear_object (&priv->installs);
      priv->installs = g_value_dup_object (value);
      break;
    case PROP_UPDATES:
      g_clear_object (&priv->updates);
      priv->updates = g_value_dup_object (value);
      break;
    case PROP_REMOVALS:
      g_clear_object (&priv->removals);
      priv->removals = g_value_dup_object (value);
      break;
    case PROP_PENDING:
      priv->pending = g_value_get_boolean (value);
      break;
    case PROP_STATUS:
      g_clear_pointer (&priv->status, g_free);
      priv->status = g_value_dup_string (value);
      break;
    case PROP_PROGRESS:
      priv->progress = g_value_get_double (value);
      break;
    case PROP_FINISHED:
      priv->finished = g_value_get_boolean (value);
      break;
    case PROP_SUCCESS:
      priv->success = g_value_get_boolean (value);
      break;
    case PROP_ERROR:
      g_clear_pointer (&priv->error, g_free);
      priv->error = g_value_dup_string (value);
      break;
    case PROP_CURRENT_OPS:
    case PROP_FINISHED_OPS:
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_transaction_class_init (GxTransactionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_transaction_set_property;
  object_class->get_property = gx_transaction_get_property;
  object_class->dispose      = gx_transaction_dispose;

  props[PROP_NAME] =
      g_param_spec_string (
          "name",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_INSTALLS] =
      g_param_spec_object (
          "installs",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_UPDATES] =
      g_param_spec_object (
          "updates",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_REMOVALS] =
      g_param_spec_object (
          "removals",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_PENDING] =
      g_param_spec_boolean (
          "pending",
          NULL, NULL, TRUE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_CURRENT_OPS] =
      g_param_spec_object (
          "current-ops",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  props[PROP_FINISHED_OPS] =
      g_param_spec_object (
          "finished-ops",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

  props[PROP_STATUS] =
      g_param_spec_string (
          "status",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_PROGRESS] =
      g_param_spec_double (
          "progress",
          NULL, NULL,
          0.0, 1.0, 0.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_FINISHED] =
      g_param_spec_boolean (
          "finished",
          NULL, NULL, FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_SUCCESS] =
      g_param_spec_boolean (
          "success",
          NULL, NULL, FALSE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  props[PROP_ERROR] =
      g_param_spec_string (
          "error",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_transaction_init (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;
  g_autoptr (GDateTime) now  = NULL;

  priv = gx_transaction_get_instance_private (self);

  now        = g_date_time_new_now_local ();
  priv->name = g_date_time_format (now, "%X");

  priv->installs     = g_list_store_new (GX_TYPE_ENTRY);
  priv->updates      = g_list_store_new (GX_TYPE_ENTRY);
  priv->removals     = g_list_store_new (GX_TYPE_ENTRY);
  priv->pending      = TRUE;
  priv->current_ops  = g_list_store_new (GX_TYPE_TRANSACTION_TASK);
  priv->finished_ops = g_list_store_new (GX_TYPE_TRANSACTION_TASK);
  priv->status       = g_strdup (_ ("Pending"));
  priv->success      = TRUE;
}

GxTransaction *
gx_transaction_new_full (GxEntry **installs,
                         guint     n_installs,
                         GxEntry **updates,
                         guint     n_updates,
                         GxEntry **removals,
                         guint     n_removals)

{
  g_autoptr (GxTransaction) self = NULL;
  GxTransactionPrivate *priv     = NULL;

  g_return_val_if_fail ((installs != NULL && n_installs > 0) ||
                            (updates != NULL && n_updates > 0) ||
                            (removals != NULL && n_removals),
                        NULL);

  for (guint i = 0; i < n_installs; i++)
    g_return_val_if_fail (GX_IS_ENTRY (installs[i]), NULL);
  for (guint i = 0; i < n_updates; i++)
    g_return_val_if_fail (GX_IS_ENTRY (updates[i]), NULL);
  for (guint i = 0; i < n_removals; i++)
    g_return_val_if_fail (GX_IS_ENTRY (removals[i]), NULL);

  self = g_object_new (GX_TYPE_TRANSACTION, NULL);
  priv = gx_transaction_get_instance_private (self);

  for (guint i = 0; i < n_installs; i++)
    g_list_store_append (priv->installs, installs[i]);
  for (guint i = 0; i < n_updates; i++)
    g_list_store_append (priv->updates, updates[i]);
  for (guint i = 0; i < n_removals; i++)
    g_list_store_append (priv->removals, removals[i]);

  return g_steal_pointer (&self);
}

GxTransaction *
gx_transaction_new_merged (GxTransaction **transactions,
                           guint           n_transactions)
{
  g_autoptr (GPtrArray) installs_pa = NULL;
  g_autoptr (GPtrArray) updates_pa  = NULL;
  g_autoptr (GPtrArray) removals_pa = NULL;

  g_return_val_if_fail (transactions != NULL, NULL);
  g_return_val_if_fail (n_transactions >= 2, NULL);

  installs_pa = g_ptr_array_new_with_free_func (g_object_unref);
  updates_pa  = g_ptr_array_new_with_free_func (g_object_unref);
  removals_pa = g_ptr_array_new_with_free_func (g_object_unref);

  for (guint i = 0; i < n_transactions; i++)
    {
      GListModel *installs   = NULL;
      GListModel *updates    = NULL;
      GListModel *removals   = NULL;
      guint       n_installs = 0;
      guint       n_updates  = 0;
      guint       n_removals = 0;

      installs = gx_transaction_get_installs (transactions[i]);
      updates  = gx_transaction_get_updates (transactions[i]);
      removals = gx_transaction_get_removals (transactions[i]);

      if (installs != NULL)
        n_installs = g_list_model_get_n_items (installs);
      if (updates != NULL)
        n_updates = g_list_model_get_n_items (updates);
      if (removals != NULL)
        n_removals = g_list_model_get_n_items (removals);

      for (guint j = 0; j < n_installs; j++)
        g_ptr_array_add (installs_pa, g_list_model_get_item (installs, j));
      for (guint j = 0; j < n_updates; j++)
        g_ptr_array_add (updates_pa, g_list_model_get_item (updates, j));
      for (guint j = 0; j < n_removals; j++)
        g_ptr_array_add (removals_pa, g_list_model_get_item (removals, j));
    }

  return gx_transaction_new_full (
      (GxEntry **) installs_pa->pdata,
      installs_pa->len,
      (GxEntry **) updates_pa->pdata,
      updates_pa->len,
      (GxEntry **) removals_pa->pdata,
      removals_pa->len);
}

GListModel *
gx_transaction_get_installs (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;

  g_return_val_if_fail (GX_IS_TRANSACTION (self), NULL);

  priv = gx_transaction_get_instance_private (self);
  return G_LIST_MODEL (priv->installs);
}

GListModel *
gx_transaction_get_updates (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;

  g_return_val_if_fail (GX_IS_TRANSACTION (self), NULL);

  priv = gx_transaction_get_instance_private (self);
  return G_LIST_MODEL (priv->updates);
}

GListModel *
gx_transaction_get_removals (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;

  g_return_val_if_fail (GX_IS_TRANSACTION (self), NULL);

  priv = gx_transaction_get_instance_private (self);
  return G_LIST_MODEL (priv->removals);
}

void
gx_transaction_hold (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));

  priv = gx_transaction_get_instance_private (self);

#define HOLD_MODEL(_model)                                            \
  if ((_model) != NULL)                                               \
    {                                                                 \
      guint n_items = 0;                                              \
                                                                      \
      n_items = g_list_model_get_n_items (G_LIST_MODEL ((_model)));   \
      for (guint i = 0; i < n_items; i++)                             \
        {                                                             \
          g_autoptr (GxEntry) entry = NULL;                           \
                                                                      \
          entry = g_list_model_get_item (G_LIST_MODEL ((_model)), i); \
          gx_entry_hold (entry);                                      \
        }                                                             \
    }

  HOLD_MODEL (priv->installs);
  HOLD_MODEL (priv->updates);
  HOLD_MODEL (priv->removals);

#undef HOLD_MODEL
}

void
gx_transaction_release (GxTransaction *self)
{
  GxTransactionPrivate *priv = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));

  priv = gx_transaction_get_instance_private (self);

#define RELEASE_MODEL(_model)                                         \
  if ((_model) != NULL)                                               \
    {                                                                 \
      guint n_items = 0;                                              \
                                                                      \
      n_items = g_list_model_get_n_items (G_LIST_MODEL ((_model)));   \
      for (guint i = 0; i < n_items; i++)                             \
        {                                                             \
          g_autoptr (GxEntry) entry = NULL;                           \
                                                                      \
          entry = g_list_model_get_item (G_LIST_MODEL ((_model)), i); \
          gx_entry_release (entry);                                   \
        }                                                             \
    }

  RELEASE_MODEL (priv->installs);
  RELEASE_MODEL (priv->updates);
  RELEASE_MODEL (priv->removals);

#undef RELEASE_MODEL
}

void
gx_transaction_add_task (GxTransaction                 *self,
                         GxBackendTransactionOpPayload *payload)
{
  GxTransactionPrivate *priv         = NULL;
  g_autoptr (GxTransactionTask) task = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (payload));

  priv = gx_transaction_get_instance_private (self);

  task = gx_transaction_task_new ();
  gx_transaction_task_set_op (task, payload);

  g_list_store_append (priv->current_ops, task);
}

void
gx_transaction_update_task (GxTransaction                         *self,
                            GxBackendTransactionOpProgressPayload *payload)
{
  GxTransactionPrivate          *priv                   = NULL;
  GxBackendTransactionOpPayload *op                     = NULL;
  guint                          op_pos                 = 0;
  gboolean                       found_payload_in_tasks = FALSE;
  g_autoptr (GxTransactionTask) task                    = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PROGRESS_PAYLOAD (payload));

  priv = gx_transaction_get_instance_private (self);

  op = gx_backend_transaction_op_progress_payload_get_op (payload);

  found_payload_in_tasks = g_list_store_find_with_equal_func_full (
      priv->current_ops, NULL, (GEqualFuncFull) find_payload_eq_func, op, &op_pos);
  g_return_if_fail (found_payload_in_tasks);

  task = g_list_model_get_item (G_LIST_MODEL (priv->current_ops), op_pos);
  gx_transaction_task_set_last_progress (task, payload);
}

void
gx_transaction_finish_task (GxTransaction                 *self,
                            GxBackendTransactionOpPayload *payload)
{
  GxTransactionPrivate *priv                   = NULL;
  guint                 op_pos                 = 0;
  gboolean              found_payload_in_tasks = FALSE;
  g_autoptr (GxTransactionTask) task           = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (payload));

  priv = gx_transaction_get_instance_private (self);

  found_payload_in_tasks = g_list_store_find_with_equal_func_full (
      priv->current_ops, NULL, (GEqualFuncFull) find_payload_eq_func, payload, &op_pos);
  g_return_if_fail (found_payload_in_tasks);

  task = g_list_model_get_item (G_LIST_MODEL (priv->current_ops), op_pos);
  g_list_store_remove (priv->current_ops, op_pos);
  g_list_store_append (priv->finished_ops, task);
}

void
gx_transaction_error_out_task (GxTransaction                 *self,
                               GxBackendTransactionOpPayload *payload,
                               const char                    *message)
{
  GxTransactionPrivate *priv                   = NULL;
  guint                 op_pos                 = 0;
  gboolean              found_payload_in_tasks = FALSE;
  g_autoptr (GxTransactionTask) task           = NULL;

  g_return_if_fail (GX_IS_TRANSACTION (self));
  g_return_if_fail (GX_IS_BACKEND_TRANSACTION_OP_PAYLOAD (payload));
  g_return_if_fail (message != NULL);

  priv = gx_transaction_get_instance_private (self);

  found_payload_in_tasks = g_list_store_find_with_equal_func_full (
      priv->current_ops, NULL, (GEqualFuncFull) find_payload_eq_func, payload, &op_pos);
  g_return_if_fail (found_payload_in_tasks);

  task = g_list_model_get_item (G_LIST_MODEL (priv->current_ops), op_pos);
  gx_transaction_task_set_error (task, message);

  g_list_store_remove (priv->current_ops, op_pos);
  g_list_store_append (priv->finished_ops, task);
}

static gboolean
find_payload_eq_func (GxTransactionTask             *task,
                      gpointer                       none,
                      GxBackendTransactionOpPayload *payload)
{
  return gx_transaction_task_get_op (task) == payload;
}
