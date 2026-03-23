/* Copyright 2014  Endless Mobile
 * Copyright 2017, 2025  Olly Betts
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
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
 * @XAPIAN_DATABASE_ACTION_CREATE: Create a database, failing if it already
 *   exists
 * @XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE: Create a database and overwrite
 *   it if one already exists
 * @XAPIAN_DATABASE_ACTION_OPEN: Open a database, failing if it doesn't already
 *   exist
 *
 * Actions for #XapianWritableDatabase.
 *
 * Since: 2.0
 */
typedef enum {
  XAPIAN_DATABASE_ACTION_CREATE_OR_OPEN,
  XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE,
  XAPIAN_DATABASE_ACTION_CREATE,
  XAPIAN_DATABASE_ACTION_OPEN
} XapianDatabaseAction;

XAPIAN_GLIB_AVAILABLE_IN_2_0
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
 * @XAPIAN_ERROR_DATABASE_CLOSED: close() was called on the Database
 * @XAPIAN_ERROR_DATABASE_CORRUPT: Database is corrupt
 * @XAPIAN_ERROR_DATABASE_CREATE: Failed to create a database
 * @XAPIAN_ERROR_DATABASE: Database error
 * @XAPIAN_ERROR_DATABASE_LOCK: Failed to acquire the lock on a database
 * @XAPIAN_ERROR_DATABASE_MODIFIED: Database modified after opening
 * @XAPIAN_ERROR_DATABASE_NOT_FOUND: Database not found during opening
 * @XAPIAN_ERROR_DATABASE_OPENING: Unable to open the database
 * @XAPIAN_ERROR_DATABASE_VERSION: Version mismatch when opening the database
 * @XAPIAN_ERROR_DOC_NOT_FOUND: Document not found
 * @XAPIAN_ERROR_FEATURE_UNAVAILABLE: Feature not available with the current backend
 * @XAPIAN_ERROR_INTERNAL: Internal state error
 * @XAPIAN_ERROR_INVALID_ARGUMENT: Invalid argument
 * @XAPIAN_ERROR_INVALID_OPERATION: Invalid operation
 * @XAPIAN_ERROR_NETWORK: Network error
 * @XAPIAN_ERROR_NETWORK_TIMEOUT: Network timeout error
 * @XAPIAN_ERROR_QUERY_PARSER: Error when parsing a query string
 * @XAPIAN_ERROR_RANGE: Out of bounds access
 * @XAPIAN_ERROR_SERIALISATION: Error when serialising or deserialising data
 * @XAPIAN_ERROR_UNIMPLEMENTED: Operation not implemented
 * @XAPIAN_ERROR_WILDCARD: Error expanding a wildcarded query
 *
 * Error codes for the %XAPIAN_ERROR error domain.
 *
 * Since: 2.0
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
  XAPIAN_ERROR_WILDCARD,
  XAPIAN_ERROR_DATABASE_NOT_FOUND,
  XAPIAN_ERROR_DATABASE_CLOSED,

  /*< private >*/
  XAPIAN_ERROR_LAST
} XapianError;

/* Allow #ifdef checks for more recently added enum values. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_ERROR_WILDCARD XAPIAN_ERROR_WILDCARD
#define XAPIAN_ERROR_DATABASE_NOT_FOUND XAPIAN_ERROR_DATABASE_NOT_FOUND
#define XAPIAN_ERROR_DATABASE_CLOSED XAPIAN_ERROR_DATABASE_CLOSED
#endif

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_error_get_type (void);
XAPIAN_GLIB_AVAILABLE_IN_2_0
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
 * @XAPIAN_QUERY_OP_MAX: pick the maximum weight of any subquery
 * @XAPIAN_QUERY_OP_WILDCARD: wildcard expansion
 * @XAPIAN_QUERY_OP_EDIT_DISTANCE: edit distance expansion
 * @XAPIAN_QUERY_OP_INVALID: construct an invalid query (this can be useful as
 *   a placeholder)
 *
 * Operators for #XapianQuery.
 *
 * Since: 2.0
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
  XAPIAN_QUERY_OP_EDIT_DISTANCE,
  XAPIAN_QUERY_OP_INVALID,

  /*< private >*/
  XAPIAN_QUERY_OP_LAST
} XapianQueryOp;

