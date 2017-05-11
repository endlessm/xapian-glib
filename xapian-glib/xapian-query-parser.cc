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

/**
 * SECTION:xapian-query-parser
 * @Title: XapianQueryParser
 * @short_description: Parse query strings
 *
 * #XapianQueryParser is a class that can parse a query string and
 * generate a #XapianQuery instance from it.
 *
 * #XapianQueryParser can use a #XapianStem to isolate the terms
 * for the query, as well as use a database for wildcard expansion.
 */

#include "config.h"

#include <xapian.h>

#include "xapian-query-parser.h"

#include "xapian-database-private.h"
#include "xapian-stopper-private.h"
#include "xapian-enums.h"
#include "xapian-error-private.h"
#include "xapian-query-private.h"
#include "xapian-stem-private.h"

#define XAPIAN_QUERY_PARSER_GET_PRIVATE(obj) \
  ((XapianQueryParserPrivate *) xapian_query_parser_get_instance_private ((XapianQueryParser *) (obj)))

typedef struct {
  Xapian::QueryParser *mQueryParser;

  XapianStem *stemmer;
  XapianStemStrategy stemming_strategy;

  XapianDatabase *database;

  XapianStopper *stopper;

  XapianQueryOp default_op;
} XapianQueryParserPrivate;

enum {
  PROP_0,

