/* Copyright 2017  Olly Betts
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

#include "xapian-term-iterator.h"
#include "xapian-error-private.h"

#include <xapian/iterator.h>

/**
 * SECTION:xapian-term-iterator
 * @Title: XapianTermIterator
 * @short_description: Iterate over terms
 *
 * #XapianTermIterator is a class that allows iterating over terms.
 *
 * A typical example is:
 *
 * |[<!-- language="C" -->
 *   XapianTermIterator *iter = xapian_database_enumerate_all_terms (db);
 *
 *   while (xapian_term_iterator_next (iter))
 *     g_print ("%s", xapian_term_iterator_get_term (iter));
 * ]|
 */

class IteratorData {
    IteratorData (const IteratorData &aData);

    void operator= (const IteratorData &aData);

  public:
    IteratorData ()
      : mCurrentInitialized(false)
    {
    }

    void set_iterator (const Xapian::TermIterator &it) {
      mCurrent = it;
    }

    bool next () {
      if (!mCurrentInitialized)
        mCurrentInitialized = true;
      else
        ++mCurrent;

      return Xapian::iterator_valid (mCurrent);
    }

    char *getTermName () {
      if (!mCurrentInitialized)
        return NULL;

      const std::string &term = *mCurrent;

      return g_strdup (term.c_str ());
    }

    unsigned int getWdf () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_wdf ();
    }

    unsigned int getTermFreq () {
      if (!mCurrentInitialized)
        return 0;

      return mCurrent.get_termfreq ();
    }

    char * getDescription () {
      const std::string &desc = mCurrent.get_description ();

      return g_strdup (desc.c_str ());
    }

  private:
    Xapian::TermIterator mCurrent;

    bool mCurrentInitialized;
};

typedef struct {
  IteratorData *data;
} XapianTermIteratorPrivate;

#define XAPIAN_TERM_ITERATOR_GET_PRIVATE(obj) \
  ((XapianTermIteratorPrivate *) xapian_term_iterator_get_instance_private ((XapianTermIterator *) (obj)))

G_DEFINE_TYPE_WITH_PRIVATE (XapianTermIterator, xapian_term_iterator, G_TYPE_OBJECT)

static void
xapian_term_iterator_finalize (GObject *gobject)
{
  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (gobject);

  delete priv->data;

  G_OBJECT_CLASS (xapian_term_iterator_parent_class)->finalize (gobject);
}

static void
xapian_term_iterator_class_init (XapianTermIteratorClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = xapian_term_iterator_finalize;
}

static void
xapian_term_iterator_init (XapianTermIterator *self)
{
  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (self);
  priv->data = new IteratorData ();
}

/*< private >
 * xapian_term_iterator_new:
 * @it: a Xapian::TermIterator
 *
 * Creates a new #XapianTermIterator from the given `Xapian::TermIterator` instance.
 *
 * Returns: (transfer full): the newly created #XapianTermIterator
 *
 * Since: 1.4
 */
XapianTermIterator *
xapian_term_iterator_new (const Xapian::TermIterator &it)
{
  XapianTermIterator *iter;
  XapianTermIteratorPrivate *priv;

  iter = static_cast<XapianTermIterator *> (g_object_new (XAPIAN_TYPE_TERM_ITERATOR, NULL));
  priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);
  priv->data->set_iterator (it);

  return iter;
}

/**
 * xapian_term_iterator_is_valid:
 * @iter: a #XapianTermIterator
 *
 * Checks whether @iter is valid.
 *
 * Returns: %TRUE if the iterator is valid
 *
 * Since: 1.4
 */
gboolean
xapian_term_iterator_is_valid (XapianTermIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), FALSE);

  return XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter)->data != NULL;
}

/**
 * xapian_term_iterator_next:
 * @iter: a #XapianTermIterator
 *
 * Advances the @iter forward.
 *
 * You should use the return value to check if @iter is still
 * valid after being advanced, before using the #XapianTermIterator
 * API.
 *
 * Returns: %TRUE if the iterator was advanced, and %FALSE otherwise
 *
 * Since: 1.4
 */
gboolean
xapian_term_iterator_next (XapianTermIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), FALSE);

  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);

  return priv->data->next ();
}

/**
 * xapian_term_iterator_get_term_name:
 * @iter: a #XapianTermIterator
 *
 * Retrieves the term name of the current item pointed by @iter.
 *
 * Returns: (transfer full): the term name
 *
 * Since: 1.4
 */
char *
xapian_term_iterator_get_term_name (XapianTermIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), 0);

  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getTermName ();
}

/**
 * xapian_term_iterator_get_wdf:
 * @iter: a #XapianTermIterator
 * @error: return location for a #GError
 *
 * Retrieves the WDF of the current item pointed by @iter.
 *
 * Returns: the WDF
 *
 * Since: 1.4
 */
unsigned int
xapian_term_iterator_get_wdf (XapianTermIterator *iter,
                              GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), 0);

  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);

  try
    {
      return priv->data->getWdf ();
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return 0;
    }
}

/**
 * xapian_term_iterator_get_term_freq:
 * @iter: a #XapianTermIterator
 * @res (out): Used to return the term frequency
 * @error: return location for a #GError
 *
 * Retrieves the term frequency of the current item pointed by @iter.
 *
 * Returns: %TRUE if the term frequency was successfully returned
 *
 * Since: 1.4
 */
gboolean
xapian_term_iterator_get_term_freq (XapianTermIterator *iter,
                                    unsigned int       *res,
                                    GError            **error)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), FALSE);
  g_return_val_if_fail (res != NULL, FALSE);

  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);

  try
    {
      *res = priv->data->getTermFreq ();
      return TRUE;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return FALSE;
    }
}

/**
 * xapian_term_iterator_get_description:
 * @iter: a #XapianTermIterator
 *
 * Retrieves a description of @iter, typically used for debugging.
 *
 * Returns: (transfer full): a description of the iterator
 *
 * Since: 1.4
 */
char *
xapian_term_iterator_get_description (XapianTermIterator *iter)
{
  g_return_val_if_fail (XAPIAN_IS_TERM_ITERATOR (iter), NULL);

  XapianTermIteratorPrivate *priv = XAPIAN_TERM_ITERATOR_GET_PRIVATE (iter);

  return priv->data->getDescription ();
}
