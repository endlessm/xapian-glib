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

#ifndef __XAPIAN_GLIB_ENQUIRE_H__
#define __XAPIAN_GLIB_ENQUIRE_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_ENQUIRE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_ENQUIRE, XapianEnquire))
#define XAPIAN_IS_ENQUIRE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_ENQUIRE))
#define XAPIAN_ENQUIRE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_ENQUIRE, XapianEnquireClass))
#define XAPIAN_IS_ENQUIRE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_ENQUIRE))
#define XAPIAN_ENQUIRE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_ENQUIRE, XapianEnquireClass))

typedef struct _XapianEnquireClass      XapianEnquireClass;

struct _XapianEnquire
{
  GObject parent_instance;
};

struct _XapianEnquireClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_enquire_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianEnquire * xapian_enquire_new              (XapianDatabase *db,
                                                 GError        **error);

XAPIAN_GLIB_AVAILABLE_IN_1_0
void            xapian_enquire_set_query        (XapianEnquire *enquire,
                                                 XapianQuery   *query,
                                                 unsigned int   qlen);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_enquire_get_query        (XapianEnquire *enquire);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianMSet *    xapian_enquire_get_mset         (XapianEnquire *enquire,
                                                 unsigned int   first,
                                                 unsigned int   max_items,
                                                 GError       **error);

G_END_DECLS

#endif /* __XAPIAN_GLIB_ENQUIRE_H__ */
