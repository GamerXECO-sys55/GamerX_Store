/* bz-section-view.h
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

#include "gx-content-section.h"

G_BEGIN_DECLS

#define GX_TYPE_SECTION_VIEW (gx_section_view_get_type ())
G_DECLARE_FINAL_TYPE (GxSectionView, gx_section_view, GX, SECTION_VIEW, AdwBin)

GtkWidget *
gx_section_view_new (GxContentSection *section);

void
gx_section_view_set_section (GxSectionView    *self,
                             GxContentSection *section);

GxContentSection *
gx_section_view_get_section (GxSectionView *self);

G_END_DECLS
