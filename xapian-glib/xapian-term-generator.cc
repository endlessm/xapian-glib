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
 * SECTION:xapian-term-generator
 * @Title: XapianTermGenerator
 * @short_description: Document indexer
 *
 * #XapianTermGenerator is a class used to index document data.
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
  Xapian::TermGenerator *mGenerator;

  XapianWritableDatabase *database;

  XapianStem *stemmer;
  XapianStemStrategy stemming_strategy;

  XapianDocument *document;
} XapianTermGeneratorPrivate;

enum {
  PROP_0,

  PROP_STEMMER,
  PROP_STEMMING_STRATEGY,
  PROP_DATABASE,
  PROP_DOCUMENT,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE (XapianTermGenerator, xapian_term_generator, G_TYPE_OBJECT)

static void
xapian_term_generator_finalize (GObject *gobject)
{
  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (gobject);

  delete priv->mGenerator;

  G_OBJECT_CLASS (xapian_term_generator_parent_class)->finalize (gobject);
}

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
xapian_term_generator_set_property (GObject      *gobject,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  XapianTermGenerator *self = XAPIAN_TERM_GENERATOR (gobject);

  switch (prop_id)
    {
    case PROP_STEMMER:
      xapian_term_generator_set_stemmer (self, (XapianStem *) g_value_get_object (value));
      break;

    case PROP_STEMMING_STRATEGY:
      xapian_term_generator_set_stemming_strategy (self, (XapianStemStrategy) g_value_get_enum (value));
      break;

    case PROP_DATABASE:
      xapian_term_generator_set_database (self, (XapianWritableDatabase *) g_value_get_object (value));
      break;

    case PROP_DOCUMENT:
      xapian_term_generator_set_document (self, (XapianDocument *) g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_term_generator_get_property (GObject    *gobject,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (gobject);

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

    case PROP_DOCUMENT:
      g_value_set_object (value, priv->document);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_term_generator_class_init (XapianTermGeneratorClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  /**
   * XapianTermGenerator:stemmer:
   *
   * The #XapianStem instance used for stemming the indexed data.
   *
   * The value of this property is only used when the
   * #XapianTermGenerator:stemming-strategy property is set
   * to a value different than %XAPIAN_STEM_STRATEGY_STEM_NONE.
   */
  obj_props[PROP_STEMMER] =
    g_param_spec_object ("stemmer",
                         "Stemmer",
                         "The XapianStem instance used to stem data",
                         XAPIAN_TYPE_STEM,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianTermGenerator:stemming-strategy:
   *
   * The stemming strategy to use when the #XapianTermGenerator:stemmer
   * property is set.
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
   * XapianTermGenerator:database:
   *
   * The #XapianWritableDatabase instance used to store the indexed
   * data.
   */
  obj_props[PROP_DATABASE] =
    g_param_spec_object ("database",
                         "Database",
                         "The database used store the indexed data",
                         XAPIAN_TYPE_WRITABLE_DATABASE,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  /**
   * XapianTermGenerator:document:
   *
   * The currently indexed document.
   */
  obj_props[PROP_DOCUMENT] =
    g_param_spec_object ("document",
                         "Document",
                         "The current document",
                         XAPIAN_TYPE_DOCUMENT,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_term_generator_set_property;
  gobject_class->get_property = xapian_term_generator_get_property;
  gobject_class->dispose = xapian_term_generator_dispose;
  gobject_class->finalize = xapian_term_generator_finalize;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_term_generator_init (XapianTermGenerator *self)
{
  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (self);

  priv->mGenerator = new Xapian::TermGenerator ();
}

/**
 * xapian_term_generator_new:
 *
 * Creates a new #XapianTermGenerator instance.
 *
 * Returns: (transfer full): the newly created #XapianTermGenerator.
 */
XapianTermGenerator *
xapian_term_generator_new (void)
{
  return static_cast<XapianTermGenerator *> (g_object_new (XAPIAN_TYPE_TERM_GENERATOR, NULL));
}

/**
 * xapian_term_generator_set_stemmer:
 * @generator: a #XapianTermGenerator
 * @stemmer: a #XapianStem
 *
 * Sets the stemmer for @generator.
 */
void
xapian_term_generator_set_stemmer (XapianTermGenerator *generator,
                                   XapianStem          *stemmer)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));
  g_return_if_fail (XAPIAN_IS_STEM (stemmer));

  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (generator);

  if (priv->stemmer == stemmer)
    return;

  g_clear_object (&priv->stemmer);
  priv->stemmer = static_cast<XapianStem *> (g_object_ref (stemmer));

  priv->mGenerator->set_stemmer(*xapian_stem_get_internal (stemmer));

  g_object_notify_by_pspec (G_OBJECT (generator), obj_props[PROP_STEMMER]);
}

/**
 * xapian_term_generator_set_stemming_strategy:
 * @generator: a #XapianTermGenerator
 * @strategy: a #XapianStemStrategy value
 *
 * Sets the stemming strategy for @generator.
 */
void
xapian_term_generator_set_stemming_strategy (XapianTermGenerator *generator,
                                             XapianStemStrategy   strategy)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));

  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (generator);

  if (priv->stemming_strategy == strategy)
    return;

  priv->stemming_strategy = strategy;

  Xapian::TermGenerator::stem_strategy stem_strategy;

#ifdef XAPIAN_GLIB_ENABLE_DEBUG
  switch (priv->stemming_strategy)
    {
    case XAPIAN_STEM_STRATEGY_STEM_NONE:
      stem_strategy = Xapian::TermGenerator::STEM_NONE;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_SOME:
      stem_strategy = Xapian::TermGenerator::STEM_SOME;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_ALL:
      stem_strategy = Xapian::TermGenerator::STEM_ALL;
      break;

    case XAPIAN_STEM_STRATEGY_STEM_ALL_Z:
      stem_strategy = Xapian::TermGenerator::STEM_ALL_Z;
      break;

    default:
      g_assert_not_reached ();
    }
#else
  stem_strategy = (Xapian::TermGenerator::stem_strategy) priv->stemming_strategy;
#endif

  priv->mGenerator->set_stemming_strategy (stem_strategy);

  g_object_notify_by_pspec (G_OBJECT (generator), obj_props[PROP_STEMMING_STRATEGY]);
}

/**
 * xapian_term_generator_set_database:
 * @generator: a #XapianTermGenerator
 * @database: a #XapianWritableDatabase
 *
 * Sets the database used to store the indexed data.
 */
void
xapian_term_generator_set_database (XapianTermGenerator    *generator,
                                    XapianWritableDatabase *database)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));
  g_return_if_fail (XAPIAN_IS_WRITABLE_DATABASE (database));

  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (generator);

