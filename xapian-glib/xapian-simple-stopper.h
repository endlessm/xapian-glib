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

#define XAPIAN_TYPE_SIMPLE_STOPPER      (xapian_simple_stopper_get_type())

XAPIAN_GLIB_AVAILABLE_IN_1_2
G_DECLARE_DERIVABLE_TYPE (XapianSimpleStopper, xapian_simple_stopper, XAPIAN, SIMPLE_STOPPER, XapianStopper)

struct _XapianSimpleStopperClass
{
  XapianStopperClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianSimpleStopper * xapian_simple_stopper_new (void);


XAPIAN_GLIB_AVAILABLE_IN_1_2
void xapian_simple_stopper_add (XapianSimpleStopper *stopper,
                                const gchar *word);

G_END_DECLS

#endif /* __XAPIAN_GLIB_SIMPLE_STOPPER_H__ */
