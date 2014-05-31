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
 * Return value: the frequency of the term
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

/**
 * xapian_mset_get_begin:
 * @mset: a #XapianMSet
 *
 * Creates an iterator placed before the beginning of the @mset.
 *
 * In order to access the contents of the @mset through the
 * #XapianMSetIterator API you will have to advance the iterator
 * first, e.g.:
 *
 * |[<!-- language=C -->
 *   XapianMSetIter *iter = xapian_mset_get_begin (mset);
 *
 *   while (xapian_mset_iterator_next (iter))
 *     {
 *       // ...
 *     }
 *
 *   g_object_unref (iter);
 * ]|
 *
 * Returns: (transfer full): the newly created #XapianMSetIterator instance
 */
XapianMSetIterator *
xapian_mset_get_begin (XapianMSet *mset)
{
  g_return_val_if_fail (XAPIAN_IS_MSET (mset), NULL);

  return xapian_mset_iterator_new (mset);
}

/**
 * xapian_mset_get_end:
 * @mset: a #XapianMSet
 *
 * Creates a new #XapianMSetIterator placed after the end of the @mset.
 *
 * In order to access the contents of the @mset through the
 * #XapianMSetIterator API you will have to advance the iterator
 * first, e.g.:
 *
 * |[<!-- language=C -->
 *   XapianMSetIter *iter = xapian_mset_get_end (mset);
 *
 *   while (xapian_mset_iterator_prev (iter))
 *     {
 *       // ...
 *     }
 *
 *   g_object_unref (iter);
 * ]|
 *
 * Returns: (transfer full): the newly created #XapianMSetIterator instance
 */
XapianMSetIterator *
xapian_mset_get_end (XapianMSet *mset)
{
  g_return_val_if_fail (XAPIAN_IS_MSET (mset), NULL);

  return xapian_mset_iterator_new (mset);
}
