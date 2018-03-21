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
 * SECTION:xapian-value-posting-source
 * @Title: XapianValuePostingSource
 * @short_description: Value posting source
 *
 * #XapianValuePostingSource is a #XapianPostingSource that generates
 * weights from a value slot. It is typically used as a base class
 * for classes that generate weights using values stored in the specified
 * slot. See #XapianValueWeightPostingSource for an implementation that
 * uses sortable_unserialise to convert values directly to weights.
 */

#include "config.h"

#include "xapian-error-private.h"
#include "xapian-posting-source-private.h"
#include "xapian-value-posting-source.h"

#define XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE(obj) \
  ((XapianValuePostingSourcePrivate *) xapian_value_posting_source_get_instance_private ((XapianValuePostingSource *) (obj)))

const unsigned int MIN_SLOT_VAL = 0;
const unsigned int MAX_SLOT_VAL = Xapian::BAD_VALUENO;

/*
 * Weights are generated from values stored in a specified slot
 * in the associated database.
 */
typedef struct _XapianValuePostingSourcePrivate XapianValuePostingSourcePrivate;

struct _XapianValuePostingSourcePrivate {
  unsigned int slot;
};

enum
{
  PROP_0,

  PROP_SLOT,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianValuePostingSource, xapian_value_posting_source,
                         XAPIAN_TYPE_POSTING_SOURCE,
                         G_ADD_PRIVATE(XapianValuePostingSource)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

static gboolean
xapian_value_posting_source_init_internal (GInitable    *self,
                                           GCancellable *cancellable,
                                           GError      **error)
{
  XapianPostingSource *pSource = XAPIAN_POSTING_SOURCE (self);
  XapianValuePostingSourcePrivate *priv = XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE (self);
  Xapian::ValuePostingSource *mValuePostingSource;

  try
    {
      mValuePostingSource = new Xapian::ValuePostingSource (priv->slot);
      xapian_posting_source_set_internal (pSource, (Xapian::PostingSource *) mValuePostingSource);

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
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_value_posting_source_init_internal;
}

static void
xapian_value_posting_source_init (XapianValuePostingSource *self)
{
}

static void
xapian_value_posting_source_set_property (GObject      *gobject,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  XapianValuePostingSourcePrivate *priv = XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_SLOT:
      priv->slot = g_value_get_uint (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_value_posting_source_get_property (GObject    *gobject,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  XapianValuePostingSourcePrivate *priv = XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_SLOT:
      g_value_set_uint (value, priv->slot);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_value_posting_source_class_init (XapianValuePostingSourceClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = xapian_value_posting_source_set_property;
  gobject_class->get_property = xapian_value_posting_source_get_property;

  obj_props[PROP_SLOT] =
        g_param_spec_uint ("slot",
                           "Slot",
                           "Slot in the database entries to use for source",
                           MIN_SLOT_VAL, MAX_SLOT_VAL, MIN_SLOT_VAL,
                           (GParamFlags) (G_PARAM_READWRITE |
                                          G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

/**
 * xapian_value_posting_source_new:
 * @slot: slot in the database entries to use for source
 * @error: return location for a #GError, or %NULL
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianValuePostingSource instance
 *
 * Since: 2.0
 */
XapianValuePostingSource *
xapian_value_posting_source_new (unsigned int slot,
                                 GError **error)
{
  return static_cast<XapianValuePostingSource *> (g_initable_new (XAPIAN_TYPE_VALUE_POSTING_SOURCE,
                                                                  NULL, error,
                                                                  "slot", slot,
                                                                  NULL));
}

/**
 * xapian_value_posting_source_get_slot:
 * @self: a #XapianValuePostingSource
 *
 * Accessor for the value of the #XapianValuePostingSource:slot property.
 *
 * Returns: the value of the #XapianValuePostingSource:slot property
 *
 * Since: 2.0
 */
guint
xapian_value_posting_source_get_slot (XapianValuePostingSource *self)
{
  XapianValuePostingSourcePrivate *priv = XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE (self);

  g_return_val_if_fail (XAPIAN_IS_VALUE_POSTING_SOURCE (self), 0);

  return priv->slot;
}
