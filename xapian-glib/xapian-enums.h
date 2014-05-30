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

/**
 * XapianDatabaseAction:
 * @XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN: Create a database or open if it
 *   already exists
 * @XAPIAN_DATABASE_ACTION_CREATE: Create a database
 * @XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE: Create a database and overwrite
 *   it if one already exists
 * @XAPIAN_DATABASE_ACTION_OPEN: Open a database
 *
 * Actions for #XapianWritableDatabase.
 */
typedef enum {
  XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN = 1,
  XAPIAN_DATABASE_ACTION_CREATE = 2,
  XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE = 3,
  XAPIAN_DATABASE_ACTION_OPEN = 4
} XapianDatabaseAction;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_database_action_get_type (void);

#define XAPIAN_TYPE_ERROR                       (xapian_error_get_type ())

/**
 * XAPIAN_ERROR:
 *
 * Error domain for Xapian.
 */
#define XAPIAN_ERROR                            (xapian_error_quark ())

/**
 * XapianError:
 * @XAPIAN_ERROR_ASSERTION: Assertion failure
 * @XAPIAN_ERROR_INVALID_ARGUMENT: Invalid argument
 * @XAPIAN_ERROR_INVALID_OPERATION: Invalid operation
 * @XAPIAN_ERROR_UNIMPLEMENTED: Operation not implemented
 * @XAPIAN_ERROR_DATABASE: Database error
 * @XAPIAN_ERROR_DATABASE_CORRUPT: Database is corrupt
 * @XAPIAN_ERROR_DATABASE_CREATE: Failed to create a database
 * @XAPIAN_ERROR_DATABASE_LOCK: Failed to acquire the lock on a database
 * @XAPIAN_ERROR_DATABASE_MODIFIED: Database modified after opening
 * @XAPIAN_ERROR_DATABASE_OPENING: Unable to open the database
 * @XAPIAN_ERROR_DATABASE_VERSION: Version mismatch when opening the database
 * @XAPIAN_ERROR_DOC_NOT_FOUND: Document not found
 * @XAPIAN_ERROR_FEATURE_UNAVAILABLE: Feature not available with the current backend
 * @XAPIAN_ERROR_INTERNAL: Internal state error
 * @XAPIAN_ERROR_NETWORK: Network error
 * @XAPIAN_ERROR_NETWORK_TIMEOUT: Network timeout error
 * @XAPIAN_ERROR_QUERY_PARSER: Error when parsing a query string
 * @XAPIAN_ERROR_SERIALISATION: Error when serialising or deserialising data
 * @XAPIAN_ERROR_RANGE: Out of bounds access
 *
 * Error codes for the %XAPIAN_ERROR error domain.
 */
typedef enum {
  XAPIAN_ERROR_ASSERTION,
  XAPIAN_ERROR_INVALID_ARGUMENT,
  XAPIAN_ERROR_INVALID_OPERATION,
  XAPIAN_ERROR_UNIMPLEMENTED,
  XAPIAN_ERROR_DATABASE,
  XAPIAN_ERROR_DATABASE_CORRUPT,
  XAPIAN_ERROR_DATABASE_CREATE,
  XAPIAN_ERROR_DATABASE_LOCK,
  XAPIAN_ERROR_DATABASE_MODIFIED,
  XAPIAN_ERROR_DATABASE_OPENING,
  XAPIAN_ERROR_DATABASE_VERSION,
  XAPIAN_ERROR_DOC_NOT_FOUND,
  XAPIAN_ERROR_FEATURE_UNAVAILABLE,
  XAPIAN_ERROR_INTERNAL,
  XAPIAN_ERROR_NETWORK,
  XAPIAN_ERROR_NETWORK_TIMEOUT,
  XAPIAN_ERROR_QUERY_PARSER,
  XAPIAN_ERROR_SERIALISATION,
  XAPIAN_ERROR_RANGE,

  /*< private >*/
  XAPIAN_ERROR_LAST
} XapianError;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_error_get_type (void);
XAPIAN_GLIB_AVAILABLE_IN_ALL
GQuark xapian_error_quark (void);

#define XAPIAN_TYPE_QUERY_OP            (xapian_query_op_get_type ())

