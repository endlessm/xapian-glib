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
xapian_document_dispose (GObject *gobject)
{
  XapianDocumentPrivate *priv = XAPIAN_DOCUMENT_GET_PRIVATE (gobject);

  if (priv->mDocument)
    {
      delete priv->mDocument;
      priv->mDocument = NULL;
    }

  G_OBJECT_CLASS (xapian_document_parent_class)->dispose (gobject);
}

static void
xapian_document_class_init (XapianDocumentClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = xapian_document_dispose;
}

static void
xapian_document_init (XapianDocument *self)
{
}

XapianDocument *
xapian_document_new (void)
{
  return static_cast<XapianDocument *> (g_object_new (XAPIAN_TYPE_DOCUMENT, NULL));
}

XapianDocument *
xapian_document_new_from_document (const Xapian::Document &aDoc)
{
  XapianDocument *res = static_cast<XapianDocument *> (g_object_new (XAPIAN_TYPE_DOCUMENT, NULL));
  XapianDocumentPrivate *priv = XAPIAN_DOCUMENT_GET_PRIVATE (res);

  priv->mDocument = new Xapian::Document (aDoc);
}

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

char *
xapian_document_get_value (XapianDocument *document,
                           unsigned int    slot)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string value = xapian_document_get_internal (document)->get_value (slot);

  return g_strdup (value.c_str ());
}

void
xapian_document_add_value (XapianDocument *document,
                           unsigned int    slot,
                           const char     *value)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  std::string tmp (value);

  xapian_document_get_internal (document)->add_value (slot, tmp);
}

void
xapian_document_remove_value (XapianDocument *document,
                              unsigned int    slot)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->remove_value (slot);
}

void
xapian_document_clear_values (XapianDocument *document)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->clear_values ();
}

/**
 * xapian_document_get_data:
 * @document: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_document_get_data (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string data = xapian_document_get_internal (document)->get_data ();

  return g_strdup (data.c_str ());
}

void
xapian_document_set_data (XapianDocument *document,
                          const char     *data)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  std::string tmp (data);

  xapian_document_get_internal (document)->set_data (tmp);
}

unsigned int
xapian_document_get_values_count (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->values_count ();
}

unsigned int
xapian_document_get_doc_id (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->get_docid ();
}

/**
 * xapian_document_get_description:
 * @document: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_document_get_description (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), NULL);

  std::string desc = xapian_document_get_internal (document)->get_description ();

  return g_strdup (desc.c_str ());
}

void
xapian_document_add_posting (XapianDocument *document,
                             const char     *tname,
                             unsigned int    term_pos,
                             unsigned int    wdf_increment)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  std::string term (tname);

  xapian_document_get_internal (document)->add_posting (term, term_pos, wdf_increment);
}

void
xapian_document_add_term (XapianDocument *document,
                          const char     *tname,
                          unsigned int    wdf_increment)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  std::string term (tname);

  xapian_document_get_internal (document)->add_term (term, wdf_increment);
}

void
xapian_document_add_boolean_term (XapianDocument *document,
                                  const char     *tname)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  xapian_document_add_term (document, tname, 0);
}

void
xapian_document_remove_posting (XapianDocument *document,
                                const char     *tname,
                                unsigned int    term_pos,
                                unsigned int    wdf_decrement)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  std::string term (tname);

  xapian_document_get_internal (document)->remove_posting (term, term_pos, wdf_decrement);
}

void
xapian_document_remove_term (XapianDocument *document,
                             const char     *tname)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));
  g_return_if_fail (tname != NULL);

  std::string term (tname);

  xapian_document_get_internal (document)->remove_term (term);
}

void
xapian_document_clear_terms (XapianDocument *document)
{
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  xapian_document_get_internal (document)->clear_terms ();
}

unsigned int
xapian_document_get_termlist_count (XapianDocument *document)
{
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), 0);

  return xapian_document_get_internal (document)->termlist_count ();
}
