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

#include <string.h>
#include <string>

#include "xapian-writable-database.h"
#include "xapian-database-private.h"
#include "xapian-document-private.h"
#include "xapian-error-private.h"
#include "xapian-enums.h"

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
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_default_init))

static Xapian::WritableDatabase *
xapian_writable_database_get_internal (XapianWritableDatabase *self)
{
  Xapian::Database *db = xapian_database_get_internal (XAPIAN_DATABASE (self));
  Xapian::WritableDatabase *write_db = dynamic_cast<Xapian::WritableDatabase *> (db);

  return write_db;
}

static gboolean
xapian_writable_database_init_internal (GInitable *initable,
                                        GCancellable *cancellable,
                                        GError **error)
{
  XapianWritableDatabase *self = XAPIAN_WRITABLE_DATABASE (initable);
  XapianWritableDatabasePrivate *priv;
  
  priv = (XapianWritableDatabasePrivate *) xapian_writable_database_get_instance_private (self);

  Xapian::WritableDatabase db;

  try
    {
      const char *path = xapian_database_get_path (XAPIAN_DATABASE (self));

      if (path == NULL)
        db = Xapian::WritableDatabase ();
      else
        {
          std::string file (path, strlen (path));

          db = Xapian::WritableDatabase (file, (int) priv->action);
        }
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return FALSE;
    }

  xapian_database_set_internal (XAPIAN_DATABASE (self), db);
  xapian_database_set_is_writable (XAPIAN_DATABASE (self), TRUE);

  return TRUE;
}

static void
initable_default_init (GInitableIface *iface)
{
  iface->init = xapian_writable_database_init_internal;
}

static void
xapian_writable_database_finalize (GObject *gobject)
{
  G_OBJECT_CLASS (xapian_writable_database_parent_class)->finalize (gobject);
}

static void
xapian_writable_database_set_property (GObject *gobject,
                                       guint prop_id,
                                       const GValue *value,
                                       GParamSpec *pspec)
{
  XapianWritableDatabase *self = XAPIAN_WRITABLE_DATABASE (gobject);
  XapianWritableDatabasePrivate *priv;

  priv = (XapianWritableDatabasePrivate *) xapian_writable_database_get_instance_private (self);

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
xapian_writable_database_get_property (GObject *gobject,
                                       guint prop_id,
                                       GValue *value,
                                       GParamSpec *pspec)
{
  XapianWritableDatabase *self = XAPIAN_WRITABLE_DATABASE (gobject);
  XapianWritableDatabasePrivate *priv;

  priv = (XapianWritableDatabasePrivate *) xapian_writable_database_get_instance_private (self);

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
  gobject_class->finalize = xapian_writable_database_finalize;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_writable_database_init (XapianWritableDatabase *self)
{
}

XapianWritableDatabase *
xapian_writable_database_new (const char            *path,
                              XapianDatabaseAction   action,
                              GError               **error)
{
  return (XapianWritableDatabase *) g_initable_new (XAPIAN_TYPE_WRITABLE_DATABASE,
                                                    NULL, error,
                                                    "path", path,
                                                    "action", action,
                                                    NULL);
}

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
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return FALSE;
    }

  return TRUE;
}

/**
 * xapian_writable_database_add_document:
 * @self: ...
 * @document: ...
 * @docid_out: (out): ...
 * @error: ..
 *
 * ...
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
