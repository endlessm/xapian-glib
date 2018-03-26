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
 * SECTION:xapian-enquire
 * @Title: XapianEnquire
 * @short_description: Query a database for matching documents
 *
 * #XapianEnquire is a class used to perform queries on #XapianDatabase.
 *
 * Typically, you create a new #XapianEnquire instance for a specific
 * #XapianDatabase (which may contain multiple groups of databases),
 * and assign a #XapianQuery to the #XapianEnquire instance; then you
 * retrieve the set of matching documents through #XapianMSet.
 */

#include "config.h"

#include "xapian-enquire.h"

#include "xapian-database-private.h"
#include "xapian-error-private.h"
#include "xapian-mset-private.h"
#include "xapian-query-private.h"

#define XAPIAN_ENQUIRE_GET_PRIVATE(obj) \
  ((XapianEnquirePrivate *) xapian_enquire_get_instance_private ((XapianEnquire *) (obj)))

typedef struct {
  Xapian::Enquire *mEnquire;

  XapianDatabase *database;

  XapianQuery *query;
} XapianEnquirePrivate;

enum {
  PROP_0,

  PROP_DATABASE,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianEnquire, xapian_enquire, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (XapianEnquire)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

static void
xapian_enquire_set_database (XapianEnquire  *self,
                             XapianDatabase *db)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (self);

  g_assert (priv->database == NULL);

  priv->database = static_cast<XapianDatabase *> (g_object_ref (db));
}

static gboolean
xapian_enquire_init_internal (GInitable    *self,
                              GCancellable *cancellable,
                              GError      **error)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (self);

  if (priv->database == NULL)
    {
      g_set_error_literal (error, XAPIAN_ERROR,
                           XAPIAN_ERROR_INVALID_ARGUMENT,
                           "XapianEnquire:database must not be empty or NULL");
      return FALSE;
    }

  try
    {
      Xapian::Database *database = xapian_database_get_internal (priv->database);

      priv->mEnquire = new Xapian::Enquire (*database);

      return TRUE;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      priv->mEnquire = NULL;

      return FALSE;
    }
}

static void
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_enquire_init_internal;
}

static void
xapian_enquire_set_property (GObject      *gobject,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  XapianEnquire *self = XAPIAN_ENQUIRE (gobject);

  switch (prop_id)
    {
    case PROP_DATABASE:
      xapian_enquire_set_database (self, (XapianDatabase *) g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_enquire_get_property (GObject    *gobject,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_DATABASE:
      g_value_set_object (value, priv->database);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_enquire_finalize (GObject *gobject)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (gobject);

  delete priv->mEnquire;

  G_OBJECT_CLASS (xapian_enquire_parent_class)->finalize (gobject);
}

static void
xapian_enquire_dispose (GObject *gobject)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (gobject);

  g_clear_object (&priv->database);
  g_clear_object (&priv->query);

  G_OBJECT_CLASS (xapian_enquire_parent_class)->dispose (gobject);
}

static void
xapian_enquire_class_init (XapianEnquireClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  /**
   * XapianEnquire:database:
   *
   * The #XapianDatabase to query.
   */
  obj_props[PROP_DATABASE] =
    g_param_spec_object ("database",
                         "Database",
                         "The database to query",
                         XAPIAN_TYPE_DATABASE,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY |
                                        G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_enquire_set_property;
  gobject_class->get_property = xapian_enquire_get_property;
  gobject_class->dispose = xapian_enquire_dispose;
  gobject_class->finalize = xapian_enquire_finalize;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_enquire_init (XapianEnquire *self)
{
}

/**
 * xapian_enquire_new:
 * @db: a #XapianDatabase
 * @error: return location for a #GError, or %NULL
 *
 * Creates and initializes a new #XapianEnquire instance for the
 * given #XapianDatabase.
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianEnquire instance
 */
XapianEnquire *
xapian_enquire_new (XapianDatabase *db,
                    GError        **error)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);

  return static_cast<XapianEnquire *> (g_initable_new (XAPIAN_TYPE_ENQUIRE,
                                                       NULL, error,
                                                       "database", db,
                                                       NULL));
}

/**
 * xapian_enquire_set_collapse_key:
 * @enquire: a #XapianEnquire
 * @collapse_key: value number to collapse on
 *
 * Sets the collapse key for the #XapianEnquire instance with the default
 * collapse_max value of 1.
 *
 * Since: 2.0
 */
void
xapian_enquire_set_collapse_key (XapianEnquire *enquire,
                                 unsigned int  collapse_key)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }

  priv->mEnquire->set_collapse_key (collapse_key);
}

/**
 * xapian_enquire_set_collapse_key_full:
 * @enquire: a #XapianEnquire
 * @collapse_key: value number to collapse on
 * @collapse_max: Max number of items with the same key to leave after collapsing
 *
 * Sets the collapse key for the #XapianEnquire instance.
 *
 * Since: 2.0
 */
