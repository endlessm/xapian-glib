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
  XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN,
  XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE,
  XAPIAN_DATABASE_ACTION_CREATE,
  XAPIAN_DATABASE_ACTION_OPEN
} XapianDatabaseAction;

XAPIAN_GLIB_AVAILABLE_IN_1_0
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

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_error_get_type (void);
XAPIAN_GLIB_AVAILABLE_IN_1_0
GQuark xapian_error_quark (void);

#define XAPIAN_TYPE_QUERY_OP            (xapian_query_op_get_type ())

/**
 * XapianQueryOp:
 * @XAPIAN_QUERY_OP_AND: filters if both sub-queries are satisfied
 * @XAPIAN_QUERY_OP_OR: filters if either sub-queries are satisfied
 * @XAPIAN_QUERY_OP_AND_NOT: filters if only the left sub-query is
 *   satisfied but not the right
 * @XAPIAN_QUERY_OP_XOR: filters if either sub-query is satisfied
 *   but not both
 * @XAPIAN_QUERY_OP_AND_MAYBE: filters if left sub-query is satisfied
 *   but uses the weights for both
 * @XAPIAN_QUERY_OP_FILTER: filters as %XAPIAN_QUERY_OP_AND, but
 *   uses only weights from the left sub-query
 * @XAPIAN_QUERY_OP_NEAR: filters if occurrances of a list of terms
 *   appear within a specified window of positions
 * @XAPIAN_QUERY_OP_PHRASE: filters if occurrances of a list of terms
 *   appear both within a specified window of positions and als in
 *   the specified order
 * @XAPIAN_QUERY_OP_VALUE_RANGE: filters by a range of values
 * @XAPIAN_QUERY_OP_SCALE_WEIGHT: scales the weight of a sub-query by
 *   the specified factor
 * @XAPIAN_QUERY_OP_ELITE_SET: picks the best N sub-queries and
 *   combines them with %XAPIAN_QUERY_OP_OR
 * @XAPIAN_QUERY_OP_VALUE_GE: filters a document value using a
 *   greater than or equal test
 * @XAPIAN_QUERY_OP_VALUE_LE: filters a document value using a
 *   less than or equal test
 * @XAPIAN_QUERY_OP_SYNONYM: treats a set of sub-queries as synonyms
 * @XAPIAN_QUERY_OP_MAX: pick the maximum weight of any subquery (Since: 1.4.0)
 * @XAPIAN_QUERY_OP_WILDCARD: wildcard expansion (Since: 1.4.0)
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
  XAPIAN_QUERY_OP_MAX,
  XAPIAN_QUERY_OP_WILDCARD,

  /*< private >*/
  XAPIAN_QUERY_OP_LAST
} XapianQueryOp;

/* Allow #ifdef checks for more recently added enum values. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_QUERY_OP_MAX XAPIAN_QUERY_OP_MAX
#define XAPIAN_QUERY_OP_WILDCARD XAPIAN_QUERY_OP_WILDCARD
#endif

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_query_op_get_type (void);

#define XAPIAN_TYPE_QUERY_PARSER_FEATURE        (xapian_query_parser_feature_get_type ())

/**
 * XapianQueryParserFeature:
 * @XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN: support AND, OR, etc as well as
 *   bracketed subexpressions
 * @XAPIAN_QUERY_PARSER_FEATURE_PHRASE: support quoted phrases
 * @XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE: support `+` and `-`
 * @XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN_ANY_CASE: support AND, OR, etc.
 *   even if not in upper case
 * @XAPIAN_QUERY_PARSER_FEATURE_WILDCARD: support right truncation,
 *   e.g. `Xap*`
 * @XAPIAN_QUERY_PARSER_FEATURE_PURE_NOT: allow queries such as `NOT apples`
 * @XAPIAN_QUERY_PARSER_FEATURE_PARTIAL: enable partial matching
 * @XAPIAN_QUERY_PARSER_FEATURE_SPELLING_CORRECTION: enable spelling
 *   correction
 * @XAPIAN_QUERY_PARSER_FEATURE_SYNONYM: enable synonym operator `~`
 * @XAPIAN_QUERY_PARSER_FEATURE_AUTO_SYNONYMS: enable automatic use of
 *   synonyms for single terms
 * @XAPIAN_QUERY_PARSER_FEATURE_AUTO_MULTIWORD_SYNONYMS: enable automatic
 *   use of synonyms for single terms and groups of terms
 * @XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM: enable generation of n-grams from
 *   CJK text (Since: 1.4.0)
 * @XAPIAN_QUERY_PARSER_FEATURE_DEFAULT: default flags
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
  XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM = 1 << 11,
  XAPIAN_QUERY_PARSER_FEATURE_DEFAULT = XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN |
                                        XAPIAN_QUERY_PARSER_FEATURE_PHRASE |
                                        XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE
} XapianQueryParserFeature;

/* Allow #ifdef XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM to test for
 * more recently added enum value. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM
#endif

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_query_parser_feature_get_type (void);

#define XAPIAN_TYPE_STEM_STRATEGY                (xapian_stem_strategy_get_type ())

/**
 * XapianStemStrategy:
 * @XAPIAN_STEM_STRATEGY_STEM_NONE: do not perform any stemming
 * @XAPIAN_STEM_STRATEGY_STEM_SOME: generate both stemmed and unstemmed terms
 * @XAPIAN_STEM_STRATEGY_STEM_ALL: generate only stemmed terms, without the Z prefix
 * @XAPIAN_STEM_STRATEGY_STEM_ALL_Z: generate only stemmed terms, with the Z prefix
 *
 * Stemming strategies.
 */