/* Allow #ifdef checks for more recently added enum values. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_QUERY_OP_MAX XAPIAN_QUERY_OP_MAX
#define XAPIAN_QUERY_OP_WILDCARD XAPIAN_QUERY_OP_WILDCARD
#define XAPIAN_QUERY_OP_EDIT_DISTANCE XAPIAN_QUERY_OP_EDIT_DISTANCE
#define XAPIAN_QUERY_OP_INVALID XAPIAN_QUERY_OP_INVALID
#endif

XAPIAN_GLIB_AVAILABLE_IN_2_0
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
 * @XAPIAN_QUERY_PARSER_FEATURE_NGRAMS: generate n-gram terms for text in
 *   scripts written without explicit word breaks
 * @XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM: older name for
 *  XAPIAN_QUERY_PARSER_FEATURE_NGRAMS
 * @XAPIAN_QUERY_PARSER_FEATURE_WORD_BREAKS: try to determine word breaks
 *   for text in scripts written without explicit word breaks (requires
 *   Xapian to be built to use ICU)
 * @XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI: support extended wildcard
 *   `*` which matches zero or more characters, and may be used anwhere in
 *   a word
 * @XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE: support extended wildcard
 *   `?` which matches exactly one character, and may be used anwhere in
 *   a word
 * @XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_GLOB: Enables both
 *   XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI and
 *   XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE
 * @XAPIAN_QUERY_PARSER_FEATURE_FUZZY: Support fuzzy matching.  E.g.
 *   `unserten~3` would expand to `uncertain` (and likely other terms).
 *   `foo~` uses edit distance of 2.  `since~0.2` uses edit distance of
 *   length("since") * 0.2 = 5 * 0.2 = 1
 * @XAPIAN_QUERY_PARSER_FEATURE_ACCUMULATE: Accumulate unstem and stoplist
 *   results.  By default this data is reset for each query parsed.
 * @XAPIAN_QUERY_PARSER_FEATURE_NO_POSITIONS: Produce a query which doesn't
 *   use positional information.  Phrase searches, `NEAR` and `ADJ` will
 *   result in `XAPIAN_QUERY_OP_AND`.
 * @XAPIAN_QUERY_PARSER_FEATURE_NO_PROPER_NOUN_HEURISTIC: Turn off special
 *   handling of capitalised words.
 * @XAPIAN_QUERY_PARSER_FEATURE_DEFAULT: default flags
 *
 * Flags for xapian_query_parser_parse_query_full().
 *
 * Since: 2.0
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
  XAPIAN_QUERY_PARSER_FEATURE_NGRAMS = 1 << 11,
  XAPIAN_QUERY_PARSER_FEATURE_WORD_BREAKS = 1 << 12,
  XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI = 1 << 13,
  XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE = 1 << 14,
  XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_GLOB = XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI |
                                              XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE,
  XAPIAN_QUERY_PARSER_FEATURE_FUZZY = 1 << 15,
  XAPIAN_QUERY_PARSER_FEATURE_ACCUMULATE = 1 << 16,
  XAPIAN_QUERY_PARSER_FEATURE_NO_POSITIONS = 1 << 17,
  XAPIAN_QUERY_PARSER_FEATURE_NO_PROPER_NOUN_HEURISTIC = 1 << 18,
  XAPIAN_QUERY_PARSER_FEATURE_CJK_NGRAM = XAPIAN_QUERY_PARSER_FEATURE_NGRAMS,
  XAPIAN_QUERY_PARSER_FEATURE_DEFAULT = XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN |
                                        XAPIAN_QUERY_PARSER_FEATURE_PHRASE |
                                        XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE
} XapianQueryParserFeature;

/* Allow #ifdef checks for more recently added enum values. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_QUERY_PARSER_FEATURE_NGRAMS XAPIAN_QUERY_PARSER_FEATURE_NGRAMS
#define XAPIAN_QUERY_PARSER_FEATURE_WORD_BREAKS XAPIAN_QUERY_PARSER_FEATURE_WORD_BREAKS
#define XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_MULTI
#define XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_SINGLE
#define XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_GLOB XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_GLOB
#define XAPIAN_QUERY_PARSER_FEATURE_FUZZY XAPIAN_QUERY_PARSER_FEATURE_FUZZY
#define XAPIAN_QUERY_PARSER_FEATURE_ACCUMULATE XAPIAN_QUERY_PARSER_FEATURE_ACCUMULATE
#define XAPIAN_QUERY_PARSER_FEATURE_NO_POSITIONS XAPIAN_QUERY_PARSER_FEATURE_NO_POSITIONS
#define XAPIAN_QUERY_PARSER_FEATURE_NO_PROPER_NOUN_HEURISTIC XAPIAN_QUERY_PARSER_FEATURE_NO_PROPER_NOUN_HEURISTIC
#endif

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_query_parser_feature_get_type (void);

#define XAPIAN_TYPE_STEM_STRATEGY                (xapian_stem_strategy_get_type ())

/**
 * XapianStemStrategy:
 * @XAPIAN_STEM_STRATEGY_STEM_NONE: do not perform any stemming
 * @XAPIAN_STEM_STRATEGY_STEM_SOME: generate both stemmed and unstemmed terms
 * @XAPIAN_STEM_STRATEGY_STEM_ALL: generate only stemmed terms, without the Z prefix
 * @XAPIAN_STEM_STRATEGY_STEM_ALL_Z: generate only stemmed terms, with the Z prefix
 * @XAPIAN_STEM_STRATEGY_STEM_SOME_FULL_POS: like
 *   XAPIAN_STEM_STRATEGY_STEM_SOME but also stores positions for stemmed terms
 *
 * Stemming strategies.
 *
 * Since: 2.0
 */
