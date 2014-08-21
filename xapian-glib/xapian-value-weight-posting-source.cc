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
 * SECTION:xapian-value-weight-posting-source
 * @Title: XapianValueWeightPostingSource
 * @short_description: Value weight posting source
 * @long_description: A posting source which reads weights from a value slot.
 * This returns entries for all documents in the given database which have a 
 * non empty values in the specified slot.
 */

#include "config.h"
#include "xapian-value-weight-posting-source.h"
#include "xapian-error-private.h"

#define XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE(obj) \
  ((XapianValueWeightPostingSourcePrivate *) xapian_value_weight_posting_source_get_instance_private ((XapianValueWeightPostingSource *) (obj)))

const unsigned int MIN_SLOT_VAL = 0;
const unsigned int MAX_SLOT_VAL = Xapian::BAD_VALUENO;

/*
 * Weights are generated from values stored in a specified slot
 * in the associated database.
 */ 
typedef struct {
  unsigned int slot_;
  gboolean slot_set;
  Xapian::ValueWeightPostingSource *mValueWeightPostingSource;
} XapianValueWeightPostingSourcePrivate;

enum
{
  PROP_0,

  PROP_SLOT_,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };  

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianValueWeightPostingSource, xapian_value_weight_posting_source,
                         XAPIAN_TYPE_POSTING_SOURCE,
                         G_ADD_PRIVATE (XapianValueWeightPostingSource)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

static gboolean
xapian_value_weight_posting_source_init_internal (GInitable    *self,
                              GCancellable *cancellable,
                              GError      **error)
{
  XapianValueWeightPostingSourcePrivate *priv = XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE (self);
  try
    {
      if (priv->slot_set)
        {
          unsigned int slot_ = priv->slot_;

          priv->mValueWeightPostingSource = new Xapian::ValueWeightPostingSource(slot_);
        }
      else {
        priv->mValueWeightPostingSource = new Xapian::ValueWeightPostingSource(MIN_SLOT_VAL);
        priv->slot_set = TRUE;
      }

      return TRUE;
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      priv->mValueWeightPostingSource = NULL;

      return FALSE;
    }
}

static void
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_value_weight_posting_source_init_internal;
}

static void xapian_value_weight_posting_source_finalize (GObject *gobject)
{
  XapianValueWeightPostingSourcePrivate *priv = XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE(gobject);

  delete priv->mValueWeightPostingSource;

  G_OBJECT_CLASS (xapian_value_weight_posting_source_parent_class)->finalize (gobject);
}

static gpointer
xapian_value_weight_posting_source_get_internal (XapianPostingSource *posting_source)
{
  XapianValueWeightPostingSourcePrivate *priv = XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE(posting_source);

  return static_cast<gpointer>(priv->mValueWeightPostingSource);
}

static void xapian_value_weight_posting_source_init (XapianValueWeightPostingSource *posting_source)
{
}

static void
xapian_value_weight_posting_source_set_property (GObject      *gobject,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  XapianValueWeightPostingSourcePrivate *priv = XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_SLOT_:
      priv->slot_ = g_value_get_uint(value);
      priv->slot_set = TRUE;
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_value_weight_posting_source_get_property (GObject    *gobject,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  XapianValueWeightPostingSourcePrivate *priv = XAPIAN_VALUE_WEIGHT_POSTING_SOURCE_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_SLOT_:
      g_value_set_int (value, priv->slot_);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_value_weight_posting_source_class_init (XapianValueWeightPostingSourceClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  XapianPostingSourceClass *xapian_posting_source_class = XAPIAN_POSTING_SOURCE_CLASS(klass);

  gobject_class->set_property = xapian_value_weight_posting_source_set_property;
  gobject_class->get_property = xapian_value_weight_posting_source_get_property;
  gobject_class->finalize = xapian_value_weight_posting_source_finalize;

  obj_props[PROP_SLOT_] =
        g_param_spec_uint ("slot_",
                         "Slot",
                         "Slot in the database entries to use for source",
                         MIN_SLOT_VAL, MAX_SLOT_VAL, MIN_SLOT_VAL,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);

  xapian_posting_source_class->get_internal = xapian_value_weight_posting_source_get_internal;
}


/**
 * xapian_value_weight_posting_source_new:
 * @error: return location for a #GError, or %NULL
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianValueWeightPostingSource instance
 *
 * Since: 1.2
 */
XapianValueWeightPostingSource *
xapian_value_weight_posting_source_new (unsigned int slot_, GError **error)
{
  return static_cast<XapianValueWeightPostingSource *> (g_initable_new (XAPIAN_TYPE_VALUE_WEIGHT_POSTING_SOURCE,
                                                       NULL, error,
                                                       "slot_", slot_,
                                                       NULL));
}

