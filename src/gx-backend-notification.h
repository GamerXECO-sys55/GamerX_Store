/* bz-backend-notification.h
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

#include "gx-entry.h"

G_BEGIN_DECLS

typedef enum
{
  GX_BACKEND_NOTIFICATION_KIND_ANY,
  GX_BACKEND_NOTIFICATION_KIND_INSTALLATION,
  GX_BACKEND_NOTIFICATION_KIND_UPDATE,
  GX_BACKEND_NOTIFICATION_KIND_REMOVAL,
} GxBackendNotificationKind;

GType gx_backend_notification_kind_get_type (void);
#define GX_TYPE_BACKEND_NOTIFICATION_KIND (gx_backend_notification_kind_get_type ())

#define GX_TYPE_BACKEND_NOTIFICATION (gx_backend_notification_get_type ())
G_DECLARE_FINAL_TYPE (GxBackendNotification, gx_backend_notification, GX, BACKEND_NOTIFICATION, GObject)

GxBackendNotification *
gx_backend_notification_new (void);

GxBackendNotificationKind
gx_backend_notification_get_kind (GxBackendNotification *self);

GxEntry *
gx_backend_notification_get_entry (GxBackendNotification *self);

const char *
gx_backend_notification_get_description (GxBackendNotification *self);

void
gx_backend_notification_set_kind (GxBackendNotification    *self,
                                  GxBackendNotificationKind kind);

void
gx_backend_notification_set_entry (GxBackendNotification *self,
                                   GxEntry               *entry);

void
gx_backend_notification_set_description (GxBackendNotification *self,
                                         const char            *description);

G_END_DECLS

/* End of bz-backend-notification.h */
