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

/**
 * SECTION:xapian-document
 * @Title: XapianDocument
 * @short_description: A document inside a database
 *
 * #XapianDocument is a class representing a document inside a
 * Xapian database.
 */

#include "config.h"

#include "xapian-document-private.h"

#include "xapian-error-private.h"

#define XAPIAN_DOCUMENT_GET_PRIVATE(obj) \
  ((XapianDocumentPrivate *) xapian_document_get_instance_private ((XapianDocument *) (obj)))

typedef struct {
  Xapian::Document *mDocument;
} XapianDocumentPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianDocument, xapian_document, G_TYPE_OBJECT)

static void
xapian_document_finalize (GObject *gobject)
{
  XapianDocumentPrivate *priv = XAPIAN_DOCUMENT_GET_PRIVATE (gobject);

  delete priv->mDocument;

  G_OBJECT_CLASS (xapian_document_parent_class)->dispose (gobject);
}

static void
xapian_document_class_init (XapianDocumentClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = xapian_document_finalize;
}

static void
xapian_document_init (XapianDocument *self)
{
}

/**
 * xapian_document_new:
 *
 * Creates a new, empty document.
 *
 * Returns: (transfer full): the newly created #XapianDocument
 */
XapianDocument *
xapian_document_new (void)
{
  return static_cast<XapianDocument *> (g_object_new (XAPIAN_TYPE_DOCUMENT, NULL));
}

/*< private >
 * xapian_document_new_from_document:
 * @aDoc: a `Xapian::Document`
 *
 * Creates a new #XapianDocument wrapper around a `Xapian::Document`
 * instance.
 *
 * Returns: (transfer full): the newly created #XapianDocument
 */
XapianDocument *
xapian_document_new_from_document (const Xapian::Document &aDoc)
{
  XapianDocument *res = static_cast<XapianDocument *> (g_object_new (XAPIAN_TYPE_DOCUMENT, NULL));
  XapianDocumentPrivate *priv = XAPIAN_DOCUMENT_GET_PRIVATE (res);

  priv->mDocument = new Xapian::Document (aDoc);

  return res;
}

/*< private >
 * xapian_document_get_internal:
 * @self: a #XapianDocument
 *
 * Retrieves the internal `Xapian::Document` instance associated
 * to @self.
 *
 * If @self has no internal `Xapian::Document` instance, a new
 * empty one will be created.
 *
 * Returns: (transfer none): a pointer to the internal `Xapian::Document`
 */
Xapian::Document *
xapian_document_get_internal (XapianDocument *self)
{
  XapianDocumentPrivate *priv = XAPIAN_DOCUMENT_GET_PRIVATE (self);

  /* if we got constructed without a document then we create an
   * empty one by default
   */
  if (!priv->mDocument)
    priv->mDocument = new Xapian::Document ();

  return priv->mDocument;
}

/**
 * xapian_document_get_value:
 * @document: a #XapianDocument
 * @slot: a slot number
 *
 * Retrieves the value associated to the @slot number inside @document.
 *
 * Returns: (transfer full): the value, or %NULL
 */
char *
xapian_document_get_value (XapianDocument *document,
                           unsigned int    slot)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string value = xapian_document_get_internal (document)->get_value (slot);

  return g_strdup (value.c_str ());
}

/**
 * xapian_document_add_value:
 * @document: a #XapianDocument
 * @slot: a slot number
 * @value: the value to associate to the @slot number
 *
 * Sets (or replaces) @value at the given @slot number inside
 * the @document.
 */
void
xapian_document_add_value (XapianDocument *document,
                           unsigned int    slot,
                           const char     *value)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->add_value (slot, std::string (value));
}

/**
 * xapian_document_remove_value:
 * @document: a #XapianDocument
 * @slot: a slot number
 *
 * Removes any value associated with the @slot number.
 */
void
xapian_document_remove_value (XapianDocument *document,
                              unsigned int    slot)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->remove_value (slot);
}

/**
 * xapian_document_clear_values:
 * @document: a #XapianDocument
 *
 * Removes all values from @document.
 */
void
xapian_document_clear_values (XapianDocument *document)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->clear_values ();
}

/**
 * xapian_document_get_data:
 * @document: a #XapianDocument
 *
 * Retrieves the content of @document.
 *
 * This is a potentially expensive operation, so it should not be
 * used when matching a document; typically, you should use the
 * values API (xapian_document_add_value() and xapian_document_get_value())
 * for fast access to matching terms or data.
 *
 * Returns: (transfer full): a string containing the contents
 *   of the document
 */
