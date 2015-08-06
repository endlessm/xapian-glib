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
 * SECTION:xapian-writable-database
 * @Title: XapianWritableDatabase
 * @short_description: A writable database
 *
 * #XapianWritableDatabase is a #XapianDatabase sub-class that can
 * be written to.
 */

#include "config.h"

#include <xapian.h>

#include "xapian-writable-database.h"
#include "xapian-database-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"
#include "xapian-enums.h"

#define XAPIAN_WRITABLE_DATABASE_GET_PRIVATE(obj) \
  ((XapianWritableDatabasePrivate *) xapian_writable_database_get_instance_private ((XapianWritableDatabase *) (obj)))

typedef struct _XapianWritableDatabasePrivate   XapianWritableDatabasePrivate;

struct _XapianWritableDatabasePrivate
{
  XapianDatabaseAction action;
};

enum
{
  PROP_0,

  PROP_ACTION,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_default_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianWritableDatabase, xapian_writable_database,
                         XAPIAN_TYPE_DATABASE,
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_default_init)
                         G_ADD_PRIVATE (XapianWritableDatabase))

static Xapian::WritableDatabase *
xapian_writable_database_get_internal (XapianWritableDatabase *self)
{
  Xapian::Database *db = xapian_database_get_internal (XAPIAN_DATABASE (self));
  Xapian::WritableDatabase *write_db = dynamic_cast<Xapian::WritableDatabase *> (db);

  return write_db;
}

