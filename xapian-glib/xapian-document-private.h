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

#ifndef __XAPIAN_GLIB_DOCUMENT_PRIVATE_H__
#define __XAPIAN_GLIB_DOCUMENT_PRIVATE_H__

#include <xapian.h>

#include "xapian-document.h"

XapianDocument *        xapian_document_new_from_document       (const Xapian::Document &aDoc);

Xapian::Document *      xapian_document_get_internal            (XapianDocument         *doc);

#endif /* __XAPIAN_GLIB_DOCUMENT_PRIVATE_H__ */
