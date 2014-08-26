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

#define XAPIAN_STOPPER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_STOPPER, XapianStopper))
#define XAPIAN_IS_STOPPER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_STOPPER))
#define XAPIAN_STOPPER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_STOPPER, XapianStopperClass))
#define XAPIAN_IS_STOPPER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_STOPPER))
#define XAPIAN_STOPPER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_STOPPER, XapianStopperClass))

typedef struct _XapianStopper           XapianStopper;
typedef struct _XapianStopperClass      XapianStopperClass;

struct _XapianStopper
{
  GObject parent_instance;
};

struct _XapianStopperClass
{
  /*< private >*/
  GObjectClass parent_instance;

  /*< public >*/
  gboolean (* is_stop_term)    (XapianStopper *stopper,
                                const char    *term);
  char *   (* get_description) (XapianStopper *stopper);

  /*< private >*/
  gpointer _padding[8];
};

XAPIAN_GLIB_AVAILABLE_IN_1_2
GType xapian_stopper_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_2
gchar *         xapian_stopper_get_description  (XapianStopper *self);

XAPIAN_GLIB_AVAILABLE_IN_1_2
gboolean        xapian_stopper_is_stop_term     (XapianStopper *self,
                                                 const char    *term);

G_END_DECLS

#endif /* __XAPIAN_GLIB_STOPPER_H__ */
