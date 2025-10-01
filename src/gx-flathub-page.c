/* bz-flathub-page.c
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

#include "gx-flathub-page.h"
#include "gx-app-tile.h"
#include "gx-category-page.h"
#include "gx-category-tile.h"
#include "gx-detailed-app-tile.h"
#include "gx-dynamic-list-view.h"
#include "gx-entry-group.h"
#include "gx-flathub-category.h"
#include "gx-inhibited-scrollable.h"
#include "gx-patterned-background.h"
#include "gx-section-view.h"
#include "gx-window.h"

struct _GxFlathubPage
{
  AdwBin parent_instance;

  GxFlathubState *state;

  /* Template widgets */
  AdwViewStack *stack;
};

G_DEFINE_FINAL_TYPE (GxFlathubPage, gx_flathub_page, ADW_TYPE_BIN)

enum
{
  PROP_0,

  PROP_STATE,

  LAST_PROP
};
static GParamSpec *props[LAST_PROP] = { 0 };

enum
{
  SIGNAL_GROUP_SELECTED,

  LAST_SIGNAL,
};
static guint signals[LAST_SIGNAL];

static void
tile_clicked (GxEntryGroup *group,
              GtkButton    *button);

static void
category_clicked (GxFlathubCategory *category,
                  GtkButton         *button);

static void
category_page_select_cb (GxFlathubPage  *self,
                         GxEntryGroup   *group,
                         GxCategoryPage *page);

static void
category_page_hiding_cb (GxFlathubPage  *self,
                         GxCategoryPage *page);

static void
gx_flathub_page_dispose (GObject *object)
{
  GxFlathubPage *self = GX_FLATHUB_PAGE (object);

  g_clear_object (&self->state);

  G_OBJECT_CLASS (gx_flathub_page_parent_class)->dispose (object);
}

