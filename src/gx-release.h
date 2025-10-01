/* bz-release.h
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

G_BEGIN_DECLS

#define GX_TYPE_RELEASE (gx_release_get_type ())
G_DECLARE_FINAL_TYPE (GxRelease, gx_release, GX, RELEASE, GObject)

GxRelease *
gx_release_new (void);

GListModel *
gx_release_get_issues (GxRelease *self);

guint64
gx_release_get_timestamp (GxRelease *self);

const char *
gx_release_get_url (GxRelease *self);

const char *
gx_release_get_description (GxRelease *self);

const char *
gx_release_get_version (GxRelease *self);

void
gx_release_set_issues (GxRelease  *self,
                       GListModel *issues);

void
gx_release_set_timestamp (GxRelease *self,
                          guint64    timestamp);

void
gx_release_set_url (GxRelease  *self,
                    const char *url);

void
gx_release_set_description (GxRelease  *self,
                            const char *description);

void
gx_release_set_version (GxRelease  *self,
                        const char *version);

G_END_DECLS

/* End of bz-release.h */
