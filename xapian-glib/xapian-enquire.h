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
#include "xapian-database.h"
#include "xapian-query.h"
#include "xapian-mset.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_ENQUIRE     (xapian_enquire_get_type())

XAPIAN_GLIB_AVAILABLE_IN_1_0
G_DECLARE_DERIVABLE_TYPE (XapianEnquire, xapian_enquire, XAPIAN, ENQUIRE, GObject)

struct _XapianEnquireClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianEnquire * xapian_enquire_new                    (XapianDatabase *db,
                                                       GError        **error);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void            xapian_enquire_set_collapse_key       (XapianEnquire *enquire,
                                                       unsigned int   collapse_key);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void            xapian_enquire_set_collapse_key_full  (XapianEnquire *enquire,
                                                       unsigned int   collapse_key,
                                                       unsigned int   collapse_max);
XAPIAN_GLIB_AVAILABLE_IN_1_2
void            xapian_enquire_set_cutoff             (XapianEnquire *enquire,
                                                       gfloat         percent_cutoff);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void            xapian_enquire_set_cutoff_full        (XapianEnquire *enquire,
                                                       gfloat         percent_cutoff,
                                                       unsigned int   weight_cutoff);

XAPIAN_GLIB_AVAILABLE_IN_1_2
void            xapian_enquire_set_sort_by_value      (XapianEnquire *enquire,
                                                       unsigned int   sort_key,
                                                       gboolean       reverse);

XAPIAN_GLIB_AVAILABLE_IN_1_0
void            xapian_enquire_set_query              (XapianEnquire *enquire,
                                                       XapianQuery   *query,
                                                       unsigned int   qlen);
XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *   xapian_enquire_get_query              (XapianEnquire *enquire);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianMSet *    xapian_enquire_get_mset               (XapianEnquire *enquire,
                                                       unsigned int   first,
                                                       unsigned int   max_items,
                                                       GError       **error);

G_END_DECLS

#endif /* __XAPIAN_GLIB_ENQUIRE_H__ */