static void
gx_flathub_page_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GxFlathubPage *self = GX_FLATHUB_PAGE (object);

  switch (prop_id)
    {
    case PROP_STATE:
      g_value_set_object (value, gx_flathub_page_get_state (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gx_flathub_page_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GxFlathubPage *self = GX_FLATHUB_PAGE (object);

  switch (prop_id)
    {
    case PROP_STATE:
      gx_flathub_page_set_state (self, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static guint
limit_if_false (gpointer object,
                gboolean value)
{
  return value ? 96 : 12;
}

static void
bind_widget_cb (GxFlathubPage     *self,
                GxAppTile         *tile,
                GxEntryGroup      *group,
                GxDynamicListView *view)
{
  g_signal_connect_swapped (tile, "clicked", G_CALLBACK (tile_clicked), group);
}

static void
unbind_widget_cb (GxFlathubPage     *self,
                  GxAppTile         *tile,
                  GxEntryGroup      *group,
                  GxDynamicListView *view)
{
  g_signal_handlers_disconnect_by_func (tile, G_CALLBACK (tile_clicked), group);
}

static void
bind_category_tile_cb (GxFlathubPage     *self,
                       GxCategoryTile    *tile,
                       GxFlathubCategory *category,
                       GxDynamicListView *view)
{
  g_signal_connect_swapped (tile, "clicked", G_CALLBACK (category_clicked), category);
}

static void
unbind_category_tile_cb (GxFlathubPage     *self,
                         GxCategoryTile    *tile,
                         GxFlathubCategory *category,
                         GxDynamicListView *view)
{
  g_signal_handlers_disconnect_by_func (tile, category_clicked, category);
}

static void
gx_flathub_page_class_init (GxFlathubPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose      = gx_flathub_page_dispose;
  object_class->get_property = gx_flathub_page_get_property;
  object_class->set_property = gx_flathub_page_set_property;

  props[PROP_STATE] =
      g_param_spec_object (
          "state",
          NULL, NULL,
          GX_TYPE_FLATHUB_STATE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  signals[SIGNAL_GROUP_SELECTED] =
      g_signal_new (
          "group-selected",
          G_OBJECT_CLASS_TYPE (klass),
          G_SIGNAL_RUN_FIRST,
          0,
          NULL, NULL,
          g_cclosure_marshal_VOID__OBJECT,
          G_TYPE_NONE, 1,
          GX_TYPE_ENTRY_GROUP);
  g_signal_set_va_marshaller (
      signals[SIGNAL_GROUP_SELECTED],
      G_TYPE_FROM_CLASS (klass),
      g_cclosure_marshal_VOID__OBJECTv);

  g_type_ensure (GX_TYPE_SECTION_VIEW);
  g_type_ensure (GX_TYPE_CATEGORY_TILE);
  g_type_ensure (GX_TYPE_PATTERNED_BACKGROUND);
  g_type_ensure (GX_TYPE_DETAILED_APP_TILE);
  g_type_ensure (GX_TYPE_INHIBITED_SCROLLABLE);
  g_type_ensure (GX_TYPE_DYNAMIC_LIST_VIEW);
  g_type_ensure (GX_TYPE_APP_TILE);

  gtk_widget_class_set_template_from_resource (widget_class, "/io/github/gamerx/GamerXStore/gx-flathub-page.ui");
  gtk_widget_class_bind_template_child (widget_class, GxFlathubPage, stack);
  gtk_widget_class_bind_template_callback (widget_class, limit_if_false);
  gtk_widget_class_bind_template_callback (widget_class, bind_widget_cb);
  gtk_widget_class_bind_template_callback (widget_class, unbind_widget_cb);
  gtk_widget_class_bind_template_callback (widget_class, bind_category_tile_cb);
  gtk_widget_class_bind_template_callback (widget_class, unbind_category_tile_cb);
}

static void
gx_flathub_page_init (GxFlathubPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

GtkWidget *
gx_flathub_page_new (void)
{
  return g_object_new (GX_TYPE_FLATHUB_PAGE, NULL);
}

void
gx_flathub_page_set_state (GxFlathubPage  *self,
                           GxFlathubState *state)
{
  g_return_if_fail (GX_IS_FLATHUB_PAGE (self));
  g_return_if_fail (state == NULL || GX_IS_FLATHUB_STATE (state));

  g_clear_object (&self->state);
  if (state != NULL)
    {
      self->state = g_object_ref (state);
      adw_view_stack_set_visible_child_name (self->stack, "content");
    }
  else
    adw_view_stack_set_visible_child_name (self->stack, "empty");

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_STATE]);
}

GxFlathubState *
gx_flathub_page_get_state (GxFlathubPage *self)
{
  g_return_val_if_fail (GX_IS_FLATHUB_PAGE (self), NULL);
  return self->state;
}

static void
tile_clicked (GxEntryGroup *group,
              GtkButton    *button)
{
  GtkWidget *self = NULL;

  self = gtk_widget_get_ancestor (GTK_WIDGET (button), GX_TYPE_FLATHUB_PAGE);
  g_signal_emit (self, signals[SIGNAL_GROUP_SELECTED], 0, group);
}

static void
category_clicked (GxFlathubCategory *category,
                  GtkButton         *button)
{
  GtkWidget         *self          = NULL;
  GtkWidget         *window        = NULL;
  GtkWidget         *nav_view      = NULL;
  AdwNavigationPage *category_page = NULL;

  self = gtk_widget_get_ancestor (GTK_WIDGET (button), GX_TYPE_FLATHUB_PAGE);
  g_assert (self != NULL);

  window = GTK_WIDGET (gtk_widget_get_root (GTK_WIDGET (self)));

  nav_view = gtk_widget_get_ancestor (GTK_WIDGET (self), ADW_TYPE_NAVIGATION_VIEW);
  g_assert (nav_view != NULL);

  category_page = gx_category_page_new (category);

  g_signal_connect_swapped (
      category_page, "select",
      G_CALLBACK (category_page_select_cb), self);
  g_signal_connect_swapped (
      category_page, "hiding",
      G_CALLBACK (category_page_hiding_cb), self);

  adw_navigation_view_push (ADW_NAVIGATION_VIEW (nav_view), category_page);

  gx_window_set_category_view_mode (GX_WINDOW (window), TRUE);
}

static void
category_page_select_cb (GxFlathubPage  *self,
                         GxEntryGroup   *group,
                         GxCategoryPage *page)
{
  g_signal_emit (self, signals[SIGNAL_GROUP_SELECTED], 0, group);
}

static void
category_page_hiding_cb (GxFlathubPage  *self,
                         GxCategoryPage *page)
{
  GtkWidget *window = NULL;

  window = GTK_WIDGET (gtk_widget_get_root (GTK_WIDGET (self)));

  gx_window_set_category_view_mode (GX_WINDOW (window), FALSE);
}
