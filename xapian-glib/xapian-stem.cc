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

#include "xapian-stem-private.h"

#include "xapian-error-private.h"

#define XAPIAN_STEM_GET_PRIVATE(obj) \
  ((XapianStemPrivate *) xapian_stem_get_instance_private ((XapianStem *) (obj)))

typedef struct {
  Xapian::Stem *mStem;

  char *language;
} XapianStemPrivate;

enum {
  PROP_0,

  PROP_LANGUAGE,

  LAST_PROP
};

static GParamSpec *obj_props[LAST_PROP] = { NULL, };

static void initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (XapianStem, xapian_stem, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (XapianStem)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init))

Xapian::Stem *
xapian_stem_get_internal (XapianStem *stem)
{
  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (stem);

  return priv->mStem;
}

static gboolean
xapian_stem_init_internal (GInitable *initable,
                           GCancellable *cancellable,
                           GError **error)
{
  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (initable);

  if (priv->language == NULL)
    {
      priv->mStem = new Xapian::Stem ();

      return TRUE;
    }

  try
    {
      std::string language (priv->language);

      priv->mStem = new Xapian::Stem (language);
    }
  catch (const Xapian::Error &err)
    {
      GError *internal_error = NULL;

      xapian_error_to_gerror (err, &internal_error);
      g_propagate_error (error, internal_error);

      priv->mStem = NULL;

      return FALSE;
    }

  return TRUE;
}

static void
initable_iface_init (GInitableIface *iface)
{
  iface->init = xapian_stem_init_internal;
}

static void
xapian_stem_finalize (GObject *gobject)
{
  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (gobject);

  delete priv->mStem;
  g_free (priv->language);

  G_OBJECT_CLASS (xapian_stem_parent_class)->finalize (gobject);
}

static void
xapian_stem_set_property (GObject *gobject,
                          guint prop_id,
                          const GValue *value,
                          GParamSpec *pspec)
{
  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_LANGUAGE:
      g_free (priv->language);
      priv->language = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_stem_get_property (GObject *gobject,
                          guint prop_id,
                          GValue *value,
                          GParamSpec *pspec)
{
  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (gobject);

  switch (prop_id)
    {
    case PROP_LANGUAGE:
      g_value_set_string (value, priv->language);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
xapian_stem_class_init (XapianStemClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  obj_props[PROP_LANGUAGE] =
    g_param_spec_string ("language",
                         "Language",
                         "The language used by the stemmer",
                         NULL,
                         (GParamFlags) (G_PARAM_READWRITE |
                                        G_PARAM_CONSTRUCT_ONLY |
                                        G_PARAM_STATIC_STRINGS));

  gobject_class->set_property = xapian_stem_set_property;
  gobject_class->get_property = xapian_stem_get_property;
  gobject_class->finalize = xapian_stem_finalize;

  g_object_class_install_properties (gobject_class, LAST_PROP, obj_props);
}

static void
xapian_stem_init (XapianStem *self)
{
}

XapianStem *
xapian_stem_new (void)
{
  /* this should really always succeed */
  return static_cast<XapianStem *> (g_initable_new (XAPIAN_TYPE_STEM,
                                                    NULL, NULL,
                                                    NULL));
}

XapianStem *
xapian_stem_new_for_language (const char *language,
                              GError    **error)
{
  g_return_val_if_fail (language != NULL, NULL);

  return static_cast<XapianStem *> (g_initable_new (XAPIAN_TYPE_STEM,
                                                    NULL, error,
                                                    "language", language,
                                                    NULL));
}

/**
 * xapian_stem_get_description:
 * @stem: ...
 *
 * ...
 *
 * Returns: (transfer full): ...
 */
char *
xapian_stem_get_description (XapianStem *stem)
{
  g_return_val_if_fail (XAPIAN_IS_STEM (stem), NULL);

  XapianStemPrivate *priv = XAPIAN_STEM_GET_PRIVATE (stem);

  std::string desc = priv->mStem->get_description ();

  return g_strdup (desc.c_str ());
}

/**
 * xapian_stem_get_available_languages:
 *
 * ...
 *
 * Returns: (array zero-terminated=1) (transfer full): ...
 */
char **
xapian_stem_get_available_languages (void)
{
  std::string languages = Xapian::Stem::get_available_languages ();

  return g_strsplit (languages.c_str (), " ", -1);
}
