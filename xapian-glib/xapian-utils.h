/* Copyright 2014  Endless Mobile
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __XAPIAN_GLIB_UTILS_H__
#define __XAPIAN_GLIB_UTILS_H__

#include "xapian-glib-types.h"

G_BEGIN_DECLS

XAPIAN_GLIB_AVAILABLE_IN_ALL
const char *    xapian_glib_version_string      (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
int             xapian_glib_major_version       (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
int             xapian_glib_minor_version       (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
int             xapian_glib_revision            (void);

G_END_DECLS

#endif /* __XAPIAN_GLIB_UTILS_H__ */
