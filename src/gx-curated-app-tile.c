/* bz-curated-app-tile.c
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

#include "gx-curated-app-tile.h"
#include "gx-app-tile.h"

struct _GxCuratedAppTile
{
  AdwBin parent_instance;

  GxEntryGroup *group;
};

G_DEFINE_FINAL_TYPE (GxCuratedAppTile, gx_curated_app_tile, ADW_TYPE_BIN);

enum
{
  PROP_0,

  PROP_GROUP,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

enum
{
  SIGNAL_CLICKED,

  LAST_SIGNAL,
};
static guint signals[LAST_SIGNAL];

static void
gx_curated_app_tile_dispose (GObject *object)
{
  GxCuratedAppTile *self = GX_CURATED_APP_TILE (object);

  g_clear_pointer (&self->group, g_object_unref);

  G_OBJECT_CLASS (gx_curated_app_tile_parent_class)->dispose (object);
}

static void
gx_curated_app_tile_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  GxCuratedAppTile *self = GX_CURATED_APP_TILE (object);

  switch (prop_id)
    {
    case PROP_GROUP:
      g_value_set_object (value, gx_curated_app_tile_get_group (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_curated_app_tile_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  GxCuratedAppTile *self = GX_CURATED_APP_TILE (object);

  switch (prop_id)
    {
    case PROP_GROUP:
      gx_curated_app_tile_set_group (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
clicked_cb (GxCuratedAppTile *self,
            GxAppTile        *tile)
{
  g_signal_emit (self, signals[SIGNAL_CLICKED], 0, NULL);
}

static void
gx_curated_app_tile_class_init (GxCuratedAppTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property = gx_curated_app_tile_set_property;
  object_class->get_property = gx_curated_app_tile_get_property;
  object_class->dispose      = gx_curated_app_tile_dispose;

  props[PROP_GROUP] =
      g_param_spec_object (
          "group",
          NULL, NULL,
          GX_TYPE_ENTRY_GROUP,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  signals[SIGNAL_CLICKED] =
      g_signal_new (
          "clicked",
          G_OBJECT_CLASS_TYPE (klass),
          G_SIGNAL_RUN_FIRST,
          0,
          NULL, NULL,
          g_cclosure_marshal_VOID__VOID,
          G_TYPE_NONE, 1,
          GX_TYPE_ENTRY);
  g_signal_set_va_marshaller (
      signals[SIGNAL_CLICKED],
      G_TYPE_FROM_CLASS (klass),
      g_cclosure_marshal_VOID__VOIDv);

  gtk_widget_class_set_css_name (widget_class, "app-tile");
  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/gx-curated-app-tile.ui");
  gtk_widget_class_bind_template_callback (widget_class, clicked_cb);
}

static void
gx_curated_app_tile_init (GxCuratedAppTile *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

GxCuratedAppTile *
gx_curated_app_tile_new (void)
{
  return g_object_new (GX_TYPE_CURATED_APP_TILE, NULL);
}

GxEntryGroup *
gx_curated_app_tile_get_group (GxCuratedAppTile *self)
{
  g_return_val_if_fail (GX_IS_CURATED_APP_TILE (self), NULL);
  return self->group;
}

void
gx_curated_app_tile_set_group (GxCuratedAppTile *self,
                               GxEntryGroup     *group)
{
  g_return_if_fail (GX_IS_CURATED_APP_TILE (self));

  g_clear_pointer (&self->group, g_object_unref);
  if (group != NULL)
    self->group = g_object_ref (group);

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_GROUP]);
}

/* End of bz-curated-app-tile.c */
