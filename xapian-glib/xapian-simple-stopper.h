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

#ifndef __XAPIAN_GLIB_SIMPLE_STOPPER_H__
#define __XAPIAN_GLIB_SIMPLE_STOPPER_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-stopper.h"

G_BEGIN_DECLS

#define XAPIAN_SIMPLE_STOPPER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_SIMPLE_STOPPER, XapianSimpleStopper))
#define XAPIAN_IS_SIMPLE_STOPPER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_SIMPLE_STOPPER))
#define XAPIAN_SIMPLE_STOPPER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_SIMPLE_STOPPER, XapianSimpleStopperClass))
#define XAPIAN_IS_SIMPLE_STOPPER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_SIMPLE_STOPPER))
#define XAPIAN_SIMPLE_STOPPER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_SIMPLE_STOPPER, XapianSimpleStopperClass))

typedef struct _XapianSimpleStopper           XapianSimpleStopper;
typedef struct _XapianSimpleStopperClass      XapianSimpleStopperClass;

struct _XapianSimpleStopper
{
  XapianStopper parent_instance;
};

struct _XapianSimpleStopperClass
{
  XapianStopperClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_2
GType xapian_simple_stopper_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianSimpleStopper * xapian_simple_stopper_new (GError **error);


XAPIAN_GLIB_AVAILABLE_IN_1_2
void xapian_simple_stopper_add (XapianSimpleStopper *stopper,
                                const gchar *word);

G_END_DECLS

#endif /* __XAPIAN_GLIB_SIMPLE_STOPPER_H__ */
