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

#include "config.h"

#include <xapian.h>

#include "xapian-utils.h"

#include <cstring>

/**
 * SECTION:xapian-utils
 * @Title: Utilities
 * @Short_description: Utility API
 *
 * Utility functions.
 */

/**
 * xapian_glib_version_string:
 *
 * The version string of the Xapian GLib bindings,
 * e.g. "1.2.3"
 *
 * Returns: (transfer none): the version string
 */
const char *
xapian_glib_version_string (void)
{
  return XAPIAN_GLIB_VERSION_S;
}

/**
 * xapian_glib_major_version:
 *
 * The major version of the Xapian GLib bindings,
 * e.g. 1 in 1.2.3
 */
int
xapian_glib_major_version (void)
{
  return XAPIAN_GLIB_MAJOR_VERSION;
}

/**
 * xapian_glib_minor_version:
 *
 * The minor version of the Xapian GLib bindings,
 * e.g. 2 in 1.2.3
 */
int
xapian_glib_minor_version (void)
{
  return XAPIAN_GLIB_MINOR_VERSION;
}

/**
 * xapian_glib_micro_version:
 *
 * The micro version of the Xapian GLib bindings,
 * e.g. 3 in 1.2.3
 */
int
xapian_glib_micro_version (void)
{
  return XAPIAN_GLIB_MICRO_VERSION;
}

/**
 * xapian_version_string:
 *
 * The version string of the underlying Xapian library,
 * e.g. "1.2.17".
 *
 * Returns: (transfer none): the version string
 */
const char *
xapian_version_string (void)
{
  return Xapian::version_string ();
}

/**
 * xapian_major_version:
 *
 * The major version of the underlying Xapian library,
 * e.g 1 in 1.2.17.
 *
 * Returns: the major version
 */
int
xapian_major_version (void)
{
  return Xapian::major_version ();
}

/**
 * xapian_minor_version:
 *
 * The minor version of the underlying Xapian library,
 * e.g. 2 in 1.2.17.
 *
 * Returns: the minor version
 */
int
xapian_minor_version (void)
{
  return Xapian::minor_version ();
}

/**
 * xapian_revision:
 *
 * The revision number of the underlying Xapian library,
 * e.g. 17 in 1.2.17
 */
int
xapian_revision (void)
{
  return Xapian::revision ();
}

/**
 * xapian_sortable_serialise:
 * @value: The numeric value to serialize
 * @len: (out): Used to return the length of the returned string
 *
 * Convert a floating point number to a string, preserving sort order. The
 * returned string may contain embedded zero bytes, so its length is also
 * returned.
 *
 * This method converts a floating point number to a string, suitable for using
 * as a value for numeric range restriction, or for use as a sort key.
 *
 * Returns: (transfer full) (array length=len): the serialised value
 *
 * Since: 1.4
 */
guchar *
xapian_sortable_serialise (double value, gsize *len)
{
  const std::string &result = Xapian::sortable_serialise (value);
  gsize size = result.size ();
  guchar *buf = static_cast<guchar *> (g_malloc (size));

  *len = size;
  std::memcpy (buf, result.data (), size);

  return buf;
}

/**
 * xapian_sortable_unserialise:
 * @value: (array length=len): The serialized value to be converted
 * @len: The length of the serialized value
 *
 * Convert a string encoded using sortable_serialise back to a floating point
 * number. The encoded string may contain embedded zero bytes, so its length
 * must also be passed.
 *
 * This expects the input to be a string produced by
 * xapian_sortable_serialise(). If the input is not such a string, the value
 * returned is undefined (but no error will be thrown).
 *
 * Returns: the unserialised numeric value
 *
 * Since: 1.4
 */
double
xapian_sortable_unserialise (const guchar *value, gsize len)
{
  const char *p = reinterpret_cast<const char*> (value);
  return Xapian::sortable_unserialise (std::string (p, len));
}