typedef enum {
  XAPIAN_STEM_STRATEGY_STEM_NONE,
  XAPIAN_STEM_STRATEGY_STEM_SOME,
  XAPIAN_STEM_STRATEGY_STEM_ALL,
  XAPIAN_STEM_STRATEGY_STEM_ALL_Z,
  XAPIAN_STEM_STRATEGY_STEM_SOME_FULL_POS
} XapianStemStrategy;

/* Allow #ifdef checks for more recently added enum values. */
#ifndef __GTK_DOC_IGNORE__
#define XAPIAN_STEM_STRATEGY_STEM_SOME_FULL_POS XAPIAN_STEM_STRATEGY_STEM_SOME_FULL_POS
#endif

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_stem_strategy_get_type (void);

#define XAPIAN_TYPE_TERM_GENERATOR_FEATURE         (xapian_term_generator_feature_get_type ())

/**
 * XapianTermGeneratorFeature:
 * @XAPIAN_TERM_GENERATOR_FEATURE_NONE: No features
 * @XAPIAN_TERM_GENERATOR_FEATURE_SPELLING: Index data required for spelling correction
 * @XAPIAN_TERM_GENERATOR_FEATURE_NGRAMS: generate n-gram terms for text in
 *   scripts written without explicit word breaks
 * @XAPIAN_TERM_GENERATOR_FEATURE_WORD_BREAKS: try to determine word breaks
 *   for text in scripts written without explicit word breaks (requires
 *   Xapian to be built to use ICU)
 *
 * TermGenerator flags
 *
 * Since: 2.0
 */
typedef enum {
  XAPIAN_TERM_GENERATOR_FEATURE_NONE,
  XAPIAN_TERM_GENERATOR_FEATURE_SPELLING = 1,
  XAPIAN_TERM_GENERATOR_FEATURE_NGRAMS = 2,
  XAPIAN_TERM_GENERATOR_FEATURE_WORD_BREAKS = 3
} XapianTermGeneratorFeature;

XAPIAN_GLIB_AVAILABLE_IN_2_0
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
 * Since: 2.0
 */
typedef enum {
  XAPIAN_DATABASE_COMPACT_FLAGS_NO_RENUMBER   = 1 << 2,
  XAPIAN_DATABASE_COMPACT_FLAGS_MULTIPASS     = 1 << 3,
  XAPIAN_DATABASE_COMPACT_FLAGS_SINGLE_FILE   = 1 << 4
} XapianDatabaseCompactFlags;

XAPIAN_GLIB_AVAILABLE_IN_2_0
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
 * Since: 2.0
 */
typedef enum {
  XAPIAN_DATABASE_FLAGS_NONE                    = 0,
  XAPIAN_DATABASE_FLAGS_NO_SYNC                 = 1 << 0,
  XAPIAN_DATABASE_FLAGS_FULL_SYNC               = 1 << 1,
  XAPIAN_DATABASE_FLAGS_DANGEROUS               = 1 << 2,
  XAPIAN_DATABASE_FLAGS_NO_TERMLIST             = 1 << 3,
  XAPIAN_DATABASE_FLAGS_RETRY_LOCK              = 1 << 4
} XapianDatabaseFlags;

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_database_flags_get_type (void);

#define XAPIAN_TYPE_DATABASE_BACKEND            (xapian_database_backend_get_type ())

/**
 * XapianDatabaseBackend:
 * @XAPIAN_DATABASE_BACKEND_DEFAULT: Use the default backend
 * @XAPIAN_DATABASE_BACKEND_GLASS: Use the Glass backend
 * @XAPIAN_DATABASE_BACKEND_CHERT: Use the Chert backend
 * @XAPIAN_DATABASE_BACKEND_STUB: Open a stub database
 * @XAPIAN_DATABASE_BACKEND_INMEMORY: Use the "in memory" backend
 * @XAPIAN_DATABASE_BACKEND_HONEY: Use the Honey backend
 *
 * The Xapian database backend to use when opening a database.
 *
 * Since: 2.0
 */
typedef enum {
  XAPIAN_DATABASE_BACKEND_DEFAULT,
  XAPIAN_DATABASE_BACKEND_GLASS,
  XAPIAN_DATABASE_BACKEND_CHERT,
  XAPIAN_DATABASE_BACKEND_STUB,
  XAPIAN_DATABASE_BACKEND_INMEMORY,
  XAPIAN_DATABASE_BACKEND_HONEY
} XapianDatabaseBackend;

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_database_backend_get_type (void);

typedef enum {
  XAPIAN_DATABASE_COMPACT_LEVEL_STANDARD,
  XAPIAN_DATABASE_COMPACT_LEVEL_FULL,
  XAPIAN_DATABASE_COMPACT_LEVEL_FULLER
} XapianDatabaseCompactLevel;

XAPIAN_GLIB_AVAILABLE_IN_2_0
GType xapian_database_compact_level_get_type (void);

G_END_DECLS

#endif /* __XAPIAN_ENUMS_H__ */