static gboolean
xapian_writable_database_init_internal (GInitable    *initable,
                                        GCancellable *cancellable,
                                        GError      **error)
{
  XapianDatabase *database = XAPIAN_DATABASE (initable);
  XapianWritableDatabasePrivate *priv;
  
  priv = XAPIAN_WRITABLE_DATABASE_GET_PRIVATE (initable);

  Xapian::WritableDatabase *db;

  try
    {
      const char *path = xapian_database_get_path (database);

      if (path == NULL)
        db = new Xapian::WritableDatabase ();
      else
        {
          std::string file (path);

          db = new Xapian::WritableDatabase (file, (int) priv->action);
        }

      xapian_database_set_internal (database, db);
      xapian_database_set_is_writable (database, TRUE);

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

static void
initable_default_init (GInitableIface *iface)
{
  iface->init = xapian_writable_database_init_internal;
}

static void
xapian_writable_database_set_property (GObject      *gobject,
                                       guint         prop_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
  XapianWritableDatabasePrivate *priv;

  priv = XAPIAN_WRITABLE_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_ACTION:
      priv->action = (XapianDatabaseAction) g_value_get_enum (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_writable_database_get_property (GObject    *gobject,
                                       guint       prop_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
  XapianWritableDatabasePrivate *priv;

  priv = XAPIAN_WRITABLE_DATABASE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_ACTION:
      g_value_set_enum (value, priv->action);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_writable_database_class_init (XapianWritableDatabaseClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  /**
   * XapianWritableDatabase:action:
   *
   * The action to be performed when creating a database.
   */
  obj_props[PROP_ACTION] =
    g_param_spec_enum ("action",
                       "Action",
                       "The action to be performed when creating a database",
                       XAPIAN_TYPE_DATABASE_ACTION,
                       XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN,
                       (GParamFlags) (G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_writable_database_set_property;
  gobject_class->get_property = xapian_writable_database_get_property;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_writable_database_init (XapianWritableDatabase *self)
{
}

/**
 * xapian_writable_database_new:
 * @path: the path of the database
 * @action: the action to perform
 * @error: return location for a #GError
 *
 * Creates and initialises a #XapianWritableDatabase for the
 * given @path.
 *
 * If the initialization was not successful, this function
 * returns %NULL and sets @error.
 *
 * Returns: (transfer full): the newly created #XapianWritableDatabase
 *   instance
 */
XapianWritableDatabase *
xapian_writable_database_new (const char            *path,
                              XapianDatabaseAction   action,
                              GError               **error)
{
  return static_cast<XapianWritableDatabase *> (g_initable_new (XAPIAN_TYPE_WRITABLE_DATABASE,
                                                                NULL, error,
                                                                "path", path,
                                                                "action", action,
                                                                NULL));
}

/**
 * xapian_writable_database_commit:
 * @self: a #XapianWritableDatabase
 * @error: return location for a #GError
 *
 * Commits the pending changes of the database.
 *
 * Returns: %TRUE if the commit was successful
 */
gboolean
xapian_writable_database_commit (XapianWritableDatabase  *self,
                                 GError                 **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  /* overzealous check */
  g_assert (xapian_database_get_is_writable (XAPIAN_DATABASE (self)));
#endif

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

      write_db->commit ();

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
 * xapian_writable_database_add_document:
 * @self: a #XapianWritableDatabase
 * @document: a #XapianDocument to add
 * @docid_out: (out): return location for the id of the newly
 *   added document
 * @error: return location for a #GError
 *
 * Adds @document to a database.
 *
 * Returns: %TRUE if the document was added, and %FALSE on error
 */
gboolean
xapian_writable_database_add_document (XapianWritableDatabase *self,
                                       XapianDocument         *document,
                                       unsigned int           *docid_out,
                                       GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), FALSE);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  g_assert (xapian_database_get_is_writable (XAPIAN_DATABASE (self)));
#endif

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);
      Xapian::Document doc = *xapian_document_get_internal (document);
      Xapian::docid id = write_db->add_document (doc);

      if (docid_out != NULL)
        *docid_out = id;

      return TRUE;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      if (docid_out != NULL)
        *docid_out = 0;

      return FALSE;
    }
}

/**
 * xapian_writable_database_delete_document:
 * @self: a #XapianWritableDatabase
 * @docid: the document to delete
 * @error: return location for a #GError
 *
 * Deletes the document with the given @docid from a database.
 *
 * Returns: %TRUE if the document was successfully deleted
 */
gboolean
xapian_writable_database_delete_document (XapianWritableDatabase *self,
                                          unsigned int            docid,
                                          GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  g_assert (xapian_database_get_is_writable (XAPIAN_DATABASE (self)));
#endif

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

      write_db->delete_document (docid);

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
 * xapian_writable_database_replace_document:
 * @self: a #XapianWritableDatabase
 * @docid: the id of the document to replace
 * @document: a #XapianDocument
 * @error: return location for a #GError
 *
 * Replaces the #XapianDocument with @docid inside a
 * #XapianWritableDatabase with the given @document.
 *
 * Returns: %TRUE if the document was replaced
 */
gboolean
xapian_writable_database_replace_document (XapianWritableDatabase *self,
                                           unsigned int            docid,
                                           XapianDocument         *document,
                                           GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);
  g_return_val_if_fail (XAPIAN_IS_DOCUMENT (document), FALSE);

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  g_assert (xapian_database_get_is_writable (XAPIAN_DATABASE (self)));
#endif

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);
      Xapian::Document doc = *xapian_document_get_internal (document);

      write_db->replace_document (docid, doc);

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
 * xapian_writable_database_begin_transaction:
 * @self: a #XapianWritableDatabase
 * @flushed: whether the transaction should be permanently stored
 *   in the database when committed, or if the transaction should
 *   only be applied to the database
 * @error: return location for a #GError
 *
 * Begins a transaction in the database.
 *
 * See also: xapian_writable_database_commit_transaction(),
 *   xapian_writable_database_cancel_transaction()
 *
 * Returns: %TRUE if the transaction can be started, and
 *   %FALSE if the backend does not support transactions or
 *   if a transaction is already in progress
 */
gboolean
xapian_writable_database_begin_transaction (XapianWritableDatabase *self,
                                            gboolean                flushed,
                                            GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);
      bool isFlushed = !!flushed;

      write_db->begin_transaction (isFlushed);

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
 * xapian_writable_database_commit_transaction:
 * @self: a #XapianWritableDatabase
 * @error: return location for a #GError
 *
 * Commits the currently pending transaction.
 *
 * If the transaction was started by passing %TRUE to the `flushed`
 * argument of xapian_writable_database_begin_transaction() then all
 * the changes caused by the transaction will be permanently stored
 * inside the database.
 *
 * In case of error, this function will return %FALSE and set @error.
 *
 * Returns: %TRUE if the transaction was successfully committed.
 */
gboolean
xapian_writable_database_commit_transaction (XapianWritableDatabase *self,
                                             GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

      write_db->commit_transaction ();

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
 * xapian_writable_database_cancel_transaction:
 * @self: a #XapianWritableDatabase
 * @error: return location for a #GError
 *
 * Cancels the currently pending transaction.
 *
 * Returns: %TRUE if the transaction was successfully cancelled
 */
gboolean
xapian_writable_database_cancel_transaction (XapianWritableDatabase *self,
                                             GError                **error)
{
  g_return_val_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self), FALSE);

  try
    {
      Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

      write_db->cancel_transaction ();

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
 * xapian_writable_database_add_spelling:
 * @self: a #XapianWritableDatabase
 * @word: The word to add
 *
 * Add a word to the spelling dictionary with a default frequency increase of 1.
 *
 * If the word is already present, its frequency is increased.
 *
 * Since: 1.2
 */
void
xapian_writable_database_add_spelling (XapianWritableDatabase *self,
                                       const char             *word)
{
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self));
  Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

  write_db->add_spelling (word);
}

/**
 * xapian_writable_database_add_spelling_full:
 * @self: a #XapianWritableDatabase
 * @word: The word to add
 * @freqinc: How much to increase its frequency by
 *
 * Add a word to the spelling dictionary.
 *
 * If the word is already present, its frequency is increased.
 *
 * Since: 1.2
 */
void
xapian_writable_database_add_spelling_full (XapianWritableDatabase *self,
                                            const char             *word,
                                            unsigned int            freqinc)
{
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self));
  Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

  write_db->add_spelling (word, freqinc);
}

/**
 * xapian_writable_database_remove_spelling:
 * @self: a #XapianWritableDatabase
 * @word: The word to remove
 *
 * Remove a word from the spelling dictionary.
 * 
 * The word's frequency is decreased by 1, and if would become zero or less then
 * the word is removed completely.
 *
 * Since: 1.2
 */
void
xapian_writable_database_remove_spelling (XapianWritableDatabase *self,
                                          const char             *word)
{
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self));
  Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

  write_db->remove_spelling (word);
}

