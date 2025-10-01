/* bz-io.h
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

#include <gio/gio.h>
#include <libdex.h>

G_BEGIN_DECLS

DexScheduler *
gx_get_io_scheduler (void);

void
gx_reap_file (GFile *file);

void
gx_reap_path (const char *path);

char *
gx_dup_cache_dir (const char *submodule);

#define gx_dup_module_dir() gx_dup_cache_dir (GAMERX_MODULE)

#define gx_discard_path(_path)                          \
  G_STMT_START                                          \
  {                                                     \
    if (g_file_test ((_path), G_FILE_TEST_IS_DIR))      \
      gx_reap_path ((_path));                           \
    else if (g_file_test ((_path), G_FILE_TEST_EXISTS)) \
      {                                                 \
        g_autoptr (GFile) file = NULL;                  \
                                                        \
        file = g_file_new_for_path ((_path));           \
        g_file_delete (file, NULL, NULL);               \
      }                                                 \
  }                                                     \
  G_STMT_END

#define gx_discard_module_dir()    \
  G_STMT_START                     \
  {                                \
    g_autofree char *_path = NULL; \
                                   \
    _path = gx_dup_module_dir ();  \
    gx_discard_path (_path);       \
  }                                \
  G_STMT_END

G_END_DECLS
