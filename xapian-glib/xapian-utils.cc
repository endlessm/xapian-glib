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

const char *
xapian_glib_version_string (void)
{
  return Xapian::version_string ();
}

int
xapian_glib_major_version (void)
{
  return Xapian::major_version ();
}

int
xapian_glib_minor_version (void)
{
  return Xapian::minor_version ();
}

int
xapian_glib_revision (void)
{
  return Xapian::revision ();
}
