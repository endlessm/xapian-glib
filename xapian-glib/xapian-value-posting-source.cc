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
#include "xapian-error-private.h"

G_DEFINE_ABSTRACT_TYPE (XapianValuePostingSource, xapian_value_posting_source, G_TYPE_OBJECT)

static void
xapian_value_posting_source_finalize (GObject *gobject)
{
  G_OBJECT_CLASS (xapian_value_posting_source_parent_class)->finalize (gobject);
}

static void
xapian_value_posting_source_class_init (XapianValuePostingSourceClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->finalize = xapian_value_posting_source_finalize;
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
  return static_cast<XapianValuePostingSource *> (g_initable_new (XAPIAN_TYPE_VALUE_POSTING_SOURCE,
                                                       NULL, error,
                                                       "slot_", slot_,
                                                       NULL));
}
