/* bz-release.c
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

#include "gx-release.h"

struct _GxRelease
{
  GObject parent_instance;

  char       *description;
  GListModel *issues;
  guint64     timestamp;
  char       *url;
  char       *version;
};

G_DEFINE_FINAL_TYPE (GxRelease, gx_release, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_DESCRIPTION,
  PROP_ISSUES,
  PROP_TIMESTAMP,
  PROP_URL,
  PROP_VERSION,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_release_dispose (GObject *object)
{
  GxRelease *self = GX_RELEASE (object);

  g_clear_pointer (&self->description, g_free);
  g_clear_pointer (&self->issues, g_object_unref);
  g_clear_pointer (&self->url, g_free);
  g_clear_pointer (&self->version, g_free);

  G_OBJECT_CLASS (gx_release_parent_class)->dispose (object);
}

static void
gx_release_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  GxRelease *self = GX_RELEASE (object);

  switch (prop_id)
    {
    case PROP_DESCRIPTION:
      g_value_set_string (value, gx_release_get_description (self));
      break;
    case PROP_ISSUES:
      g_value_set_object (value, gx_release_get_issues (self));
      break;
    case PROP_TIMESTAMP:
      g_value_set_uint64 (value, gx_release_get_timestamp (self));
      break;
    case PROP_URL:
      g_value_set_string (value, gx_release_get_url (self));
      break;
    case PROP_VERSION:
      g_value_set_string (value, gx_release_get_version (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_release_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
  GxRelease *self = GX_RELEASE (object);

  switch (prop_id)
    {
    case PROP_DESCRIPTION:
      gx_release_set_description (self, g_value_get_string (value));
      break;
    case PROP_ISSUES:
      gx_release_set_issues (self, g_value_get_object (value));
      break;
    case PROP_TIMESTAMP:
      gx_release_set_timestamp (self, g_value_get_uint64 (value));
      break;
    case PROP_URL:
      gx_release_set_url (self, g_value_get_string (value));
      break;
    case PROP_VERSION:
      gx_release_set_version (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_release_class_init (GxReleaseClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_release_set_property;
  object_class->get_property = gx_release_get_property;
  object_class->dispose      = gx_release_dispose;

  props[PROP_DESCRIPTION] =
      g_param_spec_string (
          "description",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_ISSUES] =
      g_param_spec_object (
          "issues",
          NULL, NULL,
          G_TYPE_LIST_MODEL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_TIMESTAMP] =
      g_param_spec_uint64 (
          "timestamp",
          NULL, NULL,
          0, G_MAXUINT64, (guint64) 0,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_URL] =
      g_param_spec_string (
          "url",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  props[PROP_VERSION] =
      g_param_spec_string (
          "version",
          NULL, NULL, NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
gx_release_init (GxRelease *self)
{
}

GxRelease *
gx_release_new (void)
{
  return g_object_new (GX_TYPE_RELEASE, NULL);
}

const char *
gx_release_get_description (GxRelease *self)
{
  g_return_val_if_fail (GX_IS_RELEASE (self), NULL);

  if (self->description == NULL)
    return NULL;

  return self->description;
}

GListModel *
gx_release_get_issues (GxRelease *self)
{
  g_return_val_if_fail (GX_IS_RELEASE (self), NULL);
  return self->issues;
}

guint64
gx_release_get_timestamp (GxRelease *self)
{
  g_return_val_if_fail (GX_IS_RELEASE (self), 0);
  return self->timestamp;
}

const char *
gx_release_get_url (GxRelease *self)
{
  g_return_val_if_fail (GX_IS_RELEASE (self), NULL);
  return self->url;
}

const char *
gx_release_get_version (GxRelease *self)
{
  g_return_val_if_fail (GX_IS_RELEASE (self), NULL);
  return self->version;
}

void
gx_release_set_description (GxRelease  *self,
                            const char *description)
{
  g_return_if_fail (GX_IS_RELEASE (self));

  g_clear_pointer (&self->description, g_free);
  if (description != NULL)
    self->description = g_strdup (description);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_DESCRIPTION]);
}

void
gx_release_set_issues (GxRelease  *self,
                       GListModel *issues)
{
  g_return_if_fail (GX_IS_RELEASE (self));

  g_clear_pointer (&self->issues, g_object_unref);
  if (issues != NULL)
    self->issues = g_object_ref (issues);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ISSUES]);
}

void
gx_release_set_timestamp (GxRelease *self,
                          guint64    timestamp)
{
  g_return_if_fail (GX_IS_RELEASE (self));

  self->timestamp = timestamp;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_TIMESTAMP]);
}

void
gx_release_set_url (GxRelease  *self,
                    const char *url)
{
  g_return_if_fail (GX_IS_RELEASE (self));

  g_clear_pointer (&self->url, g_free);
  if (url != NULL)
    self->url = g_strdup (url);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_URL]);
}

void
gx_release_set_version (GxRelease  *self,
                        const char *version)
{
  g_return_if_fail (GX_IS_RELEASE (self));

  g_clear_pointer (&self->version, g_free);
  if (version != NULL)
    self->version = g_strdup (version);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_VERSION]);
}

/* End of bz-release.c */
