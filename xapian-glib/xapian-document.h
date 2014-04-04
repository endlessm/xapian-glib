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

#ifndef __XAPIAN_GLIB_DOCUMENT_H__
#define __XAPIAN_GLIB_DOCUMENT_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_DOCUMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_DOCUMENT, XapianDocument))
#define XAPIAN_IS_DOCUMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_DOCUMENT))
#define XAPIAN_DOCUMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_DOCUMENT, XapianDocumentClass))
#define XAPIAN_IS_DOCUMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_DOCUMENT))
#define XAPIAN_DOCUMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_DOCUMENT, XapianDocumentClass))

typedef struct _XapianDocumentClass     XapianDocumentClass;

struct _XapianDocument
{
  GObject parent_instance;
};

struct _XapianDocumentClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_document_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianDocument *        xapian_document_new             (void);

/* values */
XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_document_get_value       (XapianDocument *document,
                                                         unsigned int    slot);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_document_add_value       (XapianDocument *document,
                                                         unsigned int    slot,
                                                         const char     *value);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_document_remove_value    (XapianDocument *document,
                                                         unsigned int    slot);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_document_clear_values    (XapianDocument *document);

/* data */
XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_document_get_data        (XapianDocument *document);
XAPIAN_GLIB_AVAILABLE_IN_ALL
void                    xapian_document_set_data        (XapianDocument *document,
                                                         const char     *data);

XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_document_values_count    (XapianDocument *document);
XAPIAN_GLIB_AVAILABLE_IN_ALL
unsigned int            xapian_document_get_doc_id      (XapianDocument *document);

XAPIAN_GLIB_AVAILABLE_IN_ALL
char *                  xapian_document_get_description (XapianDocument *document);

G_END_DECLS

#endif /* __XAPIAN_GLIB_DATABASE_H__ */
