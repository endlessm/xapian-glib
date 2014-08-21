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

#ifndef __XAPIAN_GLIB_TYPES_H__
#define __XAPIAN_GLIB_TYPES_H__

#include <gio/gio.h>
#include "xapian-glib-macros.h"
#include "xapian-enums.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_DATABASE                    (xapian_database_get_type ())
#define XAPIAN_TYPE_WRITABLE_DATABASE           (xapian_writable_database_get_type ())
#define XAPIAN_TYPE_DOCUMENT                    (xapian_document_get_type ())
#define XAPIAN_TYPE_ENQUIRE                     (xapian_enquire_get_type ())
#define XAPIAN_TYPE_POSTING_SOURCE              (xapian_posting_source_get_type ())
#define XAPIAN_TYPE_VALUE_POSTING_SOURCE        (xapian_value_posting_source_get_type ())
#define XAPIAN_TYPE_VALUE_WEIGHT_POSTING_SOURCE (xapian_value_weight_posting_source_get_type ())
#define XAPIAN_TYPE_QUERY                       (xapian_query_get_type ())
#define XAPIAN_TYPE_STEM                        (xapian_stem_get_type ())
#define XAPIAN_TYPE_QUERY_PARSER                (xapian_query_parser_get_type ())
#define XAPIAN_TYPE_MSET                        (xapian_mset_get_type ())
#define XAPIAN_TYPE_MSET_ITERATOR               (xapian_mset_iterator_get_type ())
#define XAPIAN_TYPE_TERM_GENERATOR              (xapian_term_generator_get_type ())

typedef struct _XapianDatabase                  XapianDatabase;
typedef struct _XapianWritableDatabase          XapianWritableDatabase;

typedef struct _XapianDocument                  XapianDocument;

typedef struct _XapianEnquire                   XapianEnquire;
typedef struct _XapianPostingSource             XapianPostingSource;
typedef struct _XapianValuePostingSource        XapianValuePostingSource;
typedef struct _XapianValueWeightPostingSource  XapianValueWeightPostingSource;
typedef struct _XapianQuery                     XapianQuery;
typedef struct _XapianStem                      XapianStem;
typedef struct _XapianQueryParser               XapianQueryParser;
typedef struct _XapianMSet                      XapianMSet;
typedef struct _XapianMSetIterator              XapianMSetIterator;
typedef struct _XapianTermGenerator             XapianTermGenerator;

G_END_DECLS

#endif /* __XAPIAN_GLIB_TYPES_H__ */
