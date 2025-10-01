/* bz-comet.c
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

#include "gx-comet.h"

struct _GxComet
{
  GObject parent_instance;

  char         *name;
  GtkWidget    *from;
  GtkWidget    *to;
  GdkPaintable *paintable;
  GskPath      *path;
  double        path_length;
  double        progress;
};

G_DEFINE_FINAL_TYPE (GxComet, gx_comet, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_NAME,
  PROP_FROM,
  PROP_TO,
  PROP_PAINTABLE,
  PROP_PATH,
  PROP_PATH_LENGTH,
  PROP_PROGRESS,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_comet_dispose (GObject *object)
{
  GxComet *self = GX_COMET (object);

  g_clear_pointer (&self->name, g_free);
  g_clear_pointer (&self->from, g_object_unref);
  g_clear_pointer (&self->to, g_object_unref);
  g_clear_pointer (&self->paintable, g_object_unref);
  g_clear_pointer (&self->path, gsk_path_unref);

  G_OBJECT_CLASS (gx_comet_parent_class)->dispose (object);
}

static void
gx_comet_get_property (GObject    *object,
                       guint       prop_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
  GxComet *self = GX_COMET (object);

  switch (prop_id)
    {
    case PROP_NAME:
      g_value_set_string (value, gx_comet_get_name (self));
      break;
    case PROP_FROM:
      g_value_set_object (value, gx_comet_get_from (self));
      break;
    case PROP_TO:
      g_value_set_object (value, gx_comet_get_to (self));
      break;
    case PROP_PAINTABLE:
      g_value_set_object (value, gx_comet_get_paintable (self));
      break;
    case PROP_PATH:
      g_value_set_boxed (value, gx_comet_get_path (self));
      break;
    case PROP_PATH_LENGTH:
      g_value_set_double (value, gx_comet_get_path_length (self));
      break;
    case PROP_PROGRESS:
      g_value_set_double (value, gx_comet_get_progress (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_comet_set_property (GObject      *object,
                       guint         prop_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
  GxComet *self = GX_COMET (object);

  switch (prop_id)
    {
    case PROP_NAME:
      gx_comet_set_name (self, g_value_get_string (value));
      break;
    case PROP_FROM:
      gx_comet_set_from (self, g_value_get_object (value));
      break;
    case PROP_TO:
      gx_comet_set_to (self, g_value_get_object (value));
      break;
    case PROP_PAINTABLE:
      gx_comet_set_paintable (self, g_value_get_object (value));
      break;
    case PROP_PATH:
      gx_comet_set_path (self, g_value_get_boxed (value));
      break;
    case PROP_PATH_LENGTH:
      gx_comet_set_path_length (self, g_value_get_double (value));
      break;
    case PROP_PROGRESS:
      gx_comet_set_progress (self, g_value_get_double (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_comet_class_init (GxCometClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_comet_set_property;
  object_class->get_property = gx_comet_get_property;
  object_class->dispose      = gx_comet_dispose;

  props[PROP_NAME] =
      g_param_spec_string (
          "name",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_FROM] =
      g_param_spec_object (
          "from",
          NULL, NULL,
          GTK_TYPE_WIDGET,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_TO] =
      g_param_spec_object (
          "to",
          NULL, NULL,
          GTK_TYPE_WIDGET,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_PAINTABLE] =
      g_param_spec_object (
          "paintable",
          NULL, NULL,
          GDK_TYPE_PAINTABLE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_PATH] =
      g_param_spec_boxed (
          "path",
          NULL, NULL,
          GSK_TYPE_PATH,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_PATH_LENGTH] =
      g_param_spec_double (
          "path-length",
          NULL, NULL,
          0.0, G_MAXDOUBLE, 0.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_PROGRESS] =
      g_param_spec_double (
          "progress",
          NULL, NULL,
          0.0, G_MAXDOUBLE, 0.0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_comet_init (GxComet *self)
{
}

const char *
gx_comet_get_name (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), NULL);
  return self->name;
}

GtkWidget *
gx_comet_get_from (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), NULL);
  return self->from;
}

GtkWidget *
gx_comet_get_to (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), NULL);
  return self->to;
}

GdkPaintable *
gx_comet_get_paintable (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), NULL);
  return self->paintable;
}

GskPath *
gx_comet_get_path (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), NULL);
  return self->path;
}

double
gx_comet_get_path_length (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), 0.0);
  return self->path_length;
}

double
gx_comet_get_progress (GxComet *self)
{
  g_return_val_if_fail (GX_IS_COMET (self), 0.0);
  return self->progress;
}

void
gx_comet_set_name (GxComet    *self,
                   const char *name)
{
  g_return_if_fail (GX_IS_COMET (self));

  g_clear_pointer (&self->name, g_free);
  self->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_NAME]);
}

void
gx_comet_set_from (GxComet   *self,
                   GtkWidget *from)
{
  g_return_if_fail (GX_IS_COMET (self));

  g_clear_pointer (&self->from, g_object_unref);
  self->from = g_object_ref (from);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_FROM]);
}

void
gx_comet_set_to (GxComet   *self,
                 GtkWidget *to)
{
  g_return_if_fail (GX_IS_COMET (self));

  g_clear_pointer (&self->to, g_object_unref);
  self->to = g_object_ref (to);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_TO]);
}

void
gx_comet_set_paintable (GxComet      *self,
                        GdkPaintable *paintable)
{
  g_return_if_fail (GX_IS_COMET (self));

  g_clear_pointer (&self->paintable, g_object_unref);
  self->paintable = g_object_ref (paintable);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_PAINTABLE]);
}

void
gx_comet_set_path (GxComet *self,
                   GskPath *path)
{
  g_return_if_fail (GX_IS_COMET (self));

  g_clear_pointer (&self->path, gsk_path_unref);
  self->path = gsk_path_ref (path);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_PATH]);
}

void
gx_comet_set_path_length (GxComet *self,
                          double   path_length)
{
  g_return_if_fail (GX_IS_COMET (self));

  self->path_length = path_length;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_PATH_LENGTH]);
}

void
gx_comet_set_progress (GxComet *self,
                       double   progress)
{
  g_return_if_fail (GX_IS_COMET (self));

  self->progress = progress;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_PROGRESS]);
}

/* End of bz-comet.c */