  PROP_STEMMER,
  PROP_STEMMING_STRATEGY,
  PROP_DATABASE,
  PROP_STOPPER,
  PROP_DEFAULT_OP,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE (XapianQueryParser, xapian_query_parser, G_TYPE_OBJECT)

static void
xapian_query_parser_finalize (GObject *gobject)
{
  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (gobject);

  delete priv->mQueryParser;

  G_OBJECT_CLASS (xapian_query_parser_parent_class)->finalize (gobject);
}

static void
xapian_query_parser_dispose (GObject *gobject)
{
  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (gobject);

  g_clear_object (&priv->stemmer);
  g_clear_object (&priv->database);
  g_clear_object (&priv->stopper);

  G_OBJECT_CLASS (xapian_query_parser_parent_class)->dispose (gobject);
}

static void
xapian_query_parser_set_property (GObject      *gobject,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  XapianQueryParser *self = XAPIAN_QUERY_PARSER (gobject);

  switch (prop_id)
    {
    case PROP_STEMMER:
      xapian_query_parser_set_stemmer (self, (XapianStem *) g_value_get_object (value));
      break;

    case PROP_STEMMING_STRATEGY:
      xapian_query_parser_set_stemming_strategy (self, (XapianStemStrategy) g_value_get_enum (value));
      break;

    case PROP_DATABASE:
      xapian_query_parser_set_database (self, (XapianDatabase *) g_value_get_object (value));
      break;

    case PROP_STOPPER:
      xapian_query_parser_set_stopper (self, (XapianStopper *) g_value_get_object (value));
      break;

    case PROP_DEFAULT_OP:
      xapian_query_parser_set_default_op (self, (XapianQueryOp) g_value_get_enum (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_query_parser_get_property (GObject    *gobject,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_STEMMER:
      g_value_set_object (value, priv->stemmer);
      break;

    case PROP_STEMMING_STRATEGY:
      g_value_set_enum (value, priv->stemming_strategy);
      break;

    case PROP_DATABASE:
      g_value_set_object (value, priv->database);
      break;

    case PROP_STOPPER:
      g_value_set_object (value, priv->stopper);
      break;

    case PROP_DEFAULT_OP:
      g_value_set_enum (value, priv->default_op);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_query_parser_class_init (XapianQueryParserClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  /**
   * XapianQueryParser:stemmer:
   *
   * The #XapianStem instance to be used for stemming the query string.
   */
  obj_props[PROP_STEMMER] =
    g_param_spec_object ("stemmer",
                         "Stemmer",
                         "The XapianStem object to use for stemming",
                         XAPIAN_TYPE_STEM,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianQueryParser:stemming-strategy:
   *
   * The stemming strategy to use with the #XapianStem set with the
   * #XapianQueryParser:stemmer property.
   */
  obj_props[PROP_STEMMING_STRATEGY] =
    g_param_spec_enum ("stemming-strategy",
                       "Stemming Strategy",
                       "The stemming strategy to use with the stemmer",
                       XAPIAN_TYPE_STEM_STRATEGY,
                       XAPIAN_STEM_STRATEGY_STEM_NONE,
                       (GParamFlags) (G_PARAM_READWRITE |
                                      G_PARAM_STATIC_STRINGS));

  /**
   * XapianQueryParser:database:
   *
   * The #XapianDatabase to be used for wildcard expansion.
   */
  obj_props[PROP_DATABASE] =
    g_param_spec_object ("database",
                         "Database",
                         "The database used for wildcard expansion",
                         XAPIAN_TYPE_DATABASE,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianQueryParser:stopper:
   *
   * The #XapianStopper to be used for dropping stop words
   * Since: 1.2
   */
  obj_props[PROP_STOPPER] =
    g_param_spec_object ("stopper",
                         "Stopper",
                         "The stopper to be used for dropping stop words",
                         XAPIAN_TYPE_STOPPER,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianQueryParser:default_op:
   *
   * The #XapianQueryOp used to combine non-filter query items when no
   * explicit operator is used.
   * Since: 1.4
   */
  obj_props[PROP_DEFAULT_OP] =
    g_param_spec_enum ("default-op",
                       "Default Op",
                       "The query operator used to combine non-filter query items when no explicit operator is used",
                       XAPIAN_TYPE_QUERY_OP,
                       XAPIAN_QUERY_OP_OR,
                       (GParamFlags) (G_PARAM_READWRITE |
                                      G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_query_parser_set_property;
  gobject_class->get_property = xapian_query_parser_get_property;
  gobject_class->dispose = xapian_query_parser_dispose;
  gobject_class->finalize = xapian_query_parser_finalize;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_query_parser_init (XapianQueryParser *self)
{
  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (self);

  priv->mQueryParser = new Xapian::QueryParser ();
}

/**
 * xapian_query_parser_new:
 *
 * Creates a new #XapianQueryParser.
 *
 * Returns: (transfer full): the newly created #XapianQueryParser instance
 */
XapianQueryParser *
xapian_query_parser_new (void)
{
  return static_cast<XapianQueryParser *> (g_object_new (XAPIAN_TYPE_QUERY_PARSER, NULL));
}

/**
 * xapian_query_parser_set_stemmer:
 * @parser: a #XapianQueryParser
 * @stemmer: a #XapianStem
 *
 * Sets the stemmer for @parser.
 */
void
xapian_query_parser_set_stemmer (XapianQueryParser *parser,
                                 XapianStem        *stemmer)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));
  g_return_if_fail (XAPIAN_IS_STEM (stemmer));

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  if (priv->stemmer == stemmer)
    return;

  g_clear_object (&priv->stemmer);
  priv->stemmer = static_cast<XapianStem *> (g_object_ref (stemmer));

  priv->mQueryParser->set_stemmer (*xapian_stem_get_internal (stemmer));

  g_object_notify_by_pspec (G_OBJECT (parser), obj_props[PROP_STEMMER]);
}

/**
 * xapian_query_parser_set_stemming_strategy:
 * @parser: a #XapianQueryParser
 * @strategy: a #XapianStemStrategy value
 *
 * Sets the stemming @strategy for @parser.
 *
 * The stemming strategy is only used if @parser uses a stemmer.
 */
void
xapian_query_parser_set_stemming_strategy (XapianQueryParser  *parser,
                                           XapianStemStrategy  strategy)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  if (priv->stemming_strategy == strategy)
    return;

  priv->stemming_strategy = strategy;

  Xapian::QueryParser::stem_strategy stem_strategy;

  switch (priv->stemming_strategy)
    {
    case XAPIAN_STEM_STRATEGY_STEM_NONE:
      stem_strategy = Xapian::QueryParser::STEM_NONE;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_SOME:
      stem_strategy = Xapian::QueryParser::STEM_SOME;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_ALL:
      stem_strategy = Xapian::QueryParser::STEM_ALL;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_ALL_Z:
      stem_strategy = Xapian::QueryParser::STEM_ALL_Z;
      break;

    default:
      g_assert_not_reached ();
    }

  priv->mQueryParser->set_stemming_strategy (stem_strategy);

  g_object_notify_by_pspec (G_OBJECT (parser), obj_props[PROP_STEMMING_STRATEGY]);
}

/**
 * xapian_query_parser_set_database:
 * @parser: a #XapianQueryParser
 * @database: a #XapianDatabase
 *
 * Sets the @database used by @parser for wildcard expansion.
 */
void
xapian_query_parser_set_database (XapianQueryParser *parser,
                                  XapianDatabase    *database)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));
  g_return_if_fail (XAPIAN_IS_DATABASE (database));

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  if (priv->database == database)
    return;

  g_clear_object (&priv->database);
  priv->database = static_cast<XapianDatabase *> (g_object_ref (database));

  priv->mQueryParser->set_database (*xapian_database_get_internal (database));

  g_object_notify_by_pspec (G_OBJECT (parser), obj_props[PROP_DATABASE]);
}

/**
 * xapian_query_parser_set_stopper:
 * @parser: a #XapianQueryParser
 * @stopper: a #XapianStopper
 *
 * Sets the @stopper used by @parser stop word elimination.
 * Since: 1.2
 */
void
xapian_query_parser_set_stopper (XapianQueryParser *parser,
                                 XapianStopper    *stopper)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));
  g_return_if_fail (XAPIAN_IS_STOPPER (stopper));

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  if (priv->stopper == stopper)
    return;

  g_clear_object (&priv->stopper);
  priv->stopper = static_cast<XapianStopper *> (g_object_ref (stopper));

  priv->mQueryParser->set_stopper (xapian_stopper_get_internal (stopper));

  g_object_notify_by_pspec (G_OBJECT (parser), obj_props[PROP_STOPPER]);
}

/**
 * xapian_query_parser_set_default_op:
 * @parser: a #XapianQueryParser
 * @op: a #XapianQueryOp value
 *
 * Set the operator to use to combine non-filter query items when no explicit
 * operator is used.
 * Since: 1.4
 */
void
xapian_query_parser_set_default_op (XapianQueryParser *parser,
                                    XapianQueryOp      op)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  if (priv->default_op == op)
    return;

  priv->default_op = op;

  Xapian::Query::op query_op = xapian_query_op_internal (op);

  priv->mQueryParser->set_default_op (query_op);

  g_object_notify_by_pspec (G_OBJECT (parser), obj_props[PROP_DEFAULT_OP]);
}

/**
 * xapian_query_parser_add_prefix:
 * @parser: a #XapianQueryParser
 * @field: the user visible field name
 * @prefix: the term prefix to map the @field to
 *
 * Adds a probabilistic term prefix.
 *
 * For instance, calling:
 *
 * |[<!-- language="C" -->
 *   xapian_query_parser_add_prefix (parser, "author", "A");
 * ]|
 *
 * allows the user to search for `author:Orwell`, which will be
 * converted to a search for `Aorwell`.
 *
 * It is possible to map multiple fields to the same prefix, for
 * instance `title` and `subject`.
 *
 * It is possible to map the same field to multiple prefixes; the
 * generated query will perform an OR operation on each term.
 */
void
xapian_query_parser_add_prefix (XapianQueryParser *parser,
                                const char        *field,
                                const char        *prefix)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));
  g_return_if_fail (field != NULL && prefix != NULL);

  try
    {
      XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);
      priv->mQueryParser->add_prefix (std::string (field), std::string (prefix));
    }
  catch (const Xapian::InvalidOperationError &err)
    {
      std::string desc = err.get_msg ();

      g_critical ("add_prefix: %s", desc.c_str ());
    }
}

