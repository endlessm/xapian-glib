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

#include "xapian-error-private.h"
#include "xapian-enums.h"

static const char *xapian_error_types[XAPIAN_ERROR_TYPE_LAST] = { NULL, };
static gboolean xapian_error_types_set = FALSE;

static void
init_error_types (void)
{
  if (G_LIKELY (xapian_error_types_set))
    return;

#define XAPIAN_SET_ERROR_TYPE(id,name) \
  xapian_error_types[(int) XAPIAN_ERROR_TYPE_ ## id] = g_intern_static_string (#name)

  XAPIAN_SET_ERROR_TYPE (ASSERTION, AssertionError);
  XAPIAN_SET_ERROR_TYPE (INVALID_ARGUMENT, InvalidArgumentError);
  XAPIAN_SET_ERROR_TYPE (INVALID_OPERATION, InvalidOperationErrror);
  XAPIAN_SET_ERROR_TYPE (UNIMPLEMENTED, UnimplementedError);
  XAPIAN_SET_ERROR_TYPE (DATABASE, DatabaseError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_CORRUPT, DatabaseCorruptError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_CREATE, DatabaseCreateError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_LOCK, DatabaseLockError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_MODIFIED, DatabaseModifiedError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_OPENING, DatabaseOpeningError);
  XAPIAN_SET_ERROR_TYPE (DATABASE_VERSION, DatabaseVersionError);
  XAPIAN_SET_ERROR_TYPE (DOC_NOT_FOUND, DocNotFoundError);
  XAPIAN_SET_ERROR_TYPE (FEATURE_UNAVAILABLE, FeatureUnavailableError);
  XAPIAN_SET_ERROR_TYPE (INTERNAL, InternalError);
  XAPIAN_SET_ERROR_TYPE (NETWORK, NetworkError);
  XAPIAN_SET_ERROR_TYPE (NETWORK_TIMEOUT, NetworkTimeoutError);
  XAPIAN_SET_ERROR_TYPE (QUERY_PARSER, QueryParserError);
  XAPIAN_SET_ERROR_TYPE (SERIALISATION, SerialisationError);
  XAPIAN_SET_ERROR_TYPE (RANGE, RangeError);

#undef XAPIAN_SET_ERROR_TYPE

  xapian_error_types_set = TRUE;
}

G_DEFINE_QUARK (xapian-error-type-quark, xapian_error_type)

void
xapian_error_to_gerror (const Xapian::Error  &src,
                        GError              **dest)
{
  const char *src_type = g_intern_string (src.get_type ());
  XapianErrorType error_type = XAPIAN_ERROR_TYPE_LAST;

  init_error_types ();

  for (int i = 0; i < G_N_ELEMENTS (xapian_error_types); i++)
    {
      if (src_type == xapian_error_types[i])
        {
          error_type = (XapianErrorType) i;
          break;
        }
    }

  /* unknown error in the bindings */
  if (error_type == XAPIAN_ERROR_TYPE_LAST)
    {
      g_critical ("Unknown error type %d.\n", error_type);
      return;
    }

  const std::string src_msg = src.get_msg();

  if (src.get_error_string () != NULL)
    {
      g_set_error (dest, XAPIAN_ERROR_TYPE, error_type,
                   "%s (error: %s)",
                   src_msg.c_str (),
                   src.get_error_string ());
    }
  else
    g_set_error_literal (dest, XAPIAN_ERROR_TYPE, error_type, src_msg.c_str ());
}