  if (priv->database == database)
    return;

  g_clear_object (&priv->database);
  priv->database = static_cast<XapianWritableDatabase *> (g_object_ref (database));

  Xapian::Database *db = xapian_database_get_internal (XAPIAN_DATABASE (database));
  Xapian::WritableDatabase *wdb = dynamic_cast<Xapian::WritableDatabase *> (db);

  priv->mGenerator->set_database (*wdb);

  g_object_notify_by_pspec (G_OBJECT (generator), obj_props[PROP_DATABASE]);
}

/**
 * xapian_term_generator_set_document:
 * @generator: a #XapianTermGenerator
 * @document: a #XapianDocument
 *
 * Sets the current document.
 */
void
xapian_term_generator_set_document (XapianTermGenerator *generator,
                                    XapianDocument      *document)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));
  g_return_if_fail (XAPIAN_IS_DOCUMENT (document));

  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (generator);

  if (priv->document == document)
    return;

  g_clear_object (&priv->document);
  priv->document = static_cast<XapianDocument *> (g_object_ref (document));

  priv->mGenerator->set_document (*xapian_document_get_internal (document));

  g_object_notify_by_pspec (G_OBJECT (generator), obj_props[PROP_DOCUMENT]);
}

/**
 * xapian_term_generator_index_text:
 * @generator: a #XapianTermGenerator
 * @data: the data to be indexed
 *
 * Indexes @data into the current document.
 *
 * See also: xapian_term_generator_index_text_full()
 */
void
xapian_term_generator_index_text (XapianTermGenerator *generator,
                                  const char          *data)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));
  g_return_if_fail (data != NULL);

  xapian_term_generator_index_text_full (generator, data, 1, NULL);
}

/**
 * xapian_term_generator_index_text_full:
 * @generator: a #XapianTermGenerator
 * @data: the data to be indexed
 * @wdf_inc: the increment of the WDF
 * @prefix: the default prefix for the indexed data
 *
 * Indexes @data into the current document.
 *
 * Unlike xapian_term_generator_index_text(), this function also
 * allows setting the WDF increment and the default prefix for
 * the indexed data.
 */
void
xapian_term_generator_index_text_full (XapianTermGenerator *generator,
                                       const char          *data,
                                       unsigned int         wdf_inc,
                                       const char          *prefix)
{
  g_return_if_fail (XAPIAN_IS_TERM_GENERATOR (generator));
  g_return_if_fail (data != NULL);

  XapianTermGeneratorPrivate *priv = XAPIAN_TERM_GENERATOR_GET_PRIVATE (generator);

  priv->mGenerator->index_text (std::string (data), wdf_inc, std::string (prefix));
}
