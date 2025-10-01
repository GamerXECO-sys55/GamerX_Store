/* bz-download-worker.h
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

#include <libdex.h>

G_BEGIN_DECLS

#define GX_TYPE_DOWNLOAD_WORKER (gx_download_worker_get_type ())
G_DECLARE_FINAL_TYPE (GxDownloadWorker, gx_download_worker, GX, DOWNLOAD_WORKER, GObject)

GxDownloadWorker *
gx_download_worker_new (const char *name,
                        GError    **error);

const char *
gx_download_worker_get_name (GxDownloadWorker *self);

void
gx_download_worker_set_name (GxDownloadWorker *self,
                             const char       *name);

DexFuture *
gx_download_worker_invoke (GxDownloadWorker *self,
                           GFile            *src,
                           GFile            *dest);

GxDownloadWorker *
gx_download_worker_get_default (void);

G_END_DECLS

/* End of bz-download-worker.h */
