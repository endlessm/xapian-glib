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

#include "xapian-enquire.h"

#include "xapian-database-private.h"
#include "xapian-error-private.h"

#define XAPIAN_ENQUIRE_GET_PRIVATE(obj) \
  ((XapianEnquirePrivate *) xapian_enquire_get_instance_private ((XapianEnquire *) (obj)))

typedef struct {
  Xapian::Enquire mEnquire;

  XapianDatabase *database;
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

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  g_assert (priv->database == NULL);
#endif

  priv->database = (XapianDatabase *) g_object_ref (db);
}

static gboolean
xapian_enquire_init_internal (GInitable *self,
                              GCancellable *cancellable,
                              GError **error)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (self);

  if (priv->database == NULL)
    {
      g_set_error_literal (error, XAPIAN_ERROR_TYPE,
                           XAPIAN_ERROR_TYPE_INVALID_ARGUMENT,
                           "XapianEnquire:database must not be empty or NULL");
      return FALSE;
    }

  try
    {
      Xapian::Database database = *xapian_database_get_internal (priv->database);

      priv->mEnquire = Xapian::Enquire (database);
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
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_enquire_init_internal;
}

static void
xapian_enquire_set_property (GObject *gobject,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
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
xapian_enquire_get_property (GObject *gobject,
                             guint prop_id,
                             GValue *value,
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
xapian_enquire_dispose (GObject *gobject)
{
  XapianEnquirePrivate *priv = XAPIAN_ENQUIRE_GET_PRIVATE (gobject);

  g_clear_object (&priv->database);

  G_OBJECT_CLASS (xapian_enquire_parent_class)->dispose (gobject);
}

static void
xapian_enquire_class_init (XapianEnquireClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

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

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_enquire_init (XapianEnquire *self)
{
}

XapianEnquire *
xapian_enquire_new (XapianDatabase *db,
                    GError        **error)
{
  g_return_val_if_fail (XAPIAN_IS_DATABASE (db), NULL);

  return (XapianEnquire *) g_initable_new (XAPIAN_TYPE_ENQUIRE,
                                           NULL, error,
                                           "database", db,
                                           NULL);
}
