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
 * SECTION:xapian-stopper
 * @Title: XapianStopper
 * @short_description: stopper
 *
 * Abstract class for stop-word decision functor.
 */

#include "config.h"

#include "xapian-stopper.h"
#include "xapian-stopper-private.h"
#include "xapian-error-private.h"

#define XAPIAN_STOPPER_GET_PRIVATE(obj) \
  ((XapianStopperPrivate *) xapian_stopper_get_instance_private ((XapianStopper *) (obj)))

typedef struct _XapianStopperPrivate   XapianStopperPrivate;

struct _XapianStopperPrivate
{
  Xapian::Stopper *mStopper;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (XapianStopper, xapian_stopper, G_TYPE_OBJECT)

static void
xapian_stopper_finalize (GObject *gobject)
{
  G_OBJECT_CLASS (xapian_stopper_parent_class)->finalize (gobject);
}

/*< private >
 * xapian_stopper_get_internal:
 * @self: a #XapianStopper
 *
 * Retrieves the `Xapian::Stopper` object used by @self.
 *
 * Returns: (transfer none): a pointer to the internal stopper instance
 */
Xapian::Stopper *
xapian_stopper_get_internal (XapianStopper *self)
{
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (self);

  return priv->mStopper;
}

/*< private >
 * xapian_stopper_set_internal:
 * @self: a #XapianStopper
 * @aDB: a `Xapian::Stopper` instance
 *
 * Sets the internal stopper instance wrapped by @self, clearing
 * any existing instance if needed.
 */
void
xapian_stopper_set_internal (XapianStopper   *self,
                             Xapian::Stopper *aStopper)
{
  XapianStopperPrivate *priv = XAPIAN_STOPPER_GET_PRIVATE (self);

  delete priv->mStopper;

  priv->mStopper = aStopper;
}

char *
xapian_stopper_get_description (XapianStopper *self)
{
  Xapian::Stopper *stopper = xapian_stopper_get_internal(self);

  std::string desc = stopper->get_description();

  return g_strdup (desc.c_str ());
}

static void
xapian_stopper_dispose (GObject *gobject)
{
  G_OBJECT_CLASS (xapian_stopper_parent_class)->dispose (gobject);
}

static void
xapian_stopper_class_init (XapianStopperClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->dispose = xapian_stopper_dispose;
  gobject_class->finalize = xapian_stopper_finalize;
}

static void
xapian_stopper_init (XapianStopper *stopper)
{
}
