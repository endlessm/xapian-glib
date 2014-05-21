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

#ifndef __XAPIAN_ENUMS_H__
#define __XAPIAN_ENUMS_H__

#include <glib-object.h>
#include "xapian-glib-macros.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_DATABASE_ACTION             (xapian_database_action_get_type ())

typedef enum {
  XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN = 1,
  XAPIAN_DATABASE_ACTION_CREATE = 2,
  XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE = 3,
  XAPIAN_DATABASE_ACTION_OPEN = 4
} XapianDatabaseAction;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_database_action_get_type (void);

#define XAPIAN_TYPE_ERROR_TYPE                  (xapian_error_type_get_type ())
#define XAPIAN_ERROR_TYPE                       (xapian_error_type_quark ())

typedef enum {
  XAPIAN_ERROR_TYPE_ASSERTION,
  XAPIAN_ERROR_TYPE_INVALID_ARGUMENT,
  XAPIAN_ERROR_TYPE_INVALID_OPERATION,
  XAPIAN_ERROR_TYPE_UNIMPLEMENTED,
  XAPIAN_ERROR_TYPE_DATABASE,
  XAPIAN_ERROR_TYPE_DATABASE_CORRUPT,
  XAPIAN_ERROR_TYPE_DATABASE_CREATE,
  XAPIAN_ERROR_TYPE_DATABASE_LOCK,
  XAPIAN_ERROR_TYPE_DATABASE_MODIFIED,
  XAPIAN_ERROR_TYPE_DATABASE_OPENING,
  XAPIAN_ERROR_TYPE_DATABASE_VERSION,
  XAPIAN_ERROR_TYPE_DOC_NOT_FOUND,
  XAPIAN_ERROR_TYPE_FEATURE_UNAVAILABLE,
  XAPIAN_ERROR_TYPE_INTERNAL,
  XAPIAN_ERROR_TYPE_NETWORK,
  XAPIAN_ERROR_TYPE_NETWORK_TIMEOUT,
  XAPIAN_ERROR_TYPE_QUERY_PARSER,
  XAPIAN_ERROR_TYPE_SERIALISATION,
  XAPIAN_ERROR_TYPE_RANGE,

  /*< private >*/
  XAPIAN_ERROR_TYPE_LAST
} XapianErrorType;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_error_type_get_type (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
GQuark xapian_error_type_quark (void);

#define XAPIAN_TYPE_QUERY_OP            (xapian_query_op_get_type ())

typedef enum {
  XAPIAN_QUERY_OP_AND,
  XAPIAN_QUERY_OP_OR,
  XAPIAN_QUERY_OP_AND_NOT,
  XAPIAN_QUERY_OP_XOR,
  XAPIAN_QUERY_OP_AND_MAYBE,
  XAPIAN_QUERY_OP_FILTER,
  XAPIAN_QUERY_OP_NEAR,
  XAPIAN_QUERY_OP_PHRASE,
  XAPIAN_QUERY_OP_VALUE_RANGE,
  XAPIAN_QUERY_OP_SCALE_WEIGHT,
  XAPIAN_QUERY_OP_ELITE_SET,
  XAPIAN_QUERY_OP_VALUE_GE,
  XAPIAN_QUERY_OP_VALUE_LE,
  XAPIAN_QUERY_OP_SYNONYM,

  /*< private >*/
  XAPIAN_QUERY_OP_LAST
} XapianQueryOp;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_query_op_get_type (void);

G_END_DECLS

#endif /* __XAPIAN_ENUMS_H__ */
