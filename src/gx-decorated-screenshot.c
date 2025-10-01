/* bz-decorated-screenshot.c
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

#include "gx-decorated-screenshot.h"
#include "gx-error.h"
#include "gx-window.h"
#include "gx-screenshot.h"
#include <glib/gi18n.h>

struct _GxDecoratedScreenshot
{
  GtkWidget parent_instance;

  GxAsyncTexture *async_texture;

  GtkEventController *motion;

  /* Template widgets */
  GtkWidget *buttons;
};

G_DEFINE_FINAL_TYPE (GxDecoratedScreenshot, gx_decorated_screenshot, ADW_TYPE_BIN);

enum
{
  PROP_0,

  PROP_ASYNC_TEXTURE,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_decorated_screenshot_dispose (GObject *object)
{
  GxDecoratedScreenshot *self = GX_DECORATED_SCREENSHOT (object);

  g_clear_pointer (&self->async_texture, g_object_unref);

  G_OBJECT_CLASS (gx_decorated_screenshot_parent_class)->dispose (object);
}

static void
gx_decorated_screenshot_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  GxDecoratedScreenshot *self = GX_DECORATED_SCREENSHOT (object);

  switch (prop_id)
    {
    case PROP_ASYNC_TEXTURE:
      g_value_set_object (value, gx_decorated_screenshot_get_async_texture (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_decorated_screenshot_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  GxDecoratedScreenshot *self = GX_DECORATED_SCREENSHOT (object);

  switch (prop_id)
    {
    case PROP_ASYNC_TEXTURE:
      gx_decorated_screenshot_set_async_texture (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
open_externally_clicked (GxDecoratedScreenshot *self,
                         GtkButton             *button)
{
  g_autoptr (GError) local_error = NULL;
  const char      *cache_path    = NULL;
  g_autofree char *uri           = NULL;
  gboolean         result        = FALSE;

  cache_path = gx_async_texture_get_cache_into_path (self->async_texture);
  if (cache_path == NULL)
    return;

  uri    = g_strdup_printf ("file://%s", cache_path);
  result = g_app_info_launch_default_for_uri (uri, NULL, &local_error);

  if (!result)
    {
      GtkWidget *window = NULL;

      window = gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_WINDOW);
      if (window != NULL)
        gx_show_error_for_widget (window, local_error->message);
    }
}

static void
copy_clicked (GxDecoratedScreenshot *self,
              GtkButton             *button)
{
  g_autoptr (GdkTexture) texture = NULL;
  GdkClipboard *clipboard;
  GxWindow *window = NULL;
  AdwToast *toast = NULL;

  texture = gx_async_texture_dup_texture (self->async_texture);
  /* button shouldn't be clickable if not loaded */
  g_assert (texture != NULL);

  clipboard = gdk_display_get_clipboard (gdk_display_get_default ());
  gdk_clipboard_set_texture (clipboard, texture);

  window = GX_WINDOW (gtk_widget_get_root (GTK_WIDGET (self)));
  toast = adw_toast_new (_("Copied!"));
  adw_toast_set_timeout (toast, 1);
  gx_window_add_toast (window, toast);
}

static void
gx_decorated_screenshot_class_init (GxDecoratedScreenshotClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = gx_decorated_screenshot_set_property;
  object_class->get_property = gx_decorated_screenshot_get_property;
  object_class->dispose      = gx_decorated_screenshot_dispose;

  props[PROP_ASYNC_TEXTURE] =
      g_param_spec_object (
          "async-texture",
          NULL, NULL,
          GX_TYPE_ASYNC_TEXTURE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  g_type_ensure (GX_TYPE_SCREENSHOT);

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/bz-decorated-screenshot.ui");
  gtk_widget_class_bind_template_child (widget_class, GxDecoratedScreenshot, buttons);
  gtk_widget_class_bind_template_callback (widget_class, open_externally_clicked);
  gtk_widget_class_bind_template_callback (widget_class, copy_clicked);
}

static void
motion_enter (GxDecoratedScreenshot    *self,
              gdouble                   x,
              gdouble                   y,
              GtkEventControllerMotion *controller)
{
  // gtk_widget_set_opacity (self->buttons, 0.25);
  // gx_screenshot_set_focus_x (self->screenshot_widget, x);
  // gx_screenshot_set_focus_y (self->screenshot_widget, y);
}

static void
motion_event (GxDecoratedScreenshot    *self,
              gdouble                   x,
              gdouble                   y,
              GtkEventControllerMotion *controller)
{
  // gx_screenshot_set_focus_x (self->screenshot_widget, x);
  // gx_screenshot_set_focus_y (self->screenshot_widget, y);
}

static void
motion_leave (GxDecoratedScreenshot    *self,
              GtkEventControllerMotion *controller)
{
  // gtk_widget_set_opacity (self->buttons, 1.0);
  // gx_screenshot_set_focus_x (self->screenshot_widget, -1.0);
  // gx_screenshot_set_focus_y (self->screenshot_widget, -1.0);
}

static void
gx_decorated_screenshot_init (GxDecoratedScreenshot *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->motion = gtk_event_controller_motion_new ();
  g_signal_connect_swapped (self->motion, "enter", G_CALLBACK (motion_enter), self);
  g_signal_connect_swapped (self->motion, "motion", G_CALLBACK (motion_event), self);
  g_signal_connect_swapped (self->motion, "leave", G_CALLBACK (motion_leave), self);
  gtk_widget_add_controller (GTK_WIDGET (self), self->motion);
}

GxDecoratedScreenshot *
gx_decorated_screenshot_new (void)
{
  return g_object_new (GX_TYPE_DECORATED_SCREENSHOT, NULL);
}

GxAsyncTexture *
gx_decorated_screenshot_get_async_texture (GxDecoratedScreenshot *self)
{
  g_return_val_if_fail (GX_IS_DECORATED_SCREENSHOT (self), NULL);
  return self->async_texture;
}

void
gx_decorated_screenshot_set_async_texture (GxDecoratedScreenshot *self,
                                           GxAsyncTexture        *async_texture)
{
  g_return_if_fail (GX_IS_DECORATED_SCREENSHOT (self));

  g_clear_pointer (&self->async_texture, g_object_unref);
  if (async_texture != NULL)
    self->async_texture = g_object_ref (async_texture);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ASYNC_TEXTURE]);
}

/* End of bz-decorated-screenshot.c */