/**
 * xapian_writable_database_remove_spelling_full:
 * @self: a #XapianWritableDatabase
 * @word: The word to remove
 * @freqdec: How much to decrease its frequency by
 * 
 * Remove a word from the spelling dictionary.
 *
 * The word's frequency is decreased, and if would become zero or less then the
 * word is removed completely.
 *
 * Since: 1.2
 */
void
xapian_writable_database_remove_spelling_full (XapianWritableDatabase *self,
                                               const char             *word,
                                               unsigned int            freqdec)
{
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self));
  Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

  write_db->remove_spelling (word, freqdec);
}

/**
 * xapian_writable_database_set_metadata:
 * @self: a #XapianWritableDatabase
 * @key: A key in the database's metadata
 * @value: The value to set for @key
 * 
 * Set the user-specified metadata associated with a given key.
 *
 * This method sets the metadata value associated with a given key. If there is
 * already a metadata value stored in the database with the same key, the old
 * value is replaced. If you want to delete an existing item of metadata, just
 * set its value to the empty string.
 *
 * Since: 1.4
 */
void
xapian_writable_database_set_metadata (XapianWritableDatabase *self,
                                       const char             *key,
                                       const char             *value)
{
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (self));
  Xapian::WritableDatabase *write_db = xapian_writable_database_get_internal (self);

  write_db->set_metadata (key, value);
}
