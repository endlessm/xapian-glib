/* Copyright 2014, 2018  Endless Mobile
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

#ifndef __XAPIAN_GLIB_QUERY_H__
#define __XAPIAN_GLIB_QUERY_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-posting-source.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_QUERY       (xapian_query_get_type())

XAPIAN_GLIB_AVAILABLE_IN_1_0
G_DECLARE_DERIVABLE_TYPE (XapianQuery, xapian_query, XAPIAN, QUERY, GObject)

struct _XapianQueryClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new                (void);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new_for_term       (const char    *term);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new_for_pair       (XapianQueryOp  op,
                                                 XapianQuery   *a,
                                                 XapianQuery   *b);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new_for_value      (XapianQueryOp  op,
                                                 unsigned int   slot,
                                                 const char    *value);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new_for_terms_pair (XapianQueryOp  op,
                                                 const char    *a,
                                                 const char    *b);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_query_new_from_string    (const char    *data);
XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianQuery *   xapian_query_new_from_posting_source (XapianPostingSource *posting_source);

XAPIAN_GLIB_AVAILABLE_IN_1_8
XapianQuery *   xapian_query_new_for_terms      (XapianQueryOp  op,
                                                 const char   **terms);
XAPIAN_GLIB_AVAILABLE_IN_1_8
XapianQuery *   xapian_query_new_for_queries    (XapianQueryOp  op,
                                                 GSList        *queries);

XAPIAN_GLIB_AVAILABLE_IN_1_8
XapianQuery *   xapian_query_new_wildcard       (const char    *pattern);

XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianQuery *   xapian_query_new_match_all      (void);

XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean        xapian_query_is_empty           (XapianQuery   *query);

XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int    xapian_query_get_length         (XapianQuery   *query);

XAPIAN_GLIB_AVAILABLE_IN_1_0
char *          xapian_query_get_description    (XapianQuery   *query);
XAPIAN_GLIB_AVAILABLE_IN_1_0
char *          xapian_query_serialise          (XapianQuery   *query);

G_END_DECLS

#endif /* __XAPIAN_GLIB_QUERY_H__ */
