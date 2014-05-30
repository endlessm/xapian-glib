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

/* XapianMSet {{{ */

/**
 * SECTION:xapian-mset
 * @Title: XapianMSet
 * @Short_Desc: Set of matching documents
 *
 * #XapianMSet represents a set of documents in a #XapianDatabase
 * matching a specific #XapianQuery performed by a #XapianEnquire
 * instance.
 *
 * You can query the whole set for information on the results, but
 * typically you will iterate over the #XapianMSet using an instance
 * of the #XapianMSetIterator class.
 */

#define XAPIAN_MSET_GET_PRIVATE(obj) \
  ((XapianMSetPrivate *) xapian_mset_get_instance_private ((XapianMSet *) (obj)))

typedef struct {
  Xapian::MSet *mSet;
} XapianMSetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianMSet, xapian_mset, G_TYPE_OBJECT)

static void
xapian_mset_class_finalize (GObject *gobject)
{
  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (gobject);

  delete priv->mSet;

  G_OBJECT_CLASS (xapian_mset_parent_class)->finalize (gobject);
}

static void
xapian_mset_class_init (XapianMSetClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = xapian_mset_class_finalize;
}

static void
xapian_mset_init (XapianMSet *self)
{
}

/*< private >
 * xapian_mset_new:
 * @aMSet: a Xapian::MSet
 *
 * Creates a new #XapianMSet for the given `Xapian::MSet` instance.
 *
 * Returns: (transfer full): the newly created #XapianMSet
 */
XapianMSet *
xapian_mset_new (const Xapian::MSet &aMSet)
{
  XapianMSet *res = static_cast<XapianMSet *> (g_object_new (XAPIAN_TYPE_MSET, NULL));

  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (res);
  priv->mSet = new Xapian::MSet (aMSet);

  return res;
}

/*< private >
 * xapian_mset_get_internal:
 * @mset: a #XapianMSet
 *
 * Retrieves the internal `Xapian::MSet` instance.
 *
 * Returns: (transfer none): a pointer to the internal instance
 */
Xapian::MSet *
xapian_mset_get_internal (XapianMSet *mset)
{
  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (mset);

  if (!priv->mSet)
    priv->mSet = new Xapian::MSet ();

  return priv->mSet;
}

/**
 * xapian_mset_get_termfreq:
 * @mset: a #XapianMSet
 * @term: a term
 *
 * Retrieves the frequency of the given @term.
 *
 * Return value: the frequence of the term
 */
unsigned int
xapian_mset_get_termfreq (XapianMSet *mset,
                          const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  try
    {
      return xapian_mset_get_internal (mset)->get_termfreq (std::string (term));
    }
  catch (const Xapian::InvalidOperationError &err)
    {
      return 0;
    }
}

/**
 * xapian_mset_get_termweight:
 * @mset: a #XapianMSet
 * @term: the term to use
 *
 * Retrieves the weight of the given @term.
 *
 * Returns: the weight of the term
 */
double
xapian_mset_get_termweight (XapianMSet *mset,
                            const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  try
    {
      return xapian_mset_get_internal (mset)->get_termweight (std::string (term));
    }
  catch (const Xapian::Error &err)
    {
      return 0;
    }
}

/**
 * xapian_mset_get_firstitem:
 * @mset: a #XapianMSet
 *
 * Retrieves the index of the first item in the @mset.
 *
 * Returns: the index of the first item
 */
unsigned int
xapian_mset_get_firstitem (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_firstitem ();
}

/**
 * xapian_mset_get_matches_lower_bound:
 * @mset: a #XapianMSet
 *
 * Retrieves the lower bound of the matching documents in the database.
 *
 * Returns: a lower bound of documents
 */
unsigned int
xapian_mset_get_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_matches_lower_bound ();
}

/**
 * xapian_mset_get_matches_estimated:
 * @mset: a #XapianMSet
 *
 * Retrieves an estimation of the matching documents in the database.
 *
 * Returns: an estimated number of documents
 */
unsigned int
xapian_mset_get_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_matches_estimated ();
}

/**
 * xapian_mset_get_matches_upper_bound:
 * @mset: a #XapianMSet
 *
 * Retrieves the upper bound of the matching documents in the database.
 *
 * Returns: an upper bound of documents
 */