/**
 * XapianQueryOp:
 * @XAPIAN_QUERY_OP_AND: logical AND
 * @XAPIAN_QUERY_OP_OR: logical OR
 * @XAPIAN_QUERY_OP_AND_NOT: a AND NOT b
 * @XAPIAN_QUERY_OP_XOR: a XOR b
 * @XAPIAN_QUERY_OP_AND_MAYBE: a AND MAYBE b
 * @XAPIAN_QUERY_OP_FILTER: filter
 * @XAPIAN_QUERY_OP_NEAR: a NEAR b
 * @XAPIAN_QUERY_OP_PHRASE: "a b"
 * @XAPIAN_QUERY_OP_VALUE_RANGE: ...
 * @XAPIAN_QUERY_OP_SCALE_WEIGHT: ...
 * @XAPIAN_QUERY_OP_ELITE_SET: ...
 * @XAPIAN_QUERY_OP_VALUE_GE: ...
 * @XAPIAN_QUERY_OP_VALUE_LE: ...
 * @XAPIAN_QUERY_OP_SYNONYM: ...
 *
 * Operators for #XapianQuery.
 */
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

#define XAPIAN_TYPE_QUERY_PARSER_FEATURE        (xapian_query_parser_feature_get_type ())

/**
 * XapianQueryParserFeature:
 * @XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_PHRASE: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN_ANY_CASE: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_WILDCARD: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_PURE_NOT: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_PARTIAL: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_SPELLING_CORRECTION: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_SYNONYM: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_AUTO_SYNONYMS: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_AUTO_MULTIWORD_SYNONYMS: ...
 * @XAPIAN_QUERY_PARSER_FEATURE_DEFAULT: ...
 *
 * Flags for xapian_query_parser_parse_query_full().
 */
typedef enum {
  XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN = 1 << 0,
  XAPIAN_QUERY_PARSER_FEATURE_PHRASE = 1 << 1,
  XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE = 1 << 2,
  XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN_ANY_CASE = 1 << 3,
  XAPIAN_QUERY_PARSER_FEATURE_WILDCARD = 1 << 4,
  XAPIAN_QUERY_PARSER_FEATURE_PURE_NOT = 1 << 5,
  XAPIAN_QUERY_PARSER_FEATURE_PARTIAL = 1 << 6,
  XAPIAN_QUERY_PARSER_FEATURE_SPELLING_CORRECTION = 1 << 7,
  XAPIAN_QUERY_PARSER_FEATURE_SYNONYM = 1 << 8,
  XAPIAN_QUERY_PARSER_FEATURE_AUTO_SYNONYMS = 1 << 9,
  XAPIAN_QUERY_PARSER_FEATURE_AUTO_MULTIWORD_SYNONYMS = 1 << 10 | XAPIAN_QUERY_PARSER_FEATURE_AUTO_SYNONYMS,
  XAPIAN_QUERY_PARSER_FEATURE_DEFAULT = XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN |
                                        XAPIAN_QUERY_PARSER_FEATURE_PHRASE |
                                        XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE
} XapianQueryParserFeature;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_query_parser_feature_get_type (void);

#define XAPIAN_TYPE_STEM_STRATEGY                (xapian_stem_strategy_get_type ())

/**
 * XapianStemStrategy:
 * @XAPIAN_STEM_STRATEGY_STEM_NONE: ...
 * @XAPIAN_STEM_STRATEGY_STEM_SOME: ...
 * @XAPIAN_STEM_STRATEGY_STEM_ALL: ...
 * @XAPIAN_STEM_STRATEGY_STEM_ALL_Z: ...
 *
 * Stemming strategies.
 */
typedef enum {
  XAPIAN_STEM_STRATEGY_STEM_NONE,
  XAPIAN_STEM_STRATEGY_STEM_SOME,
  XAPIAN_STEM_STRATEGY_STEM_ALL,
  XAPIAN_STEM_STRATEGY_STEM_ALL_Z
} XapianStemStrategy;

XAPIAN_GLIB_AVAILABLE_IN_ALL
GType xapian_stem_strategy_get_type (void);

G_END_DECLS

#endif /* __XAPIAN_ENUMS_H__ */
