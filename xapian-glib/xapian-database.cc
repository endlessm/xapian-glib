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
 * SECTION:xapian-database
 * @Title: XapianDatabase
 * @short_description: A read-only database
 *
 * #XapianDatabase is a class that allows read-only access to a
 * Xapian database at a given path.
 *
 * Typically, you will use #XapianDatabase to open a database for
 * querying, by using the #XapianEnquire class.
 */

#include "config.h"

#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#include <xapian.h>

#include "xapian-database-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"
#include "xapian-private.h"

#define XAPIAN_DATABASE_GET_PRIVATE(obj) \
  ((XapianDatabasePrivate *) xapian_database_get_instance_private ((XapianDatabase *) (obj)))

typedef struct _XapianDatabasePrivate   XapianDatabasePrivate;

struct _XapianDatabasePrivate
{
  char *path;
  off_t offset;
  XapianDatabaseFlags flags;
  XapianDatabaseBackend backend;

  Xapian::Database *mDB;

  GHashTable *databases;

  guint is_writable : 1;
};

enum
{
  PROP_0,

  PROP_PATH,
  PROP_OFFSET,
  PROP_FLAGS,
  PROP_BACKEND,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianDatabase, xapian_database, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (XapianDatabase)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

/*< private >
 * xapian_database_get_internal:
 * @self: a #XapianDatabase
 *
 * Retrieves the `Xapian::Database` object used by @self.
 *
 * Returns: (transfer none): a pointer to the internal database instance
 */
Xapian::Database *
xapian_database_get_internal (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->mDB;
}

/*< private >
 * xapian_database_set_internal:
 * @self: a #XapianDatabase
 * @aDB: a `Xapian::Database` instance
 *
 * Sets the internal database instance wrapped by @self, clearing
 * any existing instance if needed.
 */
void
xapian_database_set_internal (XapianDatabase   *self,
                              Xapian::Database *aDB)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  delete priv->mDB;

  priv->mDB = aDB;
}

/*< private >
 * xapian_database_set_is_writable:
 * @self: a #XapianDatabase
 * @is_writable: whether the internal instance is writable
 *
 * Sets the writable bit on the #XapianDatabase wrapper.
 *
 * The writable bit is needed because #XapianDatabase acts as
 * a wrapper of both #XapianDatabase and #XapianWritableDatabase
 * to preserve the inheritance model of C++. We use the writable
 * bit to avoid a type check.
 */
void
xapian_database_set_is_writable (XapianDatabase *self,
				 gboolean        is_writable)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  priv->is_writable = !!is_writable;
}

/*< private >
 * xapian_database_get_is_writable:
 * @self: a #XapianDatabase
 *
 * Retrieves whether the #XapianDatabase has the writable bit set.
 *
 * Returns: %TRUE if the database is writable, and %FALSE otherwise
 */
gboolean
xapian_database_get_is_writable (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->is_writable;
}

/*< private >
 * xapian_database_get_path:
 * @self: a #XapianDatabase
 *
 * Internal accessor for the #XapianDatabase:path property that
 * avoids a copy.
 *
 * Returs: (transfer none): the path of the database
 */
const char *
xapian_database_get_path (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  return priv->path;
}

static Xapian::Database *
open_database (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  if (priv->path != NULL && priv->path[0] != '\0')
    {
      int db_flags = xapian_database_get_flags (self);

      /* If we're given an offset, open the database at that location */
      if (priv->offset != 0)
        {
          int fd = open (priv->path, O_RDONLY | O_CLOEXEC);
          if (fd < 0)
            {
              std::string errmsg ("Invalid database path ");
              errmsg += priv->path;
              throw Xapian::DatabaseOpeningError (errmsg, errno);
            }
          if (lseek (fd, priv->offset, SEEK_SET) < 0)
            throw Xapian::DatabaseOpeningError ("Can't seek to offset", errno);

          /* Takes ownership of fd */
          return new Xapian::Database (fd, db_flags);
        }

      /* Otherwise just open the given path */
      return new Xapian::Database (priv->path, db_flags);
    }

  /* If no path nor offset is provided, we create an empty database */
  return new Xapian::Database ();
}

