/* bz-dynamic-list-view.h
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

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

typedef enum
{
  GX_DYNAMIC_LIST_VIEW_KIND_LIST_BOX,
  GX_DYNAMIC_LIST_VIEW_KIND_FLOW_BOX,
  GX_DYNAMIC_LIST_VIEW_KIND_CAROUSEL,

  /*< private >*/
  GX_DYNAMIC_LIST_VIEW_N_KINDS,
} GxDynamicListViewKind;

GType gx_dynamic_list_view_kind_get_type (void);
#define GX_TYPE_DYNAMIC_LIST_VIEW_KIND (gx_dynamic_list_view_kind_get_type ())

#define GX_TYPE_DYNAMIC_LIST_VIEW (gx_dynamic_list_view_get_type ())
G_DECLARE_FINAL_TYPE (GxDynamicListView, gx_dynamic_list_view, GX, DYNAMIC_LIST_VIEW, AdwBin)

GxDynamicListView *
gx_dynamic_list_view_new (void);

GListModel *
gx_dynamic_list_view_get_model (GxDynamicListView *self);

gboolean
gx_dynamic_list_view_get_scroll (GxDynamicListView *self);

GxDynamicListViewKind
gx_dynamic_list_view_get_noscroll_kind (GxDynamicListView *self);

const char *
gx_dynamic_list_view_get_child_type (GxDynamicListView *self);

const char *
gx_dynamic_list_view_get_child_prop (GxDynamicListView *self);

const char *
gx_dynamic_list_view_get_object_prop (GxDynamicListView *self);

void
gx_dynamic_list_view_set_model (GxDynamicListView *self,
                                GListModel        *model);

void
gx_dynamic_list_view_set_scroll (GxDynamicListView *self,
                                 gboolean           scroll);

void
gx_dynamic_list_view_set_noscroll_kind (GxDynamicListView    *self,
                                        GxDynamicListViewKind noscroll_kind);

void
gx_dynamic_list_view_set_child_type (GxDynamicListView *self,
                                     const char        *child_type);

void
gx_dynamic_list_view_set_child_prop (GxDynamicListView *self,
                                     const char        *child_prop);

void
gx_dynamic_list_view_set_object_prop (GxDynamicListView *self,
                                      const char        *object_prop);

guint
gx_dynamic_list_view_get_max_children_per_line (GxDynamicListView *self);

void
gx_dynamic_list_view_set_max_children_per_line (GxDynamicListView *self,
                                                guint              max_children);

G_END_DECLS

/* End of bz-dynamic-list-view.h */
