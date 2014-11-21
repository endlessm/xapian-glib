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

#ifndef __XAPIAN_GLIB_WRITABLE_DATABASE_H__
#define __XAPIAN_GLIB_WRITABLE_DATABASE_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-database.h"

G_BEGIN_DECLS

#define XAPIAN_WRITABLE_DATABASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_WRITABLE_DATABASE, XapianWritableDatabase))
#define XAPIAN_IS_WRITABLE_DATABASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_WRITABLE_DATABASE))
#define XAPIAN_WRITABLE_DATABASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_WRITABLE_DATABASE, XapianWritableDatabaseClass))
#define XAPIAN_IS_WRITABLE_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_WRITABLE_DATABASE))
#define XAPIAN_WRITABLE_DATABASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_WRITABLE_DATABASE, XapianWritableDatabaseClass))

typedef struct _XapianWritableDatabaseClass     XapianWritableDatabaseClass;

struct _XapianWritableDatabase
{
  XapianDatabase parent_instance;
};

struct _XapianWritableDatabaseClass
{
  XapianDatabaseClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_writable_database_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianWritableDatabase *        xapian_writable_database_new                    (const char            *path,
                                                                                 XapianDatabaseAction   action,
                                                                                 GError               **error);

XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_commit                 (XapianWritableDatabase *self,
                                                                                 GError **error);

XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_begin_transaction      (XapianWritableDatabase *self,
                                                                                 gboolean                flushed,
                                                                                 GError                **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_commit_transaction     (XapianWritableDatabase *self,
                                                                                 GError                **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_cancel_transaction     (XapianWritableDatabase *self,
                                                                                 GError                **error);

XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_add_document           (XapianWritableDatabase *self,
                                                                                 XapianDocument         *document,
                                                                                 unsigned int           *docid_out,
                                                                                 GError                **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_delete_document        (XapianWritableDatabase *self,
                                                                                 unsigned int            docid,
                                                                                 GError                **error);
XAPIAN_GLIB_AVAILABLE_IN_1_0
gboolean                        xapian_writable_database_replace_document       (XapianWritableDatabase *self,
                                                                                 unsigned int            docid,
                                                                                 XapianDocument         *document,
                                                                                 GError                **error);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void                            xapian_writable_database_add_spelling           (XapianWritableDatabase *self,
                                                                                 const char             *word);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void                            xapian_writable_database_add_spelling_full      (XapianWritableDatabase *self,
                                                                                 const char             *word,
                                                                                 unsigned int            freqinc);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void                            xapian_writable_database_remove_spelling        (XapianWritableDatabase *self,
                                                                                 const char             *word);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void                            xapian_writable_database_remove_spelling_full   (XapianWritableDatabase *self,
                                                                                 const char             *word,
                                                                                 unsigned int            freqdec);

G_END_DECLS

#endif /* __XAPIAN_GLIB_WRITABLE_DATABASE_H__ */
