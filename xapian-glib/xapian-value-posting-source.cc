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
 * SECTION:xapian-value-posting-source
 * @Title: XapianValuePostingSource
 * @short_description: Value posting source
 */

#include "config.h"

#include "xapian-value-posting-source.h"
#include "xapian-value-posting-source-private.h"
#include "xapian-error-private.h"


#define XAPIAN_VALUE_POSTING_SOURCE_GET_PRIVATE(obj) \
  ((XapianValuePostingSourcePrivate *) xapian_value_posting_source_get_instance_private ((XapianValuePostingSource *) (obj)))

typedef struct _XapianValuePostingSourcePrivate   XapianValuePostingSourcePrivate;

struct _XapianValuePostingSourcePrivate
{
};
G_DEFINE_ABSTRACT_TYPE_WITH_CODE (XapianValuePostingSource, xapian_value_posting_source, XAPIAN_TYPE_POSTING_SOURCE, G_ADD_PRIVATE(XapianValuePostingSource))

static void
xapian_value_posting_source_class_init (XapianValuePostingSourceClass *klass)
{
}

static void
xapian_value_posting_source_init (XapianValuePostingSource *posting_source)
{
}

/**
 * xapian_value_posting_source_new:
 * @error: return location for a #GError, or %NULL
 *
 * If the initializion failed, @error is set, and this function
 * will return %NULL.
 *
 * Returns: (transfer full): the newly created #XapianValuePostingSource instance
  *
 * Since: 1.2
 */
XapianValuePostingSource *xapian_value_posting_source_new (unsigned int slot_, GError**error)
{
  return static_cast<XapianValuePostingSource *> (g_object_new (XAPIAN_TYPE_VALUE_POSTING_SOURCE,
                                                       NULL, error,
                                                       NULL));
}