static gboolean
xapian_database_init_internal (GInitable    *self,
                               GCancellable *cancellable,
                               GError      **error)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);

  try
    {
      priv->mDB = open_database (XAPIAN_DATABASE (self));
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

  delete priv->mDB;

  g_free (priv->path);

  if (priv->databases != NULL)
    g_hash_table_unref (priv->databases);

  G_OBJECT_CLASS (xapian_database_parent_class)->finalize (self);
}

static void
xapian_database_set_property (GObject      *gobject,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_PATH:
      g_free (priv->path);
      priv->path = g_value_dup_string (value);
      break;

    case PROP_OFFSET:
      priv->offset = g_value_get_uint64 (value);
      break;

    case PROP_FLAGS:
      priv->flags = (XapianDatabaseFlags) g_value_get_flags (value);
      break;

    case PROP_BACKEND:
      priv->backend = (XapianDatabaseBackend) g_value_get_enum (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_database_get_property (GObject    *gobject,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_PATH:
      g_value_set_string (value, priv->path);
      break;

    case PROP_OFFSET:
      g_value_set_uint64 (value, priv->offset);
      break;

    case PROP_FLAGS:
      g_value_set_flags (value, (int) priv->flags);
      break;

    case PROP_BACKEND:
      g_value_set_enum (value, (int) priv->backend);
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

  /**
   * XapianDatabase:path:
   *
   * The path to the database directory.
   */
  obj_props[PROP_PATH] =
    g_param_spec_string ("path",
                         "Path",
                         "Path to the database directory",
                         NULL,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianDatabase:offset:
   *
   * The offset inside the database file.
   *
   * Since: 1.4
   */
  obj_props[PROP_OFFSET] =
    g_param_spec_uint64 ("offset", "", "", 0, G_MAXUINT64, 0,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianDatabase:flags:
   *
   * The flags to use when opening the database.
   *
   * Since: 1.4
   */
  obj_props[PROP_FLAGS] =
    g_param_spec_flags ("flags", "", "",
                        XAPIAN_TYPE_DATABASE_FLAGS,
                        XAPIAN_DATABASE_FLAGS_NONE,
                        (GParamFlags) (G_PARAM_READWRITE |
                                       G_PARAM_CONSTRUCT_ONLY |
                                       G_PARAM_STATIC_STRINGS));

  /**
   * XapianDatabase:backend:
   *
   * The backend to use when opening the database.
   *
   * Since: 1.4
   */
  obj_props[PROP_BACKEND] =
    g_param_spec_enum ("backend", "", "",
                       XAPIAN_TYPE_DATABASE_BACKEND,
                       XAPIAN_DATABASE_BACKEND_GLASS,
                       (GParamFlags) (G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_database_init (XapianDatabase *self)
{
}

/**
 * xapian_database_new:
 * @error: return location for a #GError, or %NULL
 *
 * Creates and initializes a new, empty #XapianDatabase instance.
 *
 * If the initialization was not successful, @error is set.
 *
 * Returns: (transfer full): the newly created #XapianDatabase instance,
 *   or %NULL if the initialization failed
 */
XapianDatabase *
xapian_database_new (GError **error)
{
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  return static_cast<XapianDatabase *> (g_initable_new (XAPIAN_TYPE_DATABASE, NULL, error, NULL));
}

/**
 * xapian_database_new_with_path:
 * @path: the path to the database
 * @error: return location for a #GError, or %NULL
 *
 * Creates and initializes a new #XapianDatabase at @path.
 *
 * If the initialization was not successful, @error is set.
 *
 * Returns: (transfer full): the newly created #XapianDatabase instance
 *   for the given @path, or %NULL if the initialization failed
 */
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

/**
 * xapian_database_close:
 * @db: a #XapianDatabase
 *
 * Explicitly closes a database.
 *
 * #XapianDatabase instances are implicitly closed when being
 * finalized, so you'll rarely need to call this function.
 */
void
xapian_database_close (XapianDatabase *db)
{
  g_return_if_fail (XAPIAN_IS_DATABASE (db));

  xapian_database_get_internal (db)->close ();
}

/**
 * xapian_database_reopen:
 * @db: a #XapianDatabase
 *
 * Reopens a #XapianDatabase.
 *
 * Since: 1.2
 */
void
xapian_database_reopen (XapianDatabase *db)
{
  g_return_if_fail (XAPIAN_IS_DATABASE (db));

  xapian_database_get_internal (db)->reopen ();
}

/**
 * xapian_database_get_description:
 * @db: a #XapianDatabase
 *
 * Retrieves a string describing the #XapianDatabase.
 *
 * Typically, this function is used when debugging.
 *
 * Returns: (transfer full): a description of the database
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
 * @db: a #XapianDatabase
 *
 * Retrieves a unique identifier for a #XapianDatabase.
 *
 * Returns: (transfer full): a unique identifier for the database
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
 * @db: a #XapianDatabase
 * @key: a key to access in the database metadata
 * @error: return location for a #GError
 *
 * Retrieves custom metadata associated to a key inside the
 * #XapianDatabase.
 *
 * In case of error, this function returns %NULL and sets
 * the @error argument.
 *
 * Returns: (transfer full): the stored metadata.
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
      std::string str = xapian_database_get_internal (db)->get_metadata (std::string (key));

      return g_strdup (str.c_str ());
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}

/**
 * xapian_database_get_doc_count:
 * @db: a #XapianDatabase
 *
 * Retrieves the number of documents in the database.
 *
 * Returns: the number of documents in the database
 */
unsigned int
xapian_database_get_doc_count (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), 0);

  return xapian_database_get_internal (db)->get_doccount ();
}

/**
 * xapian_database_get_last_doc_id:
 * @db: a #XapianDatabase
 *
 * Retrieves the highest document id in use in the database
 *
 * Returns: the highest document id in use
 */
unsigned int
xapian_database_get_last_doc_id (XapianDatabase *db)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), 0);

  return xapian_database_get_internal (db)->get_lastdocid ();
}

/**
 * xapian_database_get_document:
 * @db: a #XapianDatabase
 * @docid: the document id to retrieve
 * @error: return location for a #GError, or %NULL
 *
 * Retrieves the #XapianDocument with the given @docid inside
 * the database.
 *
 * If the @docid was not found, @error is set and %NULL is
 * returned.
 *
 * Returns: (transfer full): a #XapianDocument instance
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

/**
 * xapian_database_add_database:
 * @db: a #XapianDatabase
 * @new_db: a #XapianDatabase
 *
 * Adds an existing database (or group of databases) to those
 * accessed by @db.
 */
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

/**
 * xapian_database_compact_to_path:
 * @self: A #XapianDatabase
 * @path: The path to write the compacted database to.
 * @flags: Flags to compact the database with.
 *
 * Compacts the database, and writes the result to the given path.
 *
 * Since: 1.4
 */
void
xapian_database_compact_to_path (XapianDatabase             *self,
                                 const char                 *path,
                                 XapianDatabaseCompactFlags  flags)
{
#if XAPIAN_CHECK_VERSION_INTERNAL (1, 3, 4)
  Xapian::Database *real_db = xapian_database_get_internal (self);

  int real_flags = 0;

  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_NO_RENUMBER) != 0)
    real_flags |= Xapian::DBCOMPACT_NO_RENUMBER;
  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_MULTIPASS) != 0)
    real_flags |= Xapian::DBCOMPACT_MULTIPASS;
  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_SINGLE_FILE) != 0)
    real_flags |= Xapian::DBCOMPACT_SINGLE_FILE;

  const std::string output (path);
  real_db->compact (output, real_flags);