/**
 * xapian_query_parser_add_boolean_prefix:
 * @parser: a #XapianQueryParser
 * @field: the user visible field name
 * @prefix: the term prefix to map the @field to
 * @exclusive: if %TRUE each document can have at most one term
 *   with the given @prefix
 *
 * Adds a boolean term prefix to the query.
 */
void
xapian_query_parser_add_boolean_prefix (XapianQueryParser *parser,
                                        const char        *field,
                                        const char        *prefix,
                                        gboolean           exclusive)
{
  g_return_if_fail (XAPIAN_IS_QUERY_PARSER (parser));
  g_return_if_fail (field != NULL && prefix != NULL);

  try
    {
      XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

      priv->mQueryParser->add_boolean_prefix (std::string (field),
                                              std::string (prefix),
                                              exclusive);
    }
  catch (const Xapian::InvalidOperationError &err)
    {
      std::string desc = err.get_msg ();

      g_critical ("add_boolean_prefix: %s", desc.c_str ());
    }
}

/**
 * xapian_query_parser_parse_query:
 * @parser: a #XapianQueryParser
 * @query_string: the query string
 * @error: return location for a #GError
 *
 * Parses a query string and generates a #XapianQuery instance for it.
 *
 * See also: xapian_query_parser_parse_query_full()
 *
 * Returns: (transfer full): the newly created #XapianQuery instance
 */
