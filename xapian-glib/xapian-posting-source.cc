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
 * SECTION:xapian-posting-source
 * @Title: XapianPostingSource
 * @short_description: Posting source
 */

#include "config.h"

#include "xapian-posting-source.h"
#include "xapian-posting-source-private.h"
#include "xapian-error-private.h"

G_DEFINE_ABSTRACT_TYPE (XapianPostingSource, xapian_posting_source, G_TYPE_OBJECT)

/*< private >
 * xapian_posting_source_get_internal:
 * @self: a #XapianPostingSource
 *
 * Retrieves the `Xapian::PostingSource` object used by @self.
 *
 * Returns: (transfer none): a pointer to the internal database instance
 *
 * Since: 1.2
 */
Xapian::PostingSource*
xapian_posting_source_get_internal (XapianPostingSource *self) 
{
  XapianPostingSourceClass *klass = XAPIAN_POSTING_SOURCE_GET_CLASS(self);

  return static_cast<Xapian::PostingSource*>(klass->get_internal(self));
}

/*< private >
 * xapian_posting_source_get_description:
 * @self: a #XapianPostingSource
 *
 * Retrieves a string describing the #XapianPostingSource.
 *
 * Typically, this function is used when debugging.
 *
 * Returns: (transfer full): a description of the database
 *
 * Since: 1.2
 */
char*
xapian_posting_source_get_description (XapianPostingSource *self)
{
  Xapian::PostingSource *posting_source = static_cast<Xapian::PostingSource*>(XAPIAN_POSTING_SOURCE_GET_CLASS(self)->get_internal(self));
  std::string desc = posting_source->get_description();

  return g_strdup (desc.c_str ());
}

static void
xapian_posting_source_class_init (XapianPostingSourceClass *klass)
{
}

static void
xapian_posting_source_init (XapianPostingSource *posting_source)
{
}
