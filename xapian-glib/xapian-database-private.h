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

#ifndef __XAPIAN_GLIB_DATABASE_PRIVATE_H__
#define __XAPIAN_GLIB_DATABASE_PRIVATE_H__

#include <xapian.h>
#include <glib.h>
#include "xapian-database.h"

Xapian::Database *      xapian_database_get_internal    (XapianDatabase   *self);
void			xapian_database_set_internal    (XapianDatabase   *self,
							 Xapian::Database *aDB);
void			xapian_database_set_is_writable	(XapianDatabase   *self,
							 gboolean          is_writable);
gboolean                xapian_database_get_is_writable (XapianDatabase   *self);
const char *            xapian_database_get_path        (XapianDatabase   *self);


#endif /* __XAPIAN_GLIB_DATABASE_PRIVATE_H__ */
