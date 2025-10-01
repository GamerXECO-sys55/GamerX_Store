/* bz-entry.h
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

#include <gtk/gtk.h>
#include <libdex.h>

G_BEGIN_DECLS

typedef enum
{
  GX_ENTRY_KIND_APPLICATION = 1 << 0,
  GX_ENTRY_KIND_RUNTIME     = 1 << 1,
  GX_ENTRY_KIND_ADDON       = 1 << 2,
} GxEntryKind;

GType gx_entry_kind_get_type (void);
#define GX_TYPE_ENTRY_KIND (gx_entry_kind_get_type ())

#define GX_TYPE_ENTRY (gx_entry_get_type ())
G_DECLARE_DERIVABLE_TYPE (GxEntry, gx_entry, GX, ENTRY, GObject)

struct _GxEntryClass
{
  GObjectClass parent_class;
};

void
gx_entry_hold (GxEntry *self);

void
gx_entry_release (GxEntry *self);

gboolean
gx_entry_is_holding (GxEntry *self);

gboolean
gx_entry_is_installed (GxEntry *self);

void
gx_entry_set_installed (GxEntry *self,
                        gboolean installed);

gboolean
gx_entry_is_of_kinds (GxEntry *self,
                      guint    kinds);

void
gx_entry_append_addon (GxEntry    *self,
                       const char *id);

GListModel *
gx_entry_get_addons (GxEntry *self);

const char *
gx_entry_get_id (GxEntry *self);

const char *
gx_entry_get_unique_id (GxEntry *self);

const char *
gx_entry_get_unique_id_checksum (GxEntry *self);

const char *
gx_entry_get_title (GxEntry *self);

const char *
gx_entry_get_developer (GxEntry *self);

const char *
gx_entry_get_eol (GxEntry *self);

const char *
gx_entry_get_description (GxEntry *self);

const char *
gx_entry_get_long_description (GxEntry *self);

const char *
gx_entry_get_remote_repo_name (GxEntry *self);

guint64
gx_entry_get_size (GxEntry *self);

GdkPaintable *
gx_entry_get_icon_paintable (GxEntry *self);

GListModel *
gx_entry_get_screenshot_paintables (GxEntry *self);

GIcon *
gx_entry_get_mini_icon (GxEntry *self);

GPtrArray *
gx_entry_get_search_tokens (GxEntry *self);

GListModel *
gx_entry_get_share_urls (GxEntry *self);

const char *
gx_entry_get_url (GxEntry *self);

const char *
gx_entry_get_donation_url (GxEntry *self);

const char *
gx_entry_get_forge_url (GxEntry *self);

gboolean
gx_entry_get_is_foss (GxEntry *self);

const char *
gx_entry_get_light_accent_color (GxEntry *self);

const char *
gx_entry_get_dark_accent_color (GxEntry *self);

gboolean
gx_entry_get_is_flathub (GxEntry *self);

DexFuture *
gx_entry_load_mini_icon (GxEntry *self);

gint
gx_entry_calc_usefulness (GxEntry *self);

void
gx_entry_serialize (GxEntry         *self,
                    GVariantBuilder *builder);

gboolean
gx_entry_deserialize (GxEntry  *self,
                      GVariant *import,
                      GError  **error);

GIcon *
gx_load_mini_icon_sync (const char *unique_id_checksum,
                        const char *path);

G_END_DECLS
