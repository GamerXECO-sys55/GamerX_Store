/* bz-url.c
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

#include "gx-url.h"

struct _GxUrl
{
  GObject parent_instance;

  char *name;
  char *url;
};

G_DEFINE_FINAL_TYPE (GxUrl, gx_url, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_NAME,
  PROP_URL,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_url_dispose (GObject *object)
{
  GxUrl *self = GX_URL (object);

  g_clear_pointer (&self->name, g_free);
  g_clear_pointer (&self->url, g_free);

  G_OBJECT_CLASS (gx_url_parent_class)->dispose (object);
}

static void
gx_url_get_property (GObject    *object,
                     guint       prop_id,
                     GValue     *value,
                     GParamSpec *pspec)
{
  GxUrl *self = GX_URL (object);

  switch (prop_id)
    {
    case PROP_NAME:
      g_value_set_string (value, gx_url_get_name (self));
      break;
    case PROP_URL:
      g_value_set_string (value, gx_url_get_url (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_url_set_property (GObject      *object,
                     guint         prop_id,
                     const GValue *value,
                     GParamSpec   *pspec)
{
  GxUrl *self = GX_URL (object);

  switch (prop_id)
    {
    case PROP_NAME:
      gx_url_set_name (self, g_value_get_string (value));
      break;
    case PROP_URL:
      gx_url_set_url (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_url_class_init (GxUrlClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_url_set_property;
  object_class->get_property = gx_url_get_property;
  object_class->dispose      = gx_url_dispose;

  props[PROP_NAME] =
      g_param_spec_string (
          "name",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_URL] =
      g_param_spec_string (
          "url",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_url_init (GxUrl *self)
{
}

GxUrl *
gx_url_new (void)
{
  return g_object_new (GX_TYPE_URL, NULL);
}

const char *
gx_url_get_name (GxUrl *self)
{
  g_return_val_if_fail (GX_IS_URL (self), NULL);
  return self->name;
}

const char *
gx_url_get_url (GxUrl *self)
{
  g_return_val_if_fail (GX_IS_URL (self), NULL);
  return self->url;
}

void
gx_url_set_name (GxUrl      *self,
                 const char *name)
{
  g_return_if_fail (GX_IS_URL (self));

  g_clear_pointer (&self->name, g_free);
  if (name != NULL)
    self->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_NAME]);
}

void
gx_url_set_url (GxUrl      *self,
                const char *url)
{
  g_return_if_fail (GX_IS_URL (self));

  g_clear_pointer (&self->url, g_free);
  if (url != NULL)
    self->url = g_strdup (url);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_URL]);
}

/* End of bz-url.c */