typedef enum {
  XAPIAN_STEM_STRATEGY_STEM_NONE,
  XAPIAN_STEM_STRATEGY_STEM_SOME,
  XAPIAN_STEM_STRATEGY_STEM_ALL,
  XAPIAN_STEM_STRATEGY_STEM_ALL_Z
} XapianStemStrategy;

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_stem_strategy_get_type (void);

#define XAPIAN_TYPE_TERM_GENERATOR_FEATURE         (xapian_term_generator_feature_get_type ())

/**
 * XapianTermGeneratorFeature:
 * @XAPIAN_TERM_GENERATOR_FEATURE_NONE: No features
 * @XAPIAN_TERM_GENERATOR_FEATURE_SPELLING: Index data required for spelling correction
 *
 * TermGenerator flags
 *
 * Since: 1.2
 */
typedef enum {
  XAPIAN_TERM_GENERATOR_FEATURE_NONE,
  XAPIAN_TERM_GENERATOR_FEATURE_SPELLING = 1
} XapianTermGeneratorFeature;

XAPIAN_GLIB_AVAILABLE_IN_1_2
GType xapian_term_generator_feature_get_type (void);

#define XAPIAN_TYPE_DATABASE_COMPACT            (xapian_database_compact_get_type ())

/**
 * XapianDatabaseCompactFlags:
 * @XAPIAN_DATABASE_COMPACT_FLAGS_NO_RENUMBER: Use the same document ids
 * @XAPIAN_DATABASE_COMPACT_FLAGS_MULTIPASS: Merge postlists in multiple passes
 * @XAPIAN_DATABASE_COMPACT_FLAGS_SINGLE_FILE: Create a single-file database
 *
 * Compacting policies for #XapianDatabase.
 *
 * Since: 1.4
 */
typedef enum {
  XAPIAN_DATABASE_COMPACT_FLAGS_NO_RENUMBER   = 1 << 2,
  XAPIAN_DATABASE_COMPACT_FLAGS_MULTIPASS     = 1 << 3,
  XAPIAN_DATABASE_COMPACT_FLAGS_SINGLE_FILE   = 1 << 4
} XapianDatabaseCompactFlags;

XAPIAN_GLIB_AVAILABLE_IN_1_4
GType xapian_database_compact_flags_get_type (void);

#define XAPIAN_TYPE_DATABASE_FLAGS             (xapian_database_flags_get_type ())

/**
 * XapianDatabaseFlags:
 * @XAPIAN_DATABASE_FLAGS_NONE: No flags
 * @XAPIAN_DATABASE_FLAGS_NO_SYNC: Do not sync changes to disk
 * @XAPIAN_DATABASE_FLAGS_FULL_SYNC: Attempt to fully sync changes to disk
 * @XAPIAN_DATABASE_FLAGS_DANGEROUS: Update the database in-place
 * @XAPIAN_DATABASE_FLAGS_NO_TERMLIST: Do not create a termlist table
 * @XAPIAN_DATABASE_FLAGS_RETRY_LOCK: Retry the lock
 *
 * Flags used when opening a database
 *
 * Since: 1.4
 */
typedef enum {
  XAPIAN_DATABASE_FLAGS_NONE                    = 0,
  XAPIAN_DATABASE_FLAGS_NO_SYNC                 = 1 << 0,
  XAPIAN_DATABASE_FLAGS_FULL_SYNC               = 1 << 1,
  XAPIAN_DATABASE_FLAGS_DANGEROUS               = 1 << 2,
  XAPIAN_DATABASE_FLAGS_NO_TERMLIST             = 1 << 3,
  XAPIAN_DATABASE_FLAGS_RETRY_LOCK              = 1 << 4
} XapianDatabaseFlags;

XAPIAN_GLIB_AVAILABLE_IN_1_4
GType xapian_database_flags_get_type (void);

#define XAPIAN_TYPE_DATABASE_BACKEND            (xapian_database_backend_get_type ())

/**
 * XapianDatabaseBackend:
 * @XAPIAN_DATABASE_BACKEND_DEFAULT: Use the default backend
 * @XAPIAN_DATABASE_BACKEND_GLASS: Use the Glass backend
 * @XAPIAN_DATABASE_BACKEND_CHERT: Use the Chert backend
 * @XAPIAN_DATABASE_BACKEND_STUB: Open a stub database
 * @XAPIAN_DATABASE_BACKEND_INMEMORY: Use the "in memory" backend
 *
 * The Xapian database backend to use when opening a database.
 *
 * Since: 1.4
 */
typedef enum {
  XAPIAN_DATABASE_BACKEND_DEFAULT,
  XAPIAN_DATABASE_BACKEND_GLASS,
  XAPIAN_DATABASE_BACKEND_CHERT,
  XAPIAN_DATABASE_BACKEND_STUB,
  XAPIAN_DATABASE_BACKEND_INMEMORY
} XapianDatabaseBackend;

XAPIAN_GLIB_AVAILABLE_IN_1_4
GType xapian_database_backend_get_type (void);

G_END_DECLS

#endif /* __XAPIAN_ENUMS_H__ */