#else
  g_warning ("Compaction not supported by this version of Xapian (current: %d.%d.%d).",
             XAPIAN_MAJOR_VERSION,
             XAPIAN_MINOR_VERSION,
             XAPIAN_REVISION);
#endif
}

/**
 * xapian_database_compact_to_fd:
 * @self: A #XapianDatabase
 * @fd: The fd to write the compacted database.
 * @flags: Flags to compact the database with.
 *
 * Compacts the database, and writes the result to the given fd.
 *
 * Since: 1.4
 */
void
xapian_database_compact_to_fd (XapianDatabase             *self,
                               int                         fd,
                               XapianDatabaseCompactFlags  flags)
{
#if XAPIAN_CHECK_VERSION_INTERNAL (1, 3, 4)
  Xapian::Database *real_db = xapian_database_get_internal (self);

  int real_flags = 0;

  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_NO_RENUMBER) != 0)
    real_flags |= Xapian::DBCOMPACT_NO_RENUMBER;
  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_MULTIPASS) != 0)
    real_flags |= Xapian::DBCOMPACT_MULTIPASS;
  if ((flags & XAPIAN_DATABASE_COMPACT_FLAGS_SINGLE_FILE) != 0)
    real_flags |= Xapian::DBCOMPACT_SINGLE_FILE;

  real_db->compact (fd, flags);
