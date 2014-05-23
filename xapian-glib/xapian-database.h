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

#ifndef __XAPIAN_GLIB_DATABASE_H__
#define __XAPIAN_GLIB_DATABASE_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_DATABASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_DATABASE, XapianDatabase))
#define XAPIAN_IS_DATABASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_DATABASE))
#define XAPIAN_DATABASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_DATABASE, XapianDatabaseClass))
#define XAPIAN_IS_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_DATABASE))
#define XAPIAN_DATABASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_DATABASE, XapianDatabaseClass))

typedef struct _XapianDatabaseClass     XapianDatabaseClass;

struct _XapianDatabase
{
  GObject parent_instance;
};

struct _XapianDatabaseClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_database_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianDatabase *        xapian_database_new             (GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianDatabase *        xapian_database_new_with_path   (const char     *path,
                                                         GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_database_close           (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_database_get_description (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_database_get_uuid        (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_database_get_metadata    (XapianDatabase *db,
                                                         const char     *key,
                                                         GError        **error);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_database_get_doc_count   (XapianDatabase *db);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_database_get_last_doc_id (XapianDatabase *db);

XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianDocument *        xapian_database_get_document    (XapianDatabase *db,
                                                         unsigned int    docid,
                                                         GError        **error);

XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_database_add_database    (XapianDatabase *db,
                                                         XapianDatabase *new_db);

G_END_DECLS

#endif /* __XAPIAN_GLIB_DATABASE_H__ */
