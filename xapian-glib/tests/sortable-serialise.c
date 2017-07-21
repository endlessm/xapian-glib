/* Copyright 2017  Olly Betts
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

#include <glib.h>
#include "xapian-glib.h"

static void
sortable_serialise_roundtrip (void)
{
  static const double values[] = {
    -1234.0,
    0.0,
    0.125,
    2049.0, /* Serialised form contains embedded zero byte. */
  };
  size_t i;

  for (i = 0; i != G_N_ELEMENTS (values); ++i)
    {
      gsize len;
      guchar *s = xapian_sortable_serialise (values[i], &len);
      g_assert_cmpfloat (xapian_sortable_unserialise (s, len), ==, values[i]);
      g_free (s);
    }
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/sortable-serialise/roundtrip", sortable_serialise_roundtrip);

  return g_test_run ();
}
