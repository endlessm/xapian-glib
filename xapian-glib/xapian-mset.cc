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

#include <string.h>

#include "xapian-mset-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"

/* XapianMSet {{{ */

#define XAPIAN_MSET_GET_PRIVATE(obj) \
  ((XapianMSetPrivate *) xapian_mset_get_instance_private ((XapianMSet *) (obj)))

typedef struct {
  Xapian::MSet *mSet;
} XapianMSetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianMSet, xapian_mset, G_TYPE_OBJECT)

static void
xapian_mset_class_dispose (GObject *gobject)
{
  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (gobject);

  if (priv->mSet)
    {
      delete priv->mSet;
      priv->mSet = NULL;
    }

  G_OBJECT_CLASS (xapian_mset_parent_class)->dispose (gobject);
}

static void
xapian_mset_class_init (XapianMSetClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = xapian_mset_class_dispose;
}

static void
xapian_mset_init (XapianMSet *self)
{
}

/*< private >
 * xapian_mset_new:
 * @aMSet: a Xapian::MSet
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
XapianMSet *
xapian_mset_new (const Xapian::MSet &aMSet)
{
  XapianMSet *res = static_cast<XapianMSet *> (g_object_new (XAPIAN_TYPE_MSET, NULL));

  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (res);
  priv->mSet = new Xapian::MSet (aMSet);

  return res;
}

Xapian::MSet *
xapian_mset_get_internal (XapianMSet *mset)
{
  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (mset);

  return priv->mSet;
}

unsigned int
xapian_mset_get_termfreq (XapianMSet *mset,
                          const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  std::string term_s (term);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_termfreq (term_s);
}

double
xapian_mset_get_termweight (XapianMSet *mset,
                            const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  std::string term_s (term);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_termweight (term_s);
}

unsigned int
xapian_mset_get_firstitem (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_firstitem ();
}

unsigned int
xapian_mset_get_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_matches_lower_bound ();
}

unsigned int
xapian_mset_get_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_matches_estimated ();
}

unsigned int
xapian_mset_get_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_matches_upper_bound ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_uncollapsed_matches_lower_bound ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_uncollapsed_matches_estimated ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_uncollapsed_matches_upper_bound ();
}

double
xapian_mset_get_max_possible (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_max_possible ();
}

double
xapian_mset_get_max_attained (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->get_max_attained ();
}

unsigned int
xapian_mset_get_size (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->size ();
}

gboolean
xapian_mset_is_empty (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, TRUE);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet->empty () ? TRUE : FALSE;
}

/* }}} XapianMSet */

/* XapianMSetIterator {{{ */

#ifdef G_DISABLE_CHECKS
#define xapian_glib_return_if_iter_invalid(iter)                G_STMT_START{ (void)0; }G_STMT_END
#define xapian_glib_return_val_if_iter_invalid(iter,res)        G_STMT_START{ (void)0; }G_STMT_END
#else
#define xapian_glib_return_if_iter_invalid(iter)        G_STMT_START {  \
  RealMSetIterator *__real_iter = (RealMSetIterator *) iter;            \
  if (G_UNLIKELY (__real_iter == NULL || __real_iter->data == NULL)) {  \
    g_critical ("You must initialize a XapianMSetIterator using "       \
                "xapian_mset_iterator_init() before you can call "      \
                "%s().", G_STRFUNC);                                    \
    return;                                                             \
  }                                                     } G_STMT_END

#define xapian_glib_return_val_if_iter_invalid(iter,res)        G_STMT_START {  \
  RealMSetIterator *__real_iter = (RealMSetIterator *) iter;                    \
  if (G_UNLIKELY (__real_iter == NULL || __real_iter->data == NULL)) {          \
    g_critical ("You must initialize a XapianMSetIterator using "               \
                "xapian_mset_iterator_init() before you can call "              \
                "%s().", G_STRFUNC);                                            \
    return (res);                                                               \
  }                                                             } G_STMT_END