#else
  g_warning ("Compaction not supported by this version of Xapian (current: %d.%d.%d).",
             XAPIAN_MAJOR_VERSION,
             XAPIAN_MINOR_VERSION,
             XAPIAN_REVISION);
#endif
}

/*< private >
 * xapian_database_get_flags:
 * @self: a #XapianDatabase
 *
 * Retrieves the flags to be used when constructing a Xapian::Database
 * object, using the #XapianDatabase:flags and #XapianDatabase:backend
 * properties.
 */
int
xapian_database_get_flags (XapianDatabase *self)
{
  XapianDatabasePrivate *priv = XAPIAN_DATABASE_GET_PRIVATE (self);
  int db_flags = 0;

  if ((priv->flags & XAPIAN_DATABASE_FLAGS_NO_SYNC) != 0)
    db_flags |= Xapian::DB_NO_SYNC;
  if ((priv->flags & XAPIAN_DATABASE_FLAGS_FULL_SYNC) != 0)
    db_flags |= Xapian::DB_FULL_SYNC;
  if ((priv->flags & XAPIAN_DATABASE_FLAGS_DANGEROUS) != 0)
    db_flags |= Xapian::DB_DANGEROUS;
  if ((priv->flags & XAPIAN_DATABASE_FLAGS_NO_TERMLIST) != 0)
    db_flags |= Xapian::DB_NO_TERMLIST;
  if ((priv->flags & XAPIAN_DATABASE_FLAGS_RETRY_LOCK) != 0)
    db_flags |= Xapian::DB_RETRY_LOCK;

  if (priv->backend == XAPIAN_DATABASE_BACKEND_STUB)
    db_flags |= Xapian::DB_BACKEND_STUB;
  if (priv->backend == XAPIAN_DATABASE_BACKEND_GLASS)
#ifdef XAPIAN_HAS_GLASS_BACKEND
    db_flags |= Xapian::DB_BACKEND_GLASS;
#else
    g_warning ("Glass backend not supported by Xapian");
#endif
  if (priv->backend == XAPIAN_DATABASE_BACKEND_CHERT)
#ifdef XAPIAN_HAS_CHERT_BACKEND
    db_flags |= Xapian::DB_BACKEND_CHERT;
#else
    g_warning ("Chert backend not supported by Xapian");
#endif
  if (priv->backend == XAPIAN_DATABASE_BACKEND_INMEMORY)
#if XAPIAN_CHECK_VERSION_INTERNAL (1, 3, 5) && defined(XAPIAN_HAS_INMEMORY_BACKEND)
    db_flags |= Xapian::DB_BACKEND_INMEMORY;
#else
    g_warning ("In-memory backend not supported by Xapian");
#endif

  return db_flags;
}