unsigned int
xapian_mset_get_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_matches_upper_bound ();
}

/**
 * xapian_mset_get_uncollapsed_matches_lower_bound:
 * @mset: a #XapianMSet
 *
 * Retrieves the lower bound of the matching documents in the database if
 * collapsing was not used.
 *
 * Returns: a lower bound of documents
 */
unsigned int
xapian_mset_get_uncollapsed_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_uncollapsed_matches_lower_bound ();
}

/**
 * xapian_mset_get_uncollapsed_matches_estimated:
 * @mset: a #XapianMSet
 *
 * Retrieves an estimated number of matching documents in the database if
 * collapsing was not used.
 *
 * Returns: an estimate of matching documents
 */
unsigned int
xapian_mset_get_uncollapsed_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_uncollapsed_matches_estimated ();
}

/**
 * xapian_mset_get_uncollapsed_matches_upper_bound:
 * @mset: a #XapianMSet
 *
 * Retrieves the upper bound of the matching documents in the database if
 * collapsing was not used.
 *
 * Returns: an upper bound of matching documents
 */
unsigned int
xapian_mset_get_uncollapsed_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_uncollapsed_matches_upper_bound ();
}

/**
 * xapian_mset_get_max_possible:
 * @mset: a #XapianMSet
 *
 * The maximum possible weight in the @mset.
 *
 * See also: xapian_mset_get_max_attained()
 *
 * Returns: the maximum weight in the set
 */
double
xapian_mset_get_max_possible (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_max_possible ();
}

/**
 * xapian_mset_get_max_attained:
 * @mset: a #XapianMSet
 *
 * The greatest weight which is attained by any document in the
 * database.
 *
 * If @mset was the result of xapian_enquire_get_mset() with a
 * first item set to 0, the returned value is the weight of the
 * first item in the @mset.
 *
 * Returns: the maximum weight in the database
 */
double
xapian_mset_get_max_attained (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->get_max_attained ();
}

/**
 * xapian_mset_get_size:
 * @mset: a #XapianMSet
 *
 * Retrieves the number of items in the @mset.
 *
 * Returns: the number of items
 */
unsigned int
xapian_mset_get_size (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return xapian_mset_get_internal (mset)->size ();
}

/**
 * xapian_mset_is_empty:
 * @mset: a #XapianMSet
 *
 * Checks whether the @mset is empty.
 *
 * Returns: %TRUE if the set is empty, and %FALSE otherwise
 */
gboolean
xapian_mset_is_empty (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, TRUE);

  return xapian_mset_get_internal (mset)->empty ();
}

/* }}} XapianMSet */

/* XapianMSetIterator {{{ */

class IteratorData {
  public:
    IteratorData (const IteratorData &aData) {
      if (aData.mMSet)
        mMSet = static_cast<XapianMSet *> (g_object_ref (aData.mMSet));
      else
        mMSet = NULL;

      mBegin = new Xapian::MSetIterator (*aData.mBegin);
      mEnd = new Xapian::MSetIterator (*aData.mEnd);

      mCurrentInitialized = aData.mCurrentInitialized;
      if (mCurrentInitialized)
        mCurrent = new Xapian::MSetIterator (*aData.mCurrent);
      else
        mCurrent = NULL;

      if (aData.mDocument)
        mDocument = static_cast<XapianDocument *> (g_object_ref (aData.mDocument));
      else
        mDocument = NULL;
    }

    IteratorData (XapianMSet *aMset) {
      mMSet = static_cast<XapianMSet *> (g_object_ref (aMset));

      Xapian::MSet *realMSet = xapian_mset_get_internal (aMset);

      mBegin = new Xapian::MSetIterator (realMSet->begin ());
      mEnd = new Xapian::MSetIterator (realMSet->end ());
      mCurrent = NULL;
      mCurrentInitialized = false;

      mDocument = NULL;
    }

    ~IteratorData () {
      delete mBegin;
      delete mEnd;
      delete mCurrent;

      if (mMSet)
        g_object_unref (mMSet);

      if (mDocument)
        g_object_unref (mDocument);
    }

