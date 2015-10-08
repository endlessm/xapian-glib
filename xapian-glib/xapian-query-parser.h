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

#ifndef __XAPIAN_GLIB_QUERY_PARSER_H__
#define __XAPIAN_GLIB_QUERY_PARSER_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"

G_BEGIN_DECLS

#define XAPIAN_QUERY_PARSER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), XAPIAN_TYPE_QUERY_PARSER, XapianQueryParser))
#define XAPIAN_IS_QUERY_PARSER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAPIAN_TYPE_QUERY_PARSER))
#define XAPIAN_QUERY_PARSER_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), XAPIAN_TYPE_QUERY_PARSER, XapianQueryParserClass))
#define XAPIAN_IS_QUERY_PARSER_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), XAPIAN_TYPE_QUERY_PARSER))
#define XAPIAN_QUERY_PARSER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), XAPIAN_TYPE_QUERY_PARSER, XapianQueryParserClass))

typedef struct _XapianQueryParserClass     XapianQueryParserClass;

struct _XapianQueryParser
{
  GObject parent_instance;
};

struct _XapianQueryParserClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_1_0
GType xapian_query_parser_get_type (void);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQueryParser *     xapian_query_parser_new                         (void);

XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_query_parser_set_stemmer                 (XapianQueryParser        *parser,
                                                                         XapianStem               *stemmer);
XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_query_parser_set_stemming_strategy       (XapianQueryParser        *parser,
                                                                         XapianStemStrategy        strategy);
XAPIAN_GLIB_AVAILABLE_IN_1_2
void                    xapian_query_parser_set_stopper                 (XapianQueryParser        *parser,
                                                                         XapianStopper               *stopper);
XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_query_parser_set_database                (XapianQueryParser        *parser,
                                                                         XapianDatabase           *database);

XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_query_parser_add_prefix                  (XapianQueryParser        *parser,
                                                                         const char               *field,
                                                                         const char               *prefix);
XAPIAN_GLIB_AVAILABLE_IN_1_0
void                    xapian_query_parser_add_boolean_prefix          (XapianQueryParser        *parser,
                                                                         const char               *field,
                                                                         const char               *prefix,
                                                                         gboolean                  exclusive);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *           xapian_query_parser_parse_query                 (XapianQueryParser        *parser,
                                                                         const char               *query_string,
                                                                         GError                  **error);
XAPIAN_GLIB_AVAILABLE_IN_1_2
XapianStopper *           xapian_query_parser_get_stopper               (XapianQueryParser        *parser);

XAPIAN_GLIB_AVAILABLE_IN_1_0
XapianQuery *           xapian_query_parser_parse_query_full            (XapianQueryParser        *parser,
                                                                         const char               *query_string,
                                                                         XapianQueryParserFeature  flags,
                                                                         const char               *default_prefix,
                                                                         GError                  **error);

XAPIAN_GLIB_AVAILABLE_IN_1_2
char *                  xapian_query_parser_get_corrected_query_string  (XapianQueryParser        *parser);

G_END_DECLS

#endif /* __XAPIAN_GLIB_QUERY_PARSER_H__ */
