/* bz-issue.c
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

#include "gx-issue.h"

struct _GxIssue
{
  GObject parent_instance;

  char *id;
  char *url;
};

G_DEFINE_FINAL_TYPE (GxIssue, gx_issue, G_TYPE_OBJECT);

enum
{
  PROP_0,

  PROP_ID,
  PROP_URL,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

static void
gx_issue_dispose (GObject *object)
{
  GxIssue *self = GX_ISSUE (object);

  g_clear_pointer (&self->id, g_free);
  g_clear_pointer (&self->url, g_free);

  G_OBJECT_CLASS (gx_issue_parent_class)->dispose (object);
}

static void
gx_issue_get_property (GObject    *object,
                       guint       prop_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
  GxIssue *self = GX_ISSUE (object);

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_string (value, gx_issue_get_id (self));
      break;
    case PROP_URL:
      g_value_set_string (value, gx_issue_get_url (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_issue_set_property (GObject      *object,
                       guint         prop_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
  GxIssue *self = GX_ISSUE (object);

  switch (prop_id)
    {
    case PROP_ID:
      gx_issue_set_id (self, g_value_get_string (value));
      break;
    case PROP_URL:
      gx_issue_set_url (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_issue_class_init (GxIssueClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gx_issue_set_property;
  object_class->get_property = gx_issue_get_property;
  object_class->dispose      = gx_issue_dispose;

  props[PROP_ID] =
      g_param_spec_string (
          "id",
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
gx_issue_init (GxIssue *self)
{
}

GxIssue *
gx_issue_new (void)
{
  return g_object_new (GX_TYPE_ISSUE, NULL);
}

const char *
gx_issue_get_id (GxIssue *self)
{
  g_return_val_if_fail (GX_IS_ISSUE (self), NULL);
  return self->id;
}

const char *
gx_issue_get_url (GxIssue *self)
{
  g_return_val_if_fail (GX_IS_ISSUE (self), NULL);
  return self->url;
}

void
gx_issue_set_id (GxIssue    *self,
                 const char *id)
{
  g_return_if_fail (GX_IS_ISSUE (self));

  g_clear_pointer (&self->id, g_free);
  if (id != NULL)
    self->id = g_strdup (id);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_ID]);
}

void
gx_issue_set_url (GxIssue    *self,
                  const char *url)
{
  g_return_if_fail (GX_IS_ISSUE (self));

  g_clear_pointer (&self->url, g_free);
  if (url != NULL)
    self->url = g_strdup (url);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_URL]);
}

/* End of bz-issue.c */
