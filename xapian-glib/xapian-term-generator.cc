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

#include <xapian.h>

#include "xapian-term-generator.h"

#include "xapian-database-private.h"
#include "xapian-document-private.h"
#include "xapian-enums.h"
#include "xapian-error-private.h"
#include "xapian-stem-private.h"
#include "xapian-writable-database.h"

#define XAPIAN_TERM_GENERATOR_GET_PRIVATE(obj) \
  ((XapianTermGeneratorPrivate *) xapian_term_generator_get_instance_private ((XapianTermGenerator *) (obj)))

typedef struct {
  Xapian::TermGenerator mGenerator;

  XapianStem *stemmer;
  XapianWritableDatabase *database;
  XapianDocument *document;

  XapianStemStrategy strategy;
} XapianTermGeneratorPrivate;

enum {
  PROP_0,

  PROP_STEMMER,
  PROP_DATABASE,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE (XapianTermGenerator, xapian_term_generator, G_TYPE_OBJECT)

static void
xapian_term_generator_dispose (GObject *gobject)
{
  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (gobject);

  g_clear_object (&priv->stemmer);
  g_clear_object (&priv->database);
  g_clear_object (&priv->document);

  G_OBJECT_CLASS (xapian_term_generator_parent_class)->dispose (gobject);
}

static void
xapian_term_generator_set_property (GObject *gobject,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
}

static void
xapian_term_generator_get_property (GObject *gobject,
                                    guint prop_id,
                                    GValue *value,
                                    GParamSpec *pspec)
{
}

static void
xapian_term_generator_class_init (XapianTermGeneratorClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  obj_props[PROP_STEMMER] =
    g_param_spec_object ("stemmer",
                         "Stemmer",
                         "The XapianStem instance used to stem data",
                         XAPIAN_TYPE_STEM,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));
  obj_props[PROP_DATABASE] =
    g_param_spec_object ("database",
                         "Database",
                         "The database used store the indexed data",
                         XAPIAN_TYPE_WRITABLE_DATABASE,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_term_generator_set_property;
  gobject_class->get_property = xapian_term_generator_get_property;
  gobject_class->dispose = xapian_term_generator_dispose;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_term_generator_init (XapianTermGenerator *self)
{
}

XapianTermGenerator *
xapian_term_generator_new (void)
{
  return (XapianTermGenerator *) g_object_new (XAPIAN_TYPE_TERM_GENERATOR, NULL);
}