    bool isBegin () {
      if (!mCurrentInitialized)
        return false;

      return mCurrent == mBegin || *mCurrent == *mBegin;
    }

    bool isEnd () {
      if (!mCurrentInitialized)
        return false;

      return mCurrent == mEnd || *mCurrent == *mEnd;
    }

    bool next () {
      if (!mCurrentInitialized)
        {
          mCurrent = new Xapian::MSetIterator (*mBegin);
          mCurrentInitialized = true;
          return *mCurrent != *mEnd;
        }

      g_clear_object (&mDocument);

      ++(*mCurrent);

      if (*mCurrent == *mEnd)
        return false;

      return true;
    }

    bool prev () {
      if (!mCurrentInitialized)
        {
          mCurrent = new Xapian::MSetIterator (*mEnd);
          mCurrentInitialized = true;
          return *mCurrent != *mBegin;
        }

      g_clear_object (&mDocument);

      --(*mCurrent);

      if (*mCurrent == *mBegin)
        return false;

      return true;
    }

    unsigned int getRank () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent->get_rank ();
    }

    double getWeight () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent->get_weight ();
    }

    double getPercent () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent->get_percent ();
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
          Xapian::Document realDoc = mCurrent->get_document ();

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

    XapianMSet * getMSet () {
      return mMSet;
    }

  private:
    XapianMSet *mMSet;

    Xapian::MSetIterator *mBegin;
    Xapian::MSetIterator *mEnd;
    Xapian::MSetIterator *mCurrent;

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
}

/**
 * xapian_mset_get_begin:
 * @mset: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
XapianMSetIterator *
xapian_mset_get_begin (XapianMSet *mset)
{
  XapianMSetIterator *iter;
  XapianMSetIteratorPrivate *priv;

  iter = static_cast<XapianMSetIterator *> (g_object_new (XAPIAN_TYPE_MSET_ITERATOR, NULL));
  priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);
  priv->data = new IteratorData (mset);

  return iter;
}

/**
 * xapian_mset_get_end:
 * @mset: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
XapianMSetIterator *
xapian_mset_get_end (XapianMSet *mset)
{
  XapianMSetIterator *iter;
  XapianMSetIteratorPrivate *priv;

  iter = static_cast<XapianMSetIterator *> (g_object_new (XAPIAN_TYPE_MSET_ITERATOR, NULL));
  priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);
  priv->data = new IteratorData (mset);

  return iter;
}

gboolean
xapian_mset_iterator_is_valid (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  return XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter)->data != NULL;
}

gboolean
xapian_mset_iterator_is_begin (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return FALSE;

  return priv->data->isBegin ();
}

gboolean
xapian_mset_iterator_is_end (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return FALSE;

  return priv->data->isEnd ();
}

gboolean
xapian_mset_iterator_next (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return FALSE;

  return priv->data->next ();
}

gboolean
xapian_mset_iterator_prev (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), FALSE);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return FALSE;

  return priv->data->prev ();
}

unsigned int
xapian_mset_iterator_get_rank (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return 0;

  return priv->data->getRank ();
}

double
xapian_mset_iterator_get_weight (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return 0;

  return priv->data->getWeight ();
}

int
xapian_mset_iterator_get_percent (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return 0;

  return priv->data->getPercent ();
}

/**
 * xapian_mset_iterator_get_document:
 * @iter: ...
 * @error: ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 */
XapianDocument *
xapian_mset_iterator_get_document (XapianMSetIterator *iter,
                                   GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), NULL);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return NULL;

  return priv->data->getDocument (error);
}

unsigned int
xapian_mset_iterator_get_doc_id (XapianMSetIterator *iter,
                                 GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), 0);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return 0;

  return priv->data->getDocId (error);
}

/**
 * xapian_mset_iterator_get_mset:
 * @iter: ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 */
XapianMSet *
xapian_mset_iterator_get_mset (XapianMSetIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_MSET_ITERATOR (iter), NULL);

  XapianMSetIteratorPrivate *priv = XAPIAN_MSET_ITERATOR_GET_PRIVATE (iter);

  if (priv->data == NULL)
    return NULL;

  return priv->data->getMSet ();
}

/* }}} XapianMSetIterator */
