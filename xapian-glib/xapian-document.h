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

#define XAPIAN_TYPE_DOCUMENT    (xapian_document_get_type())

XAPIAN_GLIB_AVAILABLE_IN_2_0
G_DECLARE_DERIVABLE_TYPE (XapianDocument, xapian_document, XAPIAN, DOCUMENT, GObject)

struct _XapianDocumentClass
{
  /*< private >*/
  GObjectClass parent_class;
};

XAPIAN_GLIB_AVAILABLE_IN_2_0
XapianDocument *        xapian_document_new                     (void);

/* values */
XAPIAN_GLIB_AVAILABLE_IN_2_0
char *                  xapian_document_get_value               (XapianDocument *document,
                                                                 unsigned int    slot);
XAPIAN_GLIB_AVAILABLE_IN_2_0
double                  xapian_document_get_numeric_value       (XapianDocument *document,
                                                                 unsigned int    slot);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_value               (XapianDocument *document,
                                                                 unsigned int    slot,
                                                                 const char     *value);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_numeric_value       (XapianDocument *document,
                                                                 unsigned int    slot,
                                                                 double          value);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_remove_value            (XapianDocument *document,
                                                                 unsigned int    slot);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_clear_values            (XapianDocument *document);
XAPIAN_GLIB_AVAILABLE_IN_2_0
unsigned int            xapian_document_get_values_count        (XapianDocument *document);

/* data */
XAPIAN_GLIB_AVAILABLE_IN_2_0
char *                  xapian_document_get_data                (XapianDocument *document);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_set_data                (XapianDocument *document,
                                                                 const char     *data);

XAPIAN_GLIB_AVAILABLE_IN_2_0
unsigned int            xapian_document_get_doc_id              (XapianDocument *document);

XAPIAN_GLIB_AVAILABLE_IN_2_0
char *                  xapian_document_get_description         (XapianDocument *document);

/* terms */
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_posting             (XapianDocument *document,
                                                                 const char     *tname,
                                                                 unsigned int    term_pos,
                                                                 unsigned int    wdf_increment);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_remove_posting          (XapianDocument *document,
                                                                 const char     *tname,
                                                                 unsigned int    term_pos,
                                                                 unsigned int    wdf_decrement);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_term                (XapianDocument *document,
                                                                 const char     *tname);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_term_full           (XapianDocument *document,
                                                                 const char     *tname,
                                                                 unsigned int    wdf_increment);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_add_boolean_term        (XapianDocument *document,
                                                                 const char     *tname);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_remove_term             (XapianDocument *document,
                                                                 const char     *tname);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_document_clear_terms             (XapianDocument *document);
XAPIAN_GLIB_AVAILABLE_IN_2_0
unsigned int            xapian_document_get_termlist_count      (XapianDocument *document);


G_END_DECLS

#endif /* __XAPIAN_GLIB_DATABASE_H__ */
