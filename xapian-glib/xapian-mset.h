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
#include "xapian-document.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_MSET                (xapian_mset_get_type())
#define XAPIAN_TYPE_MSET_ITERATOR       (xapian_mset_iterator_get_type())

XAPIAN_GLIB_AVAILABLE_IN_1_0
G_DECLARE_DERIVABLE_TYPE (XapianMSet, xapian_mset, XAPIAN, MSET, GObject)

XAPIAN_GLIB_AVAILABLE_IN_1_0
G_DECLARE_DERIVABLE_TYPE (XapianMSetIterator, xapian_mset_iterator, XAPIAN, MSET_ITERATOR, GObject)

/* MSet */

struct _XapianMSetClass
{
  /*< private >*/
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_firstitem                               (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_termfreq                                (XapianMSet *mset,
                                                                                 const char *term);
XAPIAN_GLIB_AVAILABLE_IN_1_0
double                  xapian_mset_get_termweight                              (XapianMSet *mset,
                                                                                 const char *term);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_matches_lower_bound                     (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_matches_estimated                       (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_matches_upper_bound                     (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_uncollapsed_matches_lower_bound         (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_uncollapsed_matches_estimated           (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_uncollapsed_matches_upper_bound         (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
double                  xapian_mset_get_max_possible                            (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
double                  xapian_mset_get_max_attained                            (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_get_size                                    (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                xapian_mset_is_empty                                    (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
int                     xapian_mset_convert_to_percent                          (XapianMSet *mset,
                                                                                 double      weight);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianMSetIterator *    xapian_mset_get_begin                                   (XapianMSet *mset);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianMSetIterator *    xapian_mset_get_end                                     (XapianMSet *mset);

/* Iterator */

struct _XapianMSetIteratorClass
{
  /*< private >*/
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                xapian_mset_iterator_next               (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                xapian_mset_iterator_prev               (XapianMSetIterator *iter);
XAPIAN_GLIB_DEPRECATED_IN_1_6
gboolean                xapian_mset_iterator_is_valid           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                xapian_mset_iterator_is_begin           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                xapian_mset_iterator_is_end             (XapianMSetIterator *iter);

XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_iterator_get_rank           (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
double                  xapian_mset_iterator_get_weight         (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
int                     xapian_mset_iterator_get_percent        (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianDocument *        xapian_mset_iterator_get_document       (XapianMSetIterator *iter,
                                                                 GError            **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_iterator_get_doc_id         (XapianMSetIterator *iter,
                                                                 GError            **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_mset_iterator_get_collapse_count (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
char *                  xapian_mset_iterator_get_description    (XapianMSetIterator *iter);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianMSet *            xapian_mset_iterator_get_mset           (XapianMSetIterator *iter);

G_END_DECLS

#endif /* __XAPIAN_GLIB_MSET_H__ */
