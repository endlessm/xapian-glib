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

#include "xapian-enums.h"

#define XAPIAN_GLIB_DEFINE_ENUM_VALUE(value,nick) \
  { value, #value, nick },

#define XAPIAN_GLIB_DEFINE_ENUM_TYPE(TypeName,type_name,values) \
GType \
type_name ## _get_type (void) \
{ \
  static volatile gsize g_define_id__volatile = 0; \
  if (g_once_init_enter (&g_define_id__volatile)) \
    { \
      static const GEnumValue v[] = { \
        values \
        { 0, NULL, NULL }, \
      }; \
      GType g_define_id = g_enum_register_static (g_intern_static_string (#TypeName), v); \
      g_once_init_leave (&g_define_id__volatile, g_define_id); \
    } \
  return g_define_id__volatile; \
}

#define XAPIAN_GLIB_DEFINE_FLAGS_TYPE(TypeName,type_name,values) \
GType \
type_name ## _get_type (void) \
{ \
  static volatile gsize g_define_id__volatile = 0; \
  if (g_once_init_enter (&g_define_id__volatile)) \
    { \
      static const GFlagsValue v[] = { \
        values \
	{ 0, NULL, NULL }, \
      }; \
      GType g_define_id = g_flags_register_static (g_intern_static_string (#TypeName), v); \
      g_once_init_leave (&g_define_id__volatile, g_define_id); \
    } \
  return g_define_id__volatile; \
}

XAPIAN_GLIB_DEFINE_ENUM_TYPE (XapianDatabaseAction, xapian_database_action,
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN, "create-or-open")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_DATABASE_ACTION_CREATE, "create")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE, "create-or-overwrite")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_DATABASE_ACTION_OPEN, "open"))

XAPIAN_GLIB_DEFINE_ENUM_TYPE (XapianErrorType, xapian_error_type,
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_ASSERTION, "assertion")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_INVALID_ARGUMENT, "invalid-argument")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_INVALID_OPERATION, "invalid-operation")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_UNIMPLEMENTED, "unimplemented")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE, "database")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_CORRUPT, "database-corrupt")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_CREATE, "database-create")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_LOCK, "database-lock")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_MODIFIED, "database-modified")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_OPENING, "database-opening")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DATABASE_VERSION, "database-version")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_DOC_NOT_FOUND, "doc-not-found")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_FEATURE_UNAVAILABLE, "feature-unavailable")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_INTERNAL, "internal")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_NETWORK, "network")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_NETWORK_TIMEOUT, "network-timeout")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_QUERY_PARSER, "query-parser")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_SERIALISATION, "serialisation")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_RANGE, "range")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_ERROR_TYPE_LAST, "type-last"))

XAPIAN_GLIB_DEFINE_ENUM_TYPE (XapianQueryOp, xapian_query_op,
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_AND, "and")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_OR, "or")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_AND_NOT, "and-not")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_XOR, "xor")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_AND_MAYBE, "and-maybe")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_FILTER, "filter")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_NEAR, "near")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_PHRASE, "phrase")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_VALUE_RANGE, "value-range")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_SCALE_WEIGHT, "scale-weight")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_ELITE_SET, "elite-set")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_VALUE_GE, "value-ge")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_VALUE_LE, "value-le")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_SYNONYM, "synonym")
  XAPIAN_GLIB_DEFINE_ENUM_VALUE (XAPIAN_QUERY_OP_LAST, "op-last"))
