/* bz-screenshot.c
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

#include "gx-screenshot.h"
#include "gx-async-texture.h"

struct _GxScreenshot
{
  GtkWidget parent_instance;

  GdkPaintable *paintable;
  double        focus_x;
  double        focus_y;
};

G_DEFINE_FINAL_TYPE (GxScreenshot, gx_screenshot, GTK_TYPE_WIDGET)

enum
{
  PROP_0,

  PROP_PAINTABLE,
  PROP_FOCUS_X,
  PROP_FOCUS_Y,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
invalidate_contents (GxScreenshot *self,
                     GdkPaintable *paintable);

static void
invalidate_size (GxScreenshot *self,
                 GdkPaintable *paintable);

static void
async_loaded (GxScreenshot   *self,
              GParamSpec     *pspec,
              GxAsyncTexture *texture);

static void
gx_screenshot_dispose (GObject *object)
{
  GxScreenshot *self = GX_SCREENSHOT (object);

  if (self->paintable != NULL)
    {
      g_signal_handlers_disconnect_by_func (self->paintable, invalidate_contents, self);
      g_signal_handlers_disconnect_by_func (self->paintable, invalidate_size, self);
      g_signal_handlers_disconnect_by_func (self->paintable, async_loaded, self);
    }
  g_clear_object (&self->paintable);

  G_OBJECT_CLASS (gx_screenshot_parent_class)->dispose (object);
}

static void
gx_screenshot_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  GxScreenshot *self = GX_SCREENSHOT (object);

  switch (prop_id)
    {
    case PROP_PAINTABLE:
      g_value_set_object (value, gx_screenshot_get_paintable (self));
      break;
    case PROP_FOCUS_X:
      g_value_set_double (value, gx_screenshot_get_focus_x (self));
      break;
    case PROP_FOCUS_Y:
      g_value_set_double (value, gx_screenshot_get_focus_y (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_screenshot_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  GxScreenshot *self = GX_SCREENSHOT (object);

  switch (prop_id)
    {
    case PROP_PAINTABLE:
      gx_screenshot_set_paintable (self, g_value_get_object (value));
      break;
    case PROP_FOCUS_X:
      gx_screenshot_set_focus_x (self, g_value_get_double (value));
      break;
    case PROP_FOCUS_Y:
      gx_screenshot_set_focus_y (self, g_value_get_double (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static GtkSizeRequestMode
gx_screenshot_get_request_mode (GtkWidget *widget)
{
  return GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH;
}

static void
gx_screenshot_measure (GtkWidget     *widget,
                       GtkOrientation orientation,
                       int            for_size,
                       int           *minimum,
                       int           *natural,
                       int           *minimum_baseline,
                       int           *natural_baseline)
{
  GxScreenshot *self = GX_SCREENSHOT (widget);

  if (self->paintable == NULL)
    return;

  if (orientation == GTK_ORIENTATION_VERTICAL)
    {
      int    intrinsic_height      = 0;
      double intrinsic_aspect_rato = 0.0;

      intrinsic_height      = gdk_paintable_get_intrinsic_height (self->paintable);
      intrinsic_aspect_rato = gdk_paintable_get_intrinsic_aspect_ratio (self->paintable);

      if (for_size >= 0 && intrinsic_aspect_rato > 0.0)
        {
          double result = 0.0;

          result = ceil ((double) for_size / intrinsic_aspect_rato);

          *minimum = (int) MIN (intrinsic_height, result);
          *natural = (int) MIN (intrinsic_height, result);
        }
      else
        {
          *minimum = 0;
          *natural = intrinsic_height;
        }
    }
  else
    {
      *minimum = 0;
      *natural = gdk_paintable_get_intrinsic_width (self->paintable);
    }
}

static void
gx_screenshot_snapshot (GtkWidget   *widget,
                        GtkSnapshot *snapshot)
{
  GxScreenshot  *self            = GX_SCREENSHOT (widget);
  int            widget_width    = 0;
  int            widget_height   = 0;
  int            paintable_width = 0;
  GskRoundedRect rect            = { 0 };

  if (self->paintable == NULL)
    return;

  widget_width    = gtk_widget_get_width (widget);
  widget_height   = gtk_widget_get_height (widget);
  paintable_width = gdk_paintable_get_intrinsic_width (self->paintable);

  if (widget_width > paintable_width)
    gtk_snapshot_translate (
        snapshot,
        &GRAPHENE_POINT_INIT (
            floor ((widget_width - paintable_width) / 2.0),
            0));

  rect.bounds = GRAPHENE_RECT_INIT (
      0, 0,
      MIN (widget_width, paintable_width),
      widget_height);

  rect.corner[0].width  = 10.0;
  rect.corner[0].height = 10.0;
  rect.corner[1].width  = 10.0;
  rect.corner[1].height = 10.0;
  rect.corner[2].width  = 10.0;
  rect.corner[2].height = 10.0;
  rect.corner[3].width  = 10.0;
  rect.corner[3].height = 10.0;

  gtk_snapshot_push_rounded_clip (snapshot, &rect);
  gdk_paintable_snapshot (
      self->paintable,
      snapshot,
      rect.bounds.size.width,
      rect.bounds.size.height);
  gtk_snapshot_pop (snapshot);
}

static void
gx_screenshot_class_init (GxScreenshotClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose      = gx_screenshot_dispose;
  object_class->get_property = gx_screenshot_get_property;
  object_class->set_property = gx_screenshot_set_property;

  props[PROP_PAINTABLE] =
      g_param_spec_object (
          "paintable",
          NULL, NULL,
          GDK_TYPE_PAINTABLE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_FOCUS_X] =
      g_param_spec_double (
          "focus-x",
          NULL, NULL,
          -1.0, G_MAXDOUBLE, -1.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_FOCUS_Y] =
      g_param_spec_double (
          "focus-y",
          NULL, NULL,
          -1.0, G_MAXDOUBLE, -1.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  widget_class->get_request_mode = gx_screenshot_get_request_mode;
  widget_class->measure          = gx_screenshot_measure;
  widget_class->snapshot         = gx_screenshot_snapshot;
}

static void
gx_screenshot_init (GxScreenshot *self)
{
  self->focus_x = -1.0;
  self->focus_y = -1.0;
}

GtkWidget *
gx_screenshot_new (void)
{
  return g_object_new (GX_TYPE_SCREENSHOT, NULL);
}

void
gx_screenshot_set_paintable (GxScreenshot *self,
                             GdkPaintable *paintable)
{
  g_return_if_fail (GX_IS_SCREENSHOT (self));
  g_return_if_fail (paintable == NULL || GDK_IS_PAINTABLE (paintable));

  if (self->paintable != NULL)
    {
      g_signal_handlers_disconnect_by_func (self->paintable, invalidate_contents, self);
      g_signal_handlers_disconnect_by_func (self->paintable, invalidate_size, self);
      g_signal_handlers_disconnect_by_func (self->paintable, async_loaded, self);
    }
  g_clear_object (&self->paintable);

  if (paintable != NULL)
    {
      self->paintable = g_object_ref (paintable);
      g_signal_connect_swapped (paintable, "invalidate-contents",
                                G_CALLBACK (invalidate_contents), self);
      g_signal_connect_swapped (paintable, "invalidate-size",
                                G_CALLBACK (invalidate_size), self);
      if (GX_IS_ASYNC_TEXTURE (paintable))
        g_signal_connect_swapped (paintable, "notify::loaded",
                                  G_CALLBACK (async_loaded), self);
    }

  gtk_widget_queue_resize (GTK_WIDGET (self));
  gtk_widget_queue_draw (GTK_WIDGET (self));

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_PAINTABLE]);
}

GdkPaintable *
gx_screenshot_get_paintable (GxScreenshot *self)
{
  g_return_val_if_fail (GX_IS_SCREENSHOT (self), NULL);
  return self->paintable;
}

void
gx_screenshot_set_focus_x (GxScreenshot *self,
                           double        focus_x)
{
  g_return_if_fail (GX_IS_SCREENSHOT (self));

  self->focus_x = focus_x;
  gtk_widget_queue_draw (GTK_WIDGET (self));

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_FOCUS_X]);
}

double
gx_screenshot_get_focus_x (GxScreenshot *self)
{
  g_return_val_if_fail (GX_IS_SCREENSHOT (self), 0.0);
  return self->focus_x;
}

void
gx_screenshot_set_focus_y (GxScreenshot *self,
                           double        focus_y)
{
  g_return_if_fail (GX_IS_SCREENSHOT (self));

  self->focus_y = focus_y;
  gtk_widget_queue_draw (GTK_WIDGET (self));

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_FOCUS_Y]);
}

double
gx_screenshot_get_focus_y (GxScreenshot *self)
{
  g_return_val_if_fail (GX_IS_SCREENSHOT (self), 0.0);
  return self->focus_y;
}

static void
invalidate_contents (GxScreenshot *self,
                     GdkPaintable *paintable)
{
  gtk_widget_queue_draw (GTK_WIDGET (self));
}

static void
invalidate_size (GxScreenshot *self,
                 GdkPaintable *paintable)
{
  gtk_widget_queue_resize (GTK_WIDGET (self));
}

static void
async_loaded (GxScreenshot   *self,
              GParamSpec     *pspec,
              GxAsyncTexture *texture)
{
  gtk_widget_queue_draw (GTK_WIDGET (self));
  gtk_widget_queue_resize (GTK_WIDGET (self));
}
