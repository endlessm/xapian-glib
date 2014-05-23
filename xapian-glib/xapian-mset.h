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

#ifndef __XAPIAN_GLIB_MSET_H__
#define __XAPIAN_GLIB_MSET_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

/* MSet */

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_mset_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianMSet *            xapian_mset_ref                                         (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_mset_unref                                       (XapianMSet *mset);

XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_firstitem                               (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_matches_lower_bound                     (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_matches_estimated                       (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_matches_upper_bound                     (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_uncollapsed_matches_lower_bound         (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_uncollapsed_matches_estimated           (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_uncollapsed_matches_upper_bound         (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
double                  xapian_mset_get_max_possible                            (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
double                  xapian_mset_get_max_attained                            (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_get_size                                    (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_is_empty                                    (XapianMSet *mset);

/* Iterator */

#define XAPIAN_MSET_ITERATOR_INIT       { NULL }

struct _XapianMSetIterator
{
  /*< private >*/
  gpointer data;
};

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_mset_iterator_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_mset_iterator_init               (XapianMSetIterator *iter,
                                                                 XapianMSet         *mset);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_mset_iterator_clear              (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_iterator_next               (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_iterator_prev               (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_iterator_is_valid           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_iterator_is_begin           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
gboolean                xapian_mset_iterator_is_end             (XapianMSetIterator *iter);

XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_iterator_get_rank           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
int                     xapian_mset_iterator_get_percent        (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_mset_iterator_get_docid          (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianDocument *        xapian_mset_iterator_get_document       (XapianMSetIterator *iter,
                                                                 GError            **error);

G_END_DECLS

#endif /* __XAPIAN_GLIB_MSET_H__ */