void
xapian_enquire_set_collapse_key_full (XapianEnquire *enquire,
                                      unsigned int  collapse_key,
                                      unsigned int  collapse_max)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }

  priv->mEnquire->set_collapse_key (collapse_key, collapse_max);
}

/**
 * xapian_enquire_set_cutoff:
 * @enquire: a #XapianEnquire
 * @percent_cutoff: Minimum percentage score for returned documents. If a
 *                  document has a lower percentage score than this, it will
 *                  not appear in the MSet.
 *
 * Set the percentage cutoffs with no weight cutoff.
 *
 * Since: 2.0
 */
void
xapian_enquire_set_cutoff (XapianEnquire *enquire,
                           gfloat         percent_cutoff)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }

  priv->mEnquire->set_cutoff (percent_cutoff, 0);
}

/**
 * xapian_enquire_set_cutoff_full:
 * @enquire: a #XapianEnquire
 * @percent_cutoff: Minimum percentage score for returned documents. If a
 *                  document has a lower percentage score than this, it will
 *                  not appear in the MSet.
 * @weight_cutoff: Minimum weight for a document to be returned. If a document
 *                 has a lower score that this, it will not appear in the MSet
 *
 * Set the percentage and weight cutoffs.
 *
 * Since: 2.0
 */
void
xapian_enquire_set_cutoff_full (XapianEnquire *enquire,
                                gfloat         percent_cutoff,
                                unsigned int   weight_cutoff)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }

  priv->mEnquire->set_cutoff (percent_cutoff, weight_cutoff);
}

/**
 * xapian_enquire_set_sort_by_value:
 * @enquire: a #XapianEnquire
 * @sort_key: value number to sort on.
 * @reverse: If true, reverses the sort order.
 *
 * Set the sorting to be by value only.
 * Note that sorting by values uses a string comparison; to use this function to sort by a numeric
 * value you will have to store the values in a way that sorts appropriately, for instance by padding
 * the numbers using leading zeros — e.g. `001, 002, 003, ...`.
 *
 * Since: 2.0
 */
void
xapian_enquire_set_sort_by_value (XapianEnquire *enquire,
                                  unsigned int   sort_key,
                                  gboolean       reverse)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }

  priv->mEnquire->set_sort_by_value (sort_key, reverse);
}

/**
 * xapian_enquire_set_query:
 * @enquire: a #XapianEnquire
 * @query: a #XapianQuery
 * @qlen: the length of the @query
 *
 * Sets the #XapianQuery for the #XapianEnquire instance.
 */
void
xapian_enquire_set_query (XapianEnquire *enquire,
			  XapianQuery   *query,
			  unsigned int   qlen)
{
  g_return_if_fail (XAPIAN_IS_ENQUIRE (enquire));
  g_return_if_fail (XAPIAN_IS_QUERY (query));

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return;
    }
#endif

  if (priv->query == query)
    return;

  g_clear_object (&priv->query);
  priv->query = static_cast<XapianQuery *> (g_object_ref (query));

  priv->mEnquire->set_query (*xapian_query_get_internal (query), qlen);
}

/**
 * xapian_enquire_get_query:
 * @enquire: a #XapianEnquire
 *
 * Retrieves the #XapianQuery set using xapian_enquire_set_query().
 *
 * Returns: (transfer none): a #XapianQuery or %NULL if none is set
 */
XapianQuery *
xapian_enquire_get_query (XapianEnquire *enquire)
{
  g_return_val_if_fail (XAPIAN_IS_ENQUIRE (enquire), NULL);

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

  return priv->query;
}

/**
 * xapian_enquire_get_mset:
 * @enquire: a #XapianEnquire
 * @first: the first item in the result set
 * @max_items: the maximum number of results to return
 * @error: return location for a #GError
 *
 * Retrieves @max_items items matching the #XapianQuery used with
 * the @enquire instance.
 *
 * If @first is zero, the first item returned will be the one with
 * the highest score. If non-zero, the value will be the number of
 * items ignored. A combination of @first and @max_items can be used
 * to paginate the matching results.
 *
 * In case of error, @error will be set, and this function will
 * return %NULL.
 *
 * Returns: (transfer full): a #XapianMSet containing the matching
 *   documents
 */
XapianMSet *
xapian_enquire_get_mset (XapianEnquire *enquire,
                         unsigned int   first,
                         unsigned int   max_items,
                         GError       **error)
{
  g_return_val_if_fail (XAPIAN_IS_ENQUIRE (enquire), NULL);

  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (enquire);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  if (G_UNLIKELY (priv->mEnquire == NULL))
    {
      g_critical ("XapianEnquire must be initialized. Use g_initable_init() "
                  "before calling any XapianEnquire method.");
      return NULL;
    }
#endif

  try
    {
      Xapian::MSet mset = priv->mEnquire->get_mset (first, max_items);

      return xapian_mset_new (mset);
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}
