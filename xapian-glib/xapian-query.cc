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

#include "xapian-query-private.h"

#include "xapian-enums.h"
#include "xapian-error-private.h"

#define XAPIAN_QUERY_GET_PRIVATE(obj) \
  ((XapianQueryPrivate *) xapian_query_get_instance_private ((XapianQuery *) (obj)))

typedef struct {
  Xapian::Query *mQuery;
} XapianQueryPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianQuery, xapian_query, G_TYPE_OBJECT)

Xapian::Query *
xapian_query_get_internal (XapianQuery *self)
{
  XapianQueryPrivate *priv = XAPIAN_QUERY_GET_PRIVATE (self);

  return priv->mQuery;
}

static void
xapian_query_dispose (GObject *gobject)
{
  XapianQueryPrivate *priv = XAPIAN_QUERY_GET_PRIVATE (gobject);

  if (priv->mQuery)
    {
      delete priv->mQuery;
      priv->mQuery = NULL;
    }

  G_OBJECT_CLASS (xapian_query_parent_class)->dispose (gobject);
}

static void
xapian_query_class_init (XapianQueryClass *klass)
{
  G_OBJECT_CLASS (klass)->dispose = xapian_query_dispose;
}

static void
xapian_query_init (XapianQuery *self)
{
}

XapianQuery *
xapian_query_new_from_query (const Xapian::Query &aQuery)
{
  XapianQuery *res = static_cast<XapianQuery *> (g_object_new (XAPIAN_TYPE_QUERY, NULL));

  XapianQueryPrivate *priv = XAPIAN_QUERY_GET_PRIVATE (res);
  priv->mQuery = new Xapian::Query (aQuery);

  return res;
}

XapianQuery *
xapian_query_new (void)
{
  return static_cast<XapianQuery *> (g_object_new (XAPIAN_TYPE_QUERY, NULL));
}

gboolean
xapian_query_is_empty (XapianQuery *query)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY (query), TRUE);

  return xapian_query_get_internal (query)->empty () ? TRUE : FALSE;
}

char *
xapian_query_get_description (XapianQuery *query)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY (query), NULL);

  std::string desc = xapian_query_get_internal (query)->get_description ();

  return g_strdup (desc.c_str ());
}

unsigned int
xapian_query_get_length (XapianQuery *query)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY (query), 0);

  return xapian_query_get_internal (query)->get_length ();
}
