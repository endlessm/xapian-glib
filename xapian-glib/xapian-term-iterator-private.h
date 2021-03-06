/* Copyright 2017  Olly Betts
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

#ifndef __XAPIAN_GLIB_TERM_ITERATOR_PRIVATE_H__
#define __XAPIAN_GLIB_TERM_ITERATOR_PRIVATE_H__

#include <xapian.h>
#include "xapian-term-iterator.h"

XapianTermIterator *    xapian_term_iterator_new        (const Xapian::TermIterator &it);

#endif /* __XAPIAN_GLIB_TERM_ITERATOR_PRIVATE_H__ */
