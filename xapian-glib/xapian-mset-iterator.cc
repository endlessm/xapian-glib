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

#include <config.h>

#include "xapian-mset-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"

#include <xapian/iterator.h>

/**
 * SECTION:xapian-mset-iterator
 * @Title: XapianMSetIterator
 * @short_description: Iterate through a XapianMSet
 *
 * #XapianMSetIterator is a class that allows iterating through the items
 * inside a #XapianMSet.
 *
 * A typical example is:
 *
 * |[<!-- language="C" -->
 *   // retrieve the first 10 results
 *   XapianMSet *mset = xapian_enquire_get_mset (enquire, 0, 10, NULL);
 *
 *   unsigned int size = xapian_mset_get_size (mset);
 *
 *   XapianMSetIterator *iter = xapian_mset_get_begin (mset);
 *
 *   while (xapian_mset_iterator_next (iter))
 *     {
 *       g_print ("Result %u of %u: %.3f [docid=%u]",
 *                xapian_mset_iterator_get_rank (iter) + 1,
 *                size,
 *                xapian_mset_iterator_get_weight (iter),
 *                xapian_mset_iterator_get_doc_id (iter, NULL));
 *
 *       XapianDocument *doc = xapian_mset_iterator_get_document (iter, NULL);
 *
 *       // ... use the document
 *     }
 * ]|
 */

class IteratorData {
    IteratorData (const IteratorData &aData);

    void operator= (const IteratorData &aData);

  public:
    IteratorData ()
      : mMSet (NULL),
        mCurrentInitialized (false),
        mDocument (NULL)
    {
    }

    void set_mset (XapianMSet *aMset)
    {
      mCurrent = xapian_mset_get_internal (aMset)->begin ();

      mMSet = static_cast<XapianMSet *> (g_object_ref (aMset));
    }

    ~IteratorData () {
      if (mMSet)
        g_object_unref (mMSet);

      if (mDocument)
        g_object_unref (mDocument);
    }

    bool isBegin () {
      if (!mCurrentInitialized)
        return false;

      return mCurrent == xapian_mset_get_internal (mMSet)->begin ();
    }

    bool isEnd () {
      if (!mCurrentInitialized)
        return false;

      return !Xapian::iterator_valid (mCurrent);
    }

    bool next () {
      if (!mCurrentInitialized)
        {
          /* mCurrent was already initialised to the first element by the
           * constructor. */
          mCurrentInitialized = true;
        }
      else
        {
          g_clear_object (&mDocument);

          ++mCurrent;
        }

      return Xapian::iterator_valid (mCurrent);
    }

    bool prev () {
      if (mMSet == NULL)
        return FALSE;

      if (!mCurrentInitialized)
        {
          /* mCurrent was already initialised to the first element by the
           * constructor, so we just need to add on the size to point it
           * to just after the end.
           */
#if XAPIAN_MAJOR_VERSION == 1 && XAPIAN_MINOR_VERSION <= 3
          // MSet::operator+= was added in xapian-core 1.3.7 - to support older
          // versions we just loop and call operator++.  At least with g++ -O2
          // this won't be O(n) as the operator++ code inlines to ++index and
          // the compiler unrolls the loop and squashes this down to
          // effectively: if (size) index += size;
          Xapian::doccount size = xapian_mset_get_internal (mMSet)->size ();
          while (size--)
            ++mCurrent;
#else
          mCurrent += xapian_mset_get_internal (mMSet)->size ();
#endif
          mCurrentInitialized = true;
        }
      else
        {
          g_clear_object (&mDocument);

          --mCurrent;
        }

      return mCurrent != xapian_mset_get_internal (mMSet)->begin ();
    }

    unsigned int getRank () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_rank ();
    }

    double getWeight () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_weight ();
    }

    int getPercent () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_percent ();
    }

    XapianDocument * getDocument (GError **error) {
      if (!mCurrentInitialized)
        return NULL;

      if (mDocument)
        return mDocument;

      /* we cache the XapianDocument wrapper so that multiple invocations
       * will return the same GObject instance, instead of a different one
       * wrapping the same Xapian::Document. the field is cleared when the
       * iterator is advanced, or when it's cleared.
       */
      try
        {
          Xapian::Document realDoc = mCurrent.get_document ();

          mDocument = xapian_document_new_from_document (realDoc);
        }
      catch (const Xapian::Error &err)
        {
          GError *internal_error = NULL;

          xapian_error_to_gerror (err, &internal_error);
          g_propagate_error (error, internal_error);

          mDocument = NULL;
        }

      return mDocument;
    }

    unsigned int getDocId (GError **error) {
      XapianDocument *doc = getDocument (error);

      if (doc)
        return xapian_document_get_doc_id (doc);

      return 0;
    }

    unsigned int getCollapseCount () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_collapse_count ();
    }

    char * getDescription () {
      std::string desc = mCurrent.get_description ();

      return g_strdup (desc.c_str ());
    }

    XapianMSet * getMSet () {
      return mMSet;
    }

  private:
    XapianMSet *mMSet;

    Xapian::MSetIterator mCurrent;

    bool mCurrentInitialized;

    XapianDocument *mDocument;
};

typedef struct {
  IteratorData *data;
} XapianMSetIteratorPrivate;

