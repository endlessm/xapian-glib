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
  Xapian::MSet mSet;
} XapianMSetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianMSet, xapian_mset, G_TYPE_OBJECT)

static void
xapian_mset_class_init (XapianMSetClass *klass)
{
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
  XapianMSet *res = (XapianMSet *) g_object_new (XAPIAN_TYPE_MSET, NULL);

  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (res);
  priv->mSet = Xapian::MSet (aMSet);

  return res;
}

Xapian::MSet *
xapian_mset_get_internal (XapianMSet *mset)
{
  XapianMSetPrivate *priv = XAPIAN_MSET_GET_PRIVATE (mset);

  return &priv->mSet;
}

unsigned int
xapian_mset_get_termfreq (XapianMSet *mset,
                          const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  std::string term_s (term, strlen (term));

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_termfreq (term_s);
}

double
xapian_mset_get_termweight (XapianMSet *mset,
                            const char *term)
{
  g_return_val_if_fail (mset != NULL, 0);
  g_return_val_if_fail (term != NULL, 0);

  std::string term_s (term, strlen (term));

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_termweight (term_s);
}

unsigned int
xapian_mset_get_firstitem (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_firstitem ();
}

unsigned int
xapian_mset_get_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_matches_lower_bound ();
}

unsigned int
xapian_mset_get_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_matches_estimated ();
}

unsigned int
xapian_mset_get_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_matches_upper_bound ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_lower_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_uncollapsed_matches_lower_bound ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_estimated (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_uncollapsed_matches_estimated ();
}

unsigned int
xapian_mset_get_uncollapsed_matches_upper_bound (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_uncollapsed_matches_upper_bound ();
}

double
xapian_mset_get_max_possible (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_max_possible ();
}

double
xapian_mset_get_max_attained (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.get_max_attained ();
}

unsigned int
xapian_mset_get_size (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, 0);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.size ();
}

gboolean
xapian_mset_is_empty (XapianMSet *mset)
{
  g_return_val_if_fail (mset != NULL, TRUE);

  return XAPIAN_MSET_GET_PRIVATE (mset)->mSet.empty () ? TRUE : FALSE;
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

typedef struct {
  XapianMSet *mset;

  Xapian::MSetIterator mBegin;
  Xapian::MSetIterator mEnd;
  Xapian::MSetIterator mCurrent;

  bool mCurrentInitialized;

  XapianDocument *document;
} IteratorData;

typedef struct {
  IteratorData *data;
} RealMSetIterator;

static void
iterator_data_free (gpointer data_)
{
  if (G_LIKELY (data_ != NULL))
    {
      IteratorData *data = (IteratorData *) data_;

      g_clear_object (&data->mset);
      g_clear_object (&data->document);

      g_free (data);
    }
}

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
    iterator_data_free (real_iter->data);

  real_iter->data = g_new0 (IteratorData, 1);

  real_iter->data->mset = (XapianMSet *) g_object_ref (mset);

  real_iter->data->mBegin = Xapian::MSetIterator (XAPIAN_MSET_GET_PRIVATE (mset)->mSet.begin ());
  real_iter->data->mEnd = Xapian::MSetIterator (XAPIAN_MSET_GET_PRIVATE (mset)->mSet.end ());

  real_iter->data->mCurrentInitialized = false;

  return iter;
}

void
xapian_mset_iterator_clear (XapianMSetIterator *iter)
{
  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  g_return_if_fail (iter != NULL);

  g_clear_pointer (&real_iter->data, iterator_data_free);
}

static XapianMSetIterator *
xapian_mset_iterator_copy (XapianMSetIterator *iter)
{
  if (G_LIKELY (iter != NULL))
    {
      RealMSetIterator *real_iter = (RealMSetIterator *) iter;
      RealMSetIterator *copy = g_new0 (RealMSetIterator, 1);

      if (real_iter->data != NULL)
        {
          copy->data = g_new0 (IteratorData, 1);

          copy->data->mset = (XapianMSet *) g_object_ref (real_iter->data->mset);

          copy->data->mBegin = Xapian::MSetIterator (real_iter->data->mBegin);
          copy->data->mEnd = Xapian::MSetIterator (real_iter->data->mEnd);

          if (real_iter->data->document != NULL)
            copy->data->document = (XapianDocument *) g_object_ref (real_iter->data->document);

          copy->data->mCurrentInitialized = false;
        }

      return (XapianMSetIterator *) copy;
    }

  return NULL;
}

static void
xapian_mset_iterator_free (XapianMSetIterator *iter)
{
  if (G_LIKELY (iter != NULL))
    {
      RealMSetIterator *real_iter = (RealMSetIterator *) iter;

      iterator_data_free (real_iter->data);
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

  if (real_iter->data->mset == NULL)
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

  if (real_iter->data->mset == NULL)
    return FALSE;

  return real_iter->data->mCurrent == real_iter->data->mBegin;
}

gboolean
xapian_mset_iterator_is_end (XapianMSetIterator *iter)
{
  g_return_val_if_fail (iter != NULL, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (real_iter->data == NULL)
    return FALSE;

  if (real_iter->data->mset == NULL)
    return FALSE;

  return real_iter->data->mCurrent == real_iter->data->mEnd;
}

gboolean
xapian_mset_iterator_next (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (!real_iter->data->mCurrentInitialized)
    {
      real_iter->data->mCurrent = real_iter->data->mBegin;
      real_iter->data->mCurrentInitialized = true;
    }
  else
    real_iter->data->mCurrent++;

  g_clear_object (&(real_iter->data->document));

  return real_iter->data->mCurrent != real_iter->data->mEnd;
}

gboolean
xapian_mset_iterator_prev (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, FALSE);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  if (!real_iter->data->mCurrentInitialized)
    {
      real_iter->data->mCurrent = real_iter->data->mEnd;
      real_iter->data->mCurrentInitialized = true;
    }
  else
    real_iter->data->mCurrent--;

  g_clear_object (&(real_iter->data->document));

  return real_iter->data->mCurrent != real_iter->data->mEnd;
}

unsigned int
xapian_mset_iterator_get_rank (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->mCurrent.get_rank ();
}

double
xapian_mset_iterator_get_weight (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->mCurrent.get_weight ();
}

int
xapian_mset_iterator_get_percent (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return real_iter->data->mCurrent.get_percent ();
}

unsigned int
xapian_mset_iterator_get_docid (XapianMSetIterator *iter)
{
  xapian_glib_return_val_if_iter_invalid (iter, 0);

  RealMSetIterator *real_iter = (RealMSetIterator *) iter;

  return *(real_iter->data->mCurrent);
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

  /* we cache the XapianDocument wrapper so that multiple invocations
   * will return the same GObject instance, instead of a different one
   * wrapping the same Xapian::Document. the field is cleared when the
   * iterator is advanced, or when it's cleared.
   */
  if (real_iter->data->document != NULL)
    return real_iter->data->document;

  try
    {
      Xapian::Document document = real_iter->data->mCurrent.get_document ();

      real_iter->data->document = xapian_document_new_from_document (document);

      return real_iter->data->document;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}

/* }}} XapianMSetIterator */