#endif /* G_DISABLE_CHECKS */

static XapianMSetIterator *     xapian_mset_iterator_copy       (XapianMSetIterator *iter);
static void                     xapian_mset_iterator_free       (XapianMSetIterator *iter);

G_DEFINE_BOXED_TYPE (XapianMSetIterator, xapian_mset_iterator,
                     xapian_mset_iterator_copy,
                     xapian_mset_iterator_free)

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
          mCurrent = mBegin;
          mCurrentInitialized = true;
          return *mCurrent != *mEnd;
        }

      mCurrent++;

      if (*mCurrent == *mEnd)
        return false;

      return true;
    }

    bool prev () {
      if (!mCurrentInitialized)
        {
          mCurrent = mEnd;
          mCurrentInitialized = true;
          return *mCurrent != *mBegin;
        }

      mCurrent--;

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
} RealMSetIterator;

/**
 * xapian_mset_iterator_alloc: (constructor)
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
XapianMSetIterator *
xapian_mset_iterator_alloc (void)
{
  return g_new0 (XapianMSetIterator, 1);
}

/**
 * xapian_mset_iterator:
 * @iter: ...
 * @mset: ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 */
XapianMSetIterator *
xapian_mset_iterator_init (XapianMSetIterator *iter,
                           XapianMSet         *mset)
{
  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  g_return_val_if_fail (iter != NULL, NULL);
  g_return_val_if_fail (XAPIAN_IS_MSET (mset), NULL);

  if (real_iter->data != NULL)
    delete real_iter->data;

  real_iter->data = new IteratorData (mset);

  return iter;
}

void
xapian_mset_iterator_clear (XapianMSetIterator *iter)
{
  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  g_return_if_fail (iter != NULL);

  if (real_iter->data)
    {
      delete real_iter->data;
      real_iter->data = NULL;
    }
}

static XapianMSetIterator *
xapian_mset_iterator_copy (XapianMSetIterator *iter)
{
  if (G_UNLIKELY (iter == NULL))
    return NULL;

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;
  RealMSetIterator *copy = g_new0 (RealMSetIterator, 1);

  if (real_iter->data != NULL)
    copy->data = new IteratorData (*real_iter->data);

  return (XapianMSetIterator *) copy;
}

static void
xapian_mset_iterator_free (XapianMSetIterator *iter)
{
  if (G_LIKELY (iter != NULL))
    {
      RealMSetIterator *real_iter = (RealMSetIterator *) iter;

      delete real_iter->data;
      g_free (real_iter);
    }
}

gboolean
xapian_mset_iterator_is_valid (XapianMSetIterator *iter)
{
  g_return_val_if_fail (iter != NULL, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (real_iter->data == NULL)
    return FALSE;

  return TRUE;
}

gboolean
xapian_mset_iterator_is_begin (XapianMSetIterator *iter)
{
  g_return_val_if_fail (iter != NULL, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (real_iter->data == NULL)
    return FALSE;

  return real_iter->data->isBegin () ? TRUE : FALSE;
}

gboolean
xapian_mset_iterator_is_end (XapianMSetIterator *iter)
{
  g_return_val_if_fail (iter != NULL, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (real_iter->data == NULL)
    return FALSE;

  return real_iter->data->isEnd () ? TRUE : FALSE;
}

gboolean
xapian_mset_iterator_next (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->next () ? TRUE : FALSE;
}

gboolean
xapian_mset_iterator_prev (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->prev () ? TRUE : FALSE;
}

unsigned int
xapian_mset_iterator_get_rank (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->getRank ();
}

double
xapian_mset_iterator_get_weight (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->getWeight ();
}

int
xapian_mset_iterator_get_percent (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->getPercent ();
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
  xapian_glib_return_val_if_iter_invalid (iter, NULL);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->getDocument (error);
}

unsigned int
xapian_mset_iterator_get_doc_id (XapianMSetIterator *iter,
                                 GError            **error)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->getDocId (error);
}

/* }}} XapianMSetIterator */
