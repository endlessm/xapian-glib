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

#ifndef __XAPIAN_GLIB_STEM_H__
#define __XAPIAN_GLIB_STEM_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_STEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_STEM, XapianStem))
#define XAPIAN_IS_STEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_STEM))
#define XAPIAN_STEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_STEM, XapianStemClass))
#define XAPIAN_IS_STEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_STEM))
#define XAPIAN_STEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_STEM, XapianStemClass))

typedef struct _XapianStemClass     XapianStemClass;

struct _XapianStem
{
  GObject parent_instance;
};

struct _XapianStemClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_stem_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianStem *    xapian_stem_new                         (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
XapianStem *    xapian_stem_new_for_language            (const char *language,
                                                         GError    **error);

XAPIAN_GLIB_AVAILABLE_IN_ALL
char **         xapian_stem_get_available_languages     (void);

G_END_DECLS

#endif /* __XAPIAN_GLIB_STEM_H__ */
