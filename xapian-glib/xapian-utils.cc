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