#define XAPIAN_MSET_ITERATOR_GET_PRIVATE(obj) \
  ((XapianMSetIteratorPrivate *) xapian_mset_iterator_get_instance_private ((XapianMSetIterator *) (obj)))

G_DEFINE_TYPE_WITH_PRIVATE (XapianMSetIterator, xapian_mset_iterator, G_TYPE_OBJECT)

static void
xapian_mset_iterator_finalize (GObject *gobject)
{
  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (gobject);

  delete priv->data;

  G_OBJECT_CLASS (xapian_mset_iterator_parent_class)->finalize (gobject);
}

static void
xapian_mset_iterator_class_init (XapianMSetIteratorClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = xapian_mset_iterator_finalize;
}

static void
xapian_mset_iterator_init (XapianMSetIterator *self)
{
  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (self);
  priv->data = new IteratorData ();
}

XapianMSetIterator *
xapian_mset_iterator_new (XapianMSet *mset)
{
  XapianMSetIterator *iter;
  XapianMSetIteratorPrivate *priv;

  iter = static_cast<XapianMSetIterator *> (g_object_new (XAPIAN_TYPE_MSET_ITERATOR, NULL));
  priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);
  priv->data->set_mset (mset);

  return iter;
}

/**
 * xapian_mset_iterator_is_begin:
 * @iter: a #XapianMSetIterator
 *
 * Checks if @iter is at the start of the set.
 *
 * Returns: %TRUE if the iterator is at the start
 */
gboolean
xapian_mset_iterator_is_begin (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->isBegin ();
}

/**
 * xapian_mset_iterator_is_end:
 * @iter: a #XapianMSetIterator
 *
 * Checks if @iter is at the end of the set.
 *
 * Returns: %TRUE if the iterator is at the end
 */
gboolean
xapian_mset_iterator_is_end (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->isEnd ();
}

/**
 * xapian_mset_iterator_next:
 * @iter: a #XapianMSetIterator
 *
 * Advances the @iter forward.
 *
 * You should use the return value to check if @iter is still
 * valid after being advanced, before using the #XapianMSetIterator
 * API.
 *
 * Returns: %TRUE if the iterator was advanced, and %FALSE otherwise
 */
gboolean
xapian_mset_iterator_next (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->next ();
}

/**
 * xapian_mset_iterator_prev:
 * @iter: a #XapianMSetIterator
 *
 * Advances the @iter backward.
 *
 * You should use the return value to check if @iter is still
 * valid after being advanced, before using the #XapianMSetIterator
 * API.
 *
 * Returns: %TRUE if the iterator was advanced, and %FALSE otherwise
 */
gboolean
xapian_mset_iterator_prev (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->prev ();
}

/**
 * xapian_mset_iterator_get_rank:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves the rank of the current item pointed by @iter.
 *
 * The rank is the position of the item in the ordered list
 * of results. The rank starts from zero.
 *
 * Returns: the rank of the item
 */
unsigned int
xapian_mset_iterator_get_rank (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getRank ();
}

/**
 * xapian_mset_iterator_get_weight:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves the weight of the current item pointed by @iter.
 *
 * Returns: the weight of the item
 */
double
xapian_mset_iterator_get_weight (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getWeight ();
}

/**
 * xapian_mset_iterator_get_percent:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves the weight of the current item pointed by @iter
 * as a percentage between 0 and 100.
 *
 * Returns: the weight of the item as a percentage
 */
int
xapian_mset_iterator_get_percent (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getPercent ();
}

/**
 * xapian_mset_iterator_get_collapse_count:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves the estimated number of documents that have been
 * collapsed into the current item pointed by @iter.
 *
 * Returns: the number of collapsed documents
 */
unsigned int
xapian_mset_iterator_get_collapse_count (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getCollapseCount ();
}

/**
 * xapian_mset_iterator_get_description:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves a description of @iter, typically used for debugging.
 *
 * Returns: (transfer full): a description of the iterator
 */
char *
xapian_mset_iterator_get_description (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), NULL);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getDescription ();
}

/**
 * xapian_mset_iterator_get_document:
 * @iter: a #XapianMSetIterator
 * @error: return location for a #GError
 *
 * Retrieves the #XapianDocument currently pointed by the
 * iterator.
 *
 * Returns: (transfer none): a #XapianDocument
 */
XapianDocument *
xapian_mset_iterator_get_document (XapianMSetIterator *iter,
                                   GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), NULL);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getDocument (error);
}

/**
 * xapian_mset_iterator_get_doc_id:
 * @iter: a #XapianMSetIterator
 * @error: return location for a #GError
 *
 * Retrieves the id of the document currently pointed by the
 * iterator.
 *
 * Returns: a document id
 */
unsigned int
xapian_mset_iterator_get_doc_id (XapianMSetIterator *iter,
                                 GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getDocId (error);
}

/**
 * xapian_mset_iterator_get_mset:
 * @iter: a #XapianMSetIterator
 *
 * Retrieves the #XapianMSet that created the @iter.
 *
 * Returns: (transfer none): a #XapianMSetIterator
 */
XapianMSet *
xapian_mset_iterator_get_mset (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), NULL);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getMSet ();
}
