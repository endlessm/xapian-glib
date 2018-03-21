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

#ifndef __XAPIAN_GLIB_STOPPER_H__
#define __XAPIAN_GLIB_STOPPER_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_STOPPER     (xapian_stopper_get_type())

XAPIAN_GLIB_AVAILABLE_IN_2_0
G_DECLARE_DERIVABLE_TYPE (XapianStopper, xapian_stopper, XAPIAN, STOPPER, GObject)

/**
 * XapianStopperClass:
 * @is_stop_term: virtual function for querying a #XapianStopper if
 *   a term is a stop word
 * @get_description: virtual function for retrieving the description
 *   string of a #XapianStopper implementation
 *
 * The `XapianStopperClass` structure contains only private data.
 *
 * Since: 2.0
 */
struct _XapianStopperClass
{
  /*< private >*/
  GObjectClass parent_instance;

  /*< public >*/
  gboolean (* is_stop_term)    (XapianStopper *self,
                                const char    *term);
  char *   (* get_description) (XapianStopper *self);

  /*< private >*/
  gpointer _padding[8];
};

XAPIAN_GLIB_AVAILABLE_IN_2_0
gchar *         xapian_stopper_get_description  (XapianStopper *self);

XAPIAN_GLIB_AVAILABLE_IN_2_0
gboolean        xapian_stopper_is_stop_term     (XapianStopper *self,
                                                 const char    *term);

G_END_DECLS

#endif /* __XAPIAN_GLIB_STOPPER_H__ */
