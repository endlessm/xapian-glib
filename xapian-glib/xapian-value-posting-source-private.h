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

#ifndef __XAPIAN_GLIB_VALUE_POSTING_SOURCE_PRIVATE_H__
#define __XAPIAN_GLIB_VALUE_POSTING_SOURCE_PRIVATE_H__

#include <xapian.h>
#include <glib.h>
#include "xapian-value-posting-source.h"

Xapian::ValuePostingSource *      xapian_value_posting_source_get_internal    (XapianValuePostingSource   *self);

void                              xapian_value_posting_source_set_internal    (XapianValuePostingSource   *self,
                                                                               Xapian::ValuePostingSource *aValuePostingSource);

#endif /* __XAPIAN_GLIB_POSTING_SOURCE_PRIVATE_H__ */