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

#ifndef __XAPIAN_GLIB_VALUE_POSTING_SOURCE_H__
#define __XAPIAN_GLIB_VALUE_POSTING_SOURCE_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-posting-source.h"

G_BEGIN_DECLS

#define XAPIAN_VALUE_POSTING_SOURCE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_VALUE_POSTING_SOURCE, XapianValuePostingSource))
#define XAPIAN_IS_VALUE_POSTING_SOURCE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_VALUE_POSTING_SOURCE))
#define XAPIAN_VALUE_POSTING_SOURCE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_VALUE_POSTING_SOURCE, XapianValuePostingSourceClass))
#define XAPIAN_IS_VALUE_POSTING_SOURCE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_VALUE_POSTING_SOURCE))
#define XAPIAN_VALUE_POSTING_SOURCE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_VALUE_POSTING_SOURCE, XapianValuePostingSourceClass))

typedef struct _XapianValuePostingSource           XapianValuePostingSource;
typedef struct _XapianValuePostingSourceClass      XapianValuePostingSourceClass;

struct _XapianValuePostingSource
{
  XapianPostingSource parent_instance;
};

struct _XapianValuePostingSourceClass
{
  XapianPostingSourceClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_2
GType xapian_value_posting_source_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianValuePostingSource * xapian_value_posting_source_new (unsigned int slot, GError **error);

G_END_DECLS

#endif /* __XAPIAN_GLIB_VALUE_POSTING_SOURCE_H__ */
