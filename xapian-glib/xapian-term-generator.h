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

#ifndef __XAPIAN_GLIB_TERM_GENERATOR_H__
#define __XAPIAN_GLIB_TERM_GENERATOR_H__

#if !defined(XAPIAN_GLIB_H_INSIDE) && !defined(XAPIAN_GLIB_COMPILATION)
#error "Only <xapian-glib.h> can be included directly."
#endif

#include "xapian-glib-types.h"
#include "xapian-stem.h"
#include "xapian-stopper.h"
#include "xapian-writable-database.h"

G_BEGIN_DECLS

#define XAPIAN_TYPE_TERM_GENERATOR      (xapian_term_generator_get_type())

XAPIAN_GLIB_AVAILABLE_IN_2_0
G_DECLARE_DERIVABLE_TYPE (XapianTermGenerator, xapian_term_generator, XAPIAN, TERM_GENERATOR, GObject)

struct _XapianTermGeneratorClass
{
  GObjectClass parent_instance;
};

XAPIAN_GLIB_AVAILABLE_IN_2_0
XapianTermGenerator *   xapian_term_generator_new       (void);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_database              (XapianTermGenerator    *generator,
                                                                         XapianWritableDatabase *database);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_flags                 (XapianTermGenerator    *generator,
                                                                         XapianTermGeneratorFeature flags);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_stemmer               (XapianTermGenerator    *generator,
                                                                         XapianStem             *stemmer);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_stemming_strategy     (XapianTermGenerator    *generator,
                                                                         XapianStemStrategy      strategy);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_stopper               (XapianTermGenerator    *generator,
                                                                         XapianStopper          *stopper);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_set_document              (XapianTermGenerator    *generator,
                                                                         XapianDocument         *document);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_index_text                (XapianTermGenerator    *generator,
                                                                         const char             *data);
XAPIAN_GLIB_AVAILABLE_IN_2_0
void                    xapian_term_generator_index_text_full           (XapianTermGenerator    *generator,
                                                                         const char             *data,
                                                                         unsigned int            wdf_inc,
                                                                         const char             *prefix);

G_END_DECLS

#endif /* __XAPIAN_GLIB_TERM_GENERATOR_H__ */