XapianQuery *
xapian_query_parser_parse_query (XapianQueryParser  *parser,
                                 const char         *query_string,
                                 GError            **error)
{
  return xapian_query_parser_parse_query_full (parser, query_string,
                                               XAPIAN_QUERY_PARSER_FEATURE_DEFAULT,
                                               "",
                                               error);
}

/**
 * xapian_query_parser_parse_query_full:
 * @parser: a #XapianQuery
 * @query_string: the query string
 * @flags: a bitwise OR of #XapianQueryParserFeature values
 * @default_prefix: the default prefix for terms
 * @error: return location for a #GError
 *
 * Parses @query_string and creates a #XapianQuery instance for it.
 *
 * Returns: (transfer full): the newly created #XapianQuery instance
 */
XapianQuery *
xapian_query_parser_parse_query_full (XapianQueryParser        *parser,
                                      const char               *query_string,
                                      XapianQueryParserFeature  flags,
                                      const char               *default_prefix,
                                      GError                  **error)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY_PARSER (parser), NULL);
  g_return_val_if_fail (query_string != NULL, NULL);

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  try
    {
      unsigned int real_flags = 0;

      if (flags & XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN)
        real_flags |= Xapian::QueryParser::FLAG_BOOLEAN;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_PHRASE)
        real_flags |= Xapian::QueryParser::FLAG_PHRASE;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_LOVEHATE)
        real_flags |= Xapian::QueryParser::FLAG_LOVEHATE;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_BOOLEAN_ANY_CASE)
        real_flags |= Xapian::QueryParser::FLAG_BOOLEAN_ANY_CASE;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_WILDCARD)
        real_flags |= Xapian::QueryParser::FLAG_WILDCARD;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_PURE_NOT)
        real_flags |= Xapian::QueryParser::FLAG_PURE_NOT;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_PARTIAL)
        real_flags |= Xapian::QueryParser::FLAG_PARTIAL;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_SPELLING_CORRECTION)
        real_flags |= Xapian::QueryParser::FLAG_SPELLING_CORRECTION;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_SYNONYM)
        real_flags |= Xapian::QueryParser::FLAG_SYNONYM;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_AUTO_SYNONYMS)
        real_flags |= Xapian::QueryParser::FLAG_AUTO_SYNONYMS;
      if (flags & XAPIAN_QUERY_PARSER_FEATURE_AUTO_MULTIWORD_SYNONYMS)
        real_flags |= Xapian::QueryParser::FLAG_AUTO_MULTIWORD_SYNONYMS;

      Xapian::Query query = priv->mQueryParser->parse_query (std::string (query_string),
                                                             real_flags,
                                                             std::string (default_prefix));

      return xapian_query_new_from_query (query);
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      return NULL;
    }
}

/**
 * xapian_query_parser_get_stopper
 * @parser: a #XapianQueryParser
 *
 * Returns: (transfer none): the #XapianStopper used by @parser
 *
 * Since: 1.2
 */
XapianStopper *
xapian_query_parser_get_stopper (XapianQueryParser *parser)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY_PARSER (parser), NULL);

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);

  return priv->stopper;
}

/**
 * xapian_query_parser_get_corrected_query_string
 * @parser: a #XapianQueryParser
 *
 * Get the spelling-corrected query string.
 *
 * This will only be set if %XAPIAN_QUERY_PARSER_FEATURE_SPELLING_CORRECTION
 * is specified when xapian_query_parser_parse_query_full() was last called.
 *
 * If there were no corrections, an empty string is returned.
 *
 * Returns: (transfer full): the corrected query string
 *
 * Since: 1.2
 */
char *
xapian_query_parser_get_corrected_query_string (XapianQueryParser *parser)
{
  g_return_val_if_fail (XAPIAN_IS_QUERY_PARSER (parser), NULL);

  XapianQueryParserPrivate *priv = XAPIAN_QUERY_PARSER_GET_PRIVATE (parser);
  
  std::string corrected = priv->mQueryParser->get_corrected_query_string ();
  return g_strdup (corrected.c_str ());
}
