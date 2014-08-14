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
 * SECTION:xapian-simple-stopper
 * @Title: XapianSimpleStopper
 * @short_description: Simple stopper
 */

#include "config.h"

#include "xapian-simple-stopper.h"
#include "xapian-error-private.h"

#define XAPIAN_SIMPLE_STOPPER_GET_PRIVATE(obj) \
  ((XapianSimpleStopperPrivate *) xapian_simple_stopper_get_instance_private ((XapianSimpleStopper *) (obj)))

typedef struct {
  Xapian::SimpleStopper *mSimpleStopper;
} XapianSimpleStopperPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (XapianSimpleStopper, xapian_simple_stopper, XAPIAN_TYPE_STOPPER)

static void
xapian_simple_stopper_finalize (GObject *gobject)
{
  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (gobject);

  delete priv->mSimpleStopper;

  G_OBJECT_CLASS (xapian_simple_stopper_parent_class)->finalize (gobject);
}

static void
xapian_simple_stopper_dispose (GObject *gobject)
{
  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (gobject);

  G_OBJECT_CLASS (xapian_simple_stopper_parent_class)->dispose (gobject);
}

gpointer
xapian_simple_stopper_get_internal (XapianStopper *stopper)
{

  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (stopper);
  return static_cast<gpointer>(priv->mSimpleStopper);
}

static void
xapian_simple_stopper_class_init (XapianSimpleStopperClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  XapianStopperClass *xapian_stopper_class = XAPIAN_STOPPER_CLASS (klass);

  gobject_class->dispose = xapian_simple_stopper_dispose;
  gobject_class->finalize = xapian_simple_stopper_finalize;

  xapian_stopper_class->get_internal = xapian_simple_stopper_get_internal;
}

static void
xapian_simple_stopper_init (XapianSimpleStopper *stopper)
{
  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (stopper);

  priv->mSimpleStopper = new Xapian::SimpleStopper ();
}

void
xapian_simple_stopper_add (XapianSimpleStopper *stopper,
                           const gchar *word)
{
  g_return_if_fail (XAPIAN_IS_SIMPLE_STOPPER (stopper));

  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (stopper);

  priv->mSimpleStopper->add(word);
}

char *
xapian_simple_stopper_get_description (XapianSimpleStopper *stopper)
{
  g_return_val_if_fail (XAPIAN_IS_SIMPLE_STOPPER (stopper), NULL);

  XapianSimpleStopperPrivate *priv = XAPIAN_SIMPLE_STOPPER_GET_PRIVATE (stopper);

  std::string desc = priv->mSimpleStopper->get_description ();

  return g_strdup (desc.c_str ());
}

/**
 * xapian_simple_stopper_new:
 * @error: return location for a #GError, or %NULL
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianSimpleStopper instance
 */
XapianSimpleStopper *
xapian_simple_stopper_new (GError        **error)
{
  return static_cast<XapianSimpleStopper *> (g_initable_new (XAPIAN_TYPE_SIMPLE_STOPPER,
                                                       NULL, error,
                                                       NULL));
}
