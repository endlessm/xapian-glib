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
 * @short_description:  stopper
 */

#include "config.h"

#include "xapian-stopper.h"
#include "xapian-stopper-private.h"
#include "xapian-error-private.h"

G_DEFINE_ABSTRACT_TYPE (XapianStopper, xapian_stopper, G_TYPE_OBJECT)

static void
xapian_stopper_finalize (GObject *gobject)
{
  G_OBJECT_CLASS (xapian_stopper_parent_class)->finalize (gobject);
}

Xapian::Stopper* xapian_stopper_get_internal (XapianStopper *self) {
    XapianStopperClass *klass = XAPIAN_STOPPER_GET_CLASS(self);
    return static_cast<Xapian::Stopper*>(klass->get_internal(self));
}

char* xapian_stopper_get_description (XapianStopper *self) {
  Xapian::Stopper *stopper = static_cast<Xapian::Stopper*>(XAPIAN_STOPPER_GET_CLASS(self)->get_internal(self));
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