char *
xapian_document_get_data (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string data = xapian_document_get_internal (document)->get_data ();

  return g_strdup (data.c_str ());
}

/**
 * xapian_document_set_data:
 * @document: a #XapianDocument
 * @data: the content of the document
 *
 * Sets the contents of the @document.
 */
void
xapian_document_set_data (XapianDocument *document,
                          const char     *data)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->set_data (std::string (data));
}

/**
 * xapian_document_get_values_count:
 * @document: a #XapianDocument
 *
 * Retrieves the number of values in the @document.
 *
 * Returns: the number of values
 */
unsigned int
xapian_document_get_values_count (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->values_count ();
}

/**
 * xapian_document_get_doc_id:
 * @document: a #XapianDocument
 *
 * Retrieves the document id of @document.
 *
 * Returns: the document id
 */
unsigned int
xapian_document_get_doc_id (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->get_docid ();
}

/**
 * xapian_document_get_description:
 * @document: a #XapianDocument
 *
 * Retrieves a description of @document, typically used when
 * debugging.
 *
 * Returns: (transfer full): a description of the document
 */
char *
xapian_document_get_description (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string desc = xapian_document_get_internal (document)->get_description ();

  return g_strdup (desc.c_str ());
}

/**
 * xapian_document_add_posting:
 * @document: a #XapianDocument
 * @tname: the term to add
 * @term_pos: the position of @tname
 * @wdf_increment: the increment to be applied to the WDF
 *
 * Adds a term to the document, with positional information.
 *
 * See also: xapian_document_add_term(), xapian_document_add_term_full()
 */
void
xapian_document_add_posting (XapianDocument *document,
                             const char     *tname,
                             unsigned int    term_pos,
                             unsigned int    wdf_increment)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->add_posting (std::string (tname),
                                                        term_pos,
                                                        wdf_increment);
}

/**
 * xapian_document_add_term_full:
 * @document: a #XapianDocument
 * @tname: the term to add
 * @wdf_increment: the increment to be applied to the WDF
 *
 * Adds a term to the document, without positional information.
 */
void
xapian_document_add_term_full (XapianDocument *document,
                               const char     *tname,
                               unsigned int    wdf_increment)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->add_term (std::string (tname), wdf_increment);
}

/**
 * xapian_document_add_term:
 * @document: a #XapianDocument
 * @tname: the term to add
 *
 * Adds a term to the document, without positional information,
 * and with a default WDF increment of 1.
 */
void
xapian_document_add_term (XapianDocument *document,
                          const char     *tname)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->add_term (std::string (tname));
}

/**
 * xapian_document_add_boolean_term:
 * @document: a #XapianDocument
 * @tname: the term to add
 *
 * Adds a term to the document without incrementing the WDF.
 *
 * This function is the equivalent of xapian_document_add_term_full()
 * with a WDF increment set to 0.
 */
void
xapian_document_add_boolean_term (XapianDocument *document,
                                  const char     *tname)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->add_boolean_term (std::string (tname));
}

/**
 * xapian_document_remove_posting:
 * @document: a #XapianDocument
 * @tname: the term to remove
 * @term_pos: the position of the term
 * @wdf_decrement: the decrement to be applied to the WDF when
 *   removing the term
 *
 * Removes a posting of a term inside @document.
 *
 * Note that the term will still index the document, even if all the
 * occurrences have been removed. To completely remove @tname from
 * the @document, use xapian_document_remove_term() instead.
 */
void
xapian_document_remove_posting (XapianDocument *document,
                                const char     *tname,
                                unsigned int    term_pos,
                                unsigned int    wdf_decrement)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->remove_posting (std::string (tname),
                                                           term_pos,
                                                           wdf_decrement);
}

/**
 * xapian_document_remove_term:
 * @document: a #XapianDocument
 * @tname: the term to remove
 *
 * Removes @tname from the list of terms stored inside @document.
 */
void
xapian_document_remove_term (XapianDocument *document,
                             const char     *tname)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_get_internal (document)->remove_term (std::string (tname));
}

/**
 * xapian_document_clear_terms:
 * @document: a #XapianDocument
 *
 * Clears all the terms stored in the @document.
 */
void
xapian_document_clear_terms (XapianDocument *document)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->clear_terms ();
}

/**
 * xapian_document_get_termlist_count:
 * @document: a #XapianDocument
 *
 * Retrieves the number of terms registered in the @document.
 *
 * Returns: the number of terms
 */
unsigned int
xapian_document_get_termlist_count (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->termlist_count ();
}
