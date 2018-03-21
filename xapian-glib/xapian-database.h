/* Copyright 2014  Endless Mobile
 * Copyright 2017  Olly Betts
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

#ifndef __XAPIAN_GLIB_DATABASE_H__
#define __XAPIAN_GLIB_DATABASE_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-document.h"
#include "xapian-term-iterator.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_DATABASE    (xapian_database_get_type())

XAPIAN_GLIB_AVAILABLE_IN_1_0
G_DECLARE_DERIVABLE_TYPE (XapianDatabase, xapian_database, XAPIAN, DATABASE, GObject)

struct _XapianDatabaseClass
{
  /*< private >*/
  GObjectClass parent_class;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianDatabase *        xapian_database_new             (GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianDatabase *        xapian_database_new_with_path   (const char     *path,
                                                         GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_database_close           (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_1_2
void                    xapian_database_reopen          (XapianDatabase *db);

XAPIAN_GLIB_AVAILABLE_IN_1_0
char *                  xapian_database_get_description (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_1_0
char *                  xapian_database_get_uuid        (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_1_0
char *                  xapian_database_get_metadata    (XapianDatabase *db,
                                                         const char     *key,
                                                         GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_database_get_doc_count   (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_1_0
unsigned int            xapian_database_get_last_doc_id (XapianDatabase *db);

XAPIAN_GLIB_AVAILABLE_IN_1_4
double                  xapian_database_get_average_length (XapianDatabase *db);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianDocument *        xapian_database_get_document    (XapianDatabase *db,
                                                         unsigned int    docid,
                                                         GError        **error);

XAPIAN_GLIB_AVAILABLE_IN_1_4
unsigned int            xapian_database_get_term_freq   (XapianDatabase *db,
                                                         const char     *term);
XAPIAN_GLIB_AVAILABLE_IN_1_4
unsigned int            xapian_database_get_collection_freq (XapianDatabase *db,
                                                             const char     *term);


XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_database_add_database    (XapianDatabase *db,
                                                         XapianDatabase *new_db);

XAPIAN_GLIB_AVAILABLE_IN_1_4
gboolean                xapian_database_compact_to_path (XapianDatabase             *self,
                                                         const char                 *path,
                                                         XapianDatabaseCompactFlags  flags,
                                                         GError                    **error);

XAPIAN_GLIB_AVAILABLE_IN_1_4
gboolean                xapian_database_compact_to_fd   (XapianDatabase             *self,
                                                         int                         fd,
                                                         XapianDatabaseCompactFlags  flags,
                                                         GError                    **error);

XAPIAN_GLIB_AVAILABLE_IN_1_4
XapianTermIterator *    xapian_database_enumerate_all_terms (XapianDatabase *self,
                                                             const char     *prefix);

G_END_DECLS

#endif /* __XAPIAN_GLIB_DATABASE_H__ */
