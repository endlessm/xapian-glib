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

#ifndef __XAPIAN_GLIB_TERM_ITERATOR_H__
#define __XAPIAN_GLIB_TERM_ITERATOR_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_TERM_ITERATOR               (xapian_term_iterator_get_type ())

XAPIAN_GLIB_AVAILABLE_IN_2_0
G_DECLARE_DERIVABLE_TYPE (XapianTermIterator, xapian_term_iterator, XAPIAN, TERM_ITERATOR, GObject)

struct _XapianTermIteratorClass
{
  /*< private >*/
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_2_0
gboolean                xapian_term_iterator_next               (XapianTermIterator *iter);

XAPIAN_GLIB_AVAILABLE_IN_2_0
char *                  xapian_term_iterator_get_term_name      (XapianTermIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_2_0
unsigned int            xapian_term_iterator_get_wdf            (XapianTermIterator *iter,
                                                                 GError            **error);
XAPIAN_GLIB_AVAILABLE_IN_2_0
gboolean                xapian_term_iterator_get_term_freq      (XapianTermIterator *iter,
                                                                 unsigned int       *res,
                                                                 GError            **error);
XAPIAN_GLIB_AVAILABLE_IN_2_0
char *                  xapian_term_iterator_get_description    (XapianTermIterator *iter);

G_END_DECLS

#endif /* __XAPIAN_GLIB_TERM_ITERATOR_H__ */
