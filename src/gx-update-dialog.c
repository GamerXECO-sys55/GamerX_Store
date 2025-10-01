/* bz-update-dialog.c
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

#include "gx-entry.h"
#include "gx-update-dialog.h"

struct _GxUpdateDialog
{
  AdwAlertDialog parent_instance;

  GListModel         *updates;
  gboolean            install_accepted;
  GtkFilterListModel *app_filter;

  /* Template widgets */
  GtkNoSelection *selection_model;
  GtkLabel       *runtime_label;
};

G_DEFINE_FINAL_TYPE (GxUpdateDialog, gx_update_dialog, ADW_TYPE_ALERT_DIALOG)

static gboolean
match_for_app (GxEntry *item,
               gpointer user_data);

static void
on_response (AdwAlertDialog *alert,
             gchar          *response,
             GxUpdateDialog *self);

static void
gx_update_dialog_dispose (GObject *object)
{
  GxUpdateDialog *self = GX_UPDATE_DIALOG (object);

  g_clear_object (&self->updates);

  G_OBJECT_CLASS (gx_update_dialog_parent_class)->dispose (object);
}

static gboolean
invert_boolean (gpointer object,
                gboolean value)
{
  return !value;
}

static gboolean
is_null (gpointer object,
         GObject *value)
{
  return value == NULL;
}

static void
gx_update_dialog_class_init (GxUpdateDialogClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = gx_update_dialog_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/bz-update-dialog.ui");
  gtk_widget_class_bind_template_child (widget_class, GxUpdateDialog, selection_model);
  gtk_widget_class_bind_template_child (widget_class, GxUpdateDialog, runtime_label);
  gtk_widget_class_bind_template_callback (widget_class, invert_boolean);
  gtk_widget_class_bind_template_callback (widget_class, is_null);
}

static void
gx_update_dialog_init (GxUpdateDialog *self)
{
  GtkCustomFilter *filter = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));
  g_signal_connect (self, "response", G_CALLBACK (on_response), self);

  filter           = gtk_custom_filter_new ((GtkCustomFilterFunc) match_for_app, NULL, NULL);
  self->app_filter = gtk_filter_list_model_new (NULL, GTK_FILTER (filter));
  gtk_no_selection_set_model (self->selection_model, G_LIST_MODEL (self->app_filter));
}

static void
on_response (AdwAlertDialog *alert,
             gchar          *response,
             GxUpdateDialog *self)
{
  self->install_accepted = g_strcmp0 (response, "install") == 0;
}

AdwDialog *
gx_update_dialog_new (GListModel *updates)
{
  GxUpdateDialog *update_dialog = NULL;
  guint           n_updates     = 0;
  guint           n_apps        = 0;

  g_return_val_if_fail (G_IS_LIST_MODEL (updates), NULL);

  update_dialog          = g_object_new (GX_TYPE_UPDATE_DIALOG, NULL);
  update_dialog->updates = g_object_ref (updates);

  gtk_filter_list_model_set_model (update_dialog->app_filter, updates);

  n_updates = g_list_model_get_n_items (updates);
  n_apps    = g_list_model_get_n_items (G_LIST_MODEL (update_dialog->app_filter));
  if (n_updates > 0)
    {
      if (n_apps == 0)
        {
          g_autofree char *body = NULL;

          body = g_strdup_printf (_ ("%d runtimes and/or addons are eligible for updates. Would you like to install them?"), n_updates);
          adw_alert_dialog_set_body (ADW_ALERT_DIALOG (update_dialog), body);
          adw_alert_dialog_set_extra_child (ADW_ALERT_DIALOG (update_dialog), NULL);
        }
      else if (n_updates > n_apps)
        {
          g_autofree char *label = NULL;

          label = g_strdup_printf (_ ("Additionally, %d runtimes and/or addons will be updated."), n_updates - n_apps);
          gtk_label_set_label (update_dialog->runtime_label, label);
          gtk_widget_set_visible (GTK_WIDGET (update_dialog->runtime_label), TRUE);
        }
    }

  return ADW_DIALOG (update_dialog);
}

GListModel *
gx_update_dialog_was_accepted (GxUpdateDialog *self)
{
  g_return_val_if_fail (GX_IS_UPDATE_DIALOG (self), FALSE);

  return self->install_accepted ? g_object_ref (self->updates) : NULL;
}

static gboolean
match_for_app (GxEntry *item,
               gpointer user_data)
{
  return gx_entry_is_of_kinds (item, GX_ENTRY_KIND_APPLICATION);
}
