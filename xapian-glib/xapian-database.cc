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

#include <xapian.h>

#include "xapian-database-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"

#define XAPIAN_DATABASE_GET_PRIVATE(obj) \
  ((XapianDatabasePrivate *) xapian_database_get_instance_private ((XapianDatabase *) (obj)))

typedef struct _XapianDatabasePrivate   XapianDatabasePrivate;

struct _XapianDatabasePrivate
{
  char *path;

  Xapian::Database *mDB;

  GHashTable *databases;

  guint is_writable : 1;
};

enum
{
  PROP_0,

  PROP_PATH,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianDatabase, xapian_database, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (XapianDatabase)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

Xapian::Database *
xapian_database_get_internal (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->mDB;
}

void
xapian_database_set_internal (XapianDatabase *self,
                              Xapian::Database *aDB)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  delete priv->mDB;

  priv->mDB = aDB;
}

void
xapian_database_set_is_writable (XapianDatabase *self,
				 gboolean        is_writable)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  priv->is_writable = !!is_writable;
}

gboolean
xapian_database_get_is_writable (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->is_writable;
}

const char *
xapian_database_get_path (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->path;
}

static gboolean
xapian_database_init_internal (GInitable *self,
                               GCancellable *cancellable,
                               GError **error)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  try
    {
      if (priv->path != NULL)
        {
          std::string path (priv->path);

          priv->mDB = new Xapian::Database (path);
        }
      else
        priv->mDB = new Xapian::Database ();
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      priv->mDB = NULL;

      return FALSE;
    }

  return TRUE;
}

static void
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_database_init_internal;
}

static void
xapian_database_finalize (GObject *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  priv->mDB->close();
  delete priv->mDB;

  g_free (priv->path);

  if (priv->databases != NULL)
    g_hash_table_unref (priv->databases);

  G_OBJECT_CLASS (xapian_database_parent_class)->finalize (self);
}

static void
xapian_database_set_property (GObject *gobject,
                              guint prop_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_PATH:
      g_free (priv->path);
      priv->path = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_database_get_property (GObject *gobject,
                              guint prop_id,
                              GValue *value,
                              GParamSpec *pspec)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_PATH:
      g_value_set_string (value, priv->path);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_database_class_init (XapianDatabaseClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = xapian_database_set_property;
  gobject_class->get_property = xapian_database_get_property;
  gobject_class->finalize = xapian_database_finalize;

  obj_props[PROP_PATH] =
    g_param_spec_string ("path",
                         "Path",
                         "Path to the database directory",
                         NULL,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY |
                                        G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_database_init (XapianDatabase *self)
{
}

XapianDatabase *
xapian_database_new (GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  return static_cast<XapianDatabase *> (g_initable_new (XAPIAN_TYPE_DATABASE, NULL, error, NULL));
}

XapianDatabase *
xapian_database_new_with_path (const char  *path,
                               GError     **error)
{
  g_return_val_if_fail (path != NULL, NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  return static_cast<XapianDatabase *> (g_initable_new (XAPIAN_TYPE_DATABASE,
                                                        NULL, error,
                                                        "path", path,
                                                        NULL));
}

void
xapian_database_close (XapianDatabase *db)
{
  g_return_if_fail (XAPIAN_IS_DATABASE (db));

  xapian_database_get_internal (db)->close();
}

/**
 * xapian_database_get_description:
 * @db: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_database_get_description (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);

  std::string str = xapian_database_get_internal (db)->get_description ();

  return g_strdup (str.c_str ());
}

/**
 * xapian_database_get_uuid:
 * @db: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_database_get_uuid (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);

  std::string str = xapian_database_get_internal (db)->get_uuid ();

  return g_strdup (str.c_str ());
}

/**
 * xapian_database_get_metadata:
 * @db: ...
 * @key: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_database_get_metadata (XapianDatabase  *db,
                              const char      *key,
                              GError         **error)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);
  g_return_val_if_fail (key != NULL, NULL);

  try
    {
      std::string tmp_key (key);
      std::string str = xapian_database_get_internal (db)->get_metadata (tmp_key);

      return g_strdup (str.c_str());
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}

unsigned int
xapian_database_get_doc_count (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), 0);

  return xapian_database_get_internal (db)->get_doccount ();
}

unsigned int
xapian_database_get_last_doc_id (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), 0);

  return xapian_database_get_internal (db)->get_lastdocid ();
}

/**
 * xapian_database_get_document:
 * @db: ...
 * @docid: ...
 * @error: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
XapianDocument *
xapian_database_get_document (XapianDatabase  *db,
                              unsigned int     docid,
                              GError         **error)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);
  g_return_val_if_fail (docid > 0, NULL);

  try
    {
      Xapian::Document doc = xapian_database_get_internal (db)->get_document (docid);

      return xapian_document_new_from_document (doc);
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}

void
xapian_database_add_database (XapianDatabase *db,
                              XapianDatabase *new_db)
{
  g_return_if_fail (XAPIAN_IS_DATABASE (db));
  g_return_if_fail (XAPIAN_IS_DATABASE (new_db));

  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (db);
  if (priv->databases == NULL)
    priv->databases = g_hash_table_new_full (NULL, NULL, g_object_unref, NULL);

  if (g_hash_table_lookup (priv->databases, new_db) != NULL)
    return;

  Xapian::Database *real_db = xapian_database_get_internal (db);

  real_db->add_database (*xapian_database_get_internal (new_db));

  /* we tie the lifetime of the child database to the parent's because
   * the transfer rules of Xapian::Database::add_database() are a bit
   * unclear
   */
  g_hash_table_add (priv->databases, g_object_ref (new_db));
}
