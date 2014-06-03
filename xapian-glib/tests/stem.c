#include <glib.h>
#include "xapian-glib.h"

static void
stem_default (void)
{
  XapianStem *stem = xapian_stem_new ();

  g_assert_nonnull (stem);
  g_assert_true (XAPIAN_IS_STEM (stem));

  g_object_unref (stem);
}

static void
stem_none (void)
{
  GError *error = NULL;
  XapianStem *stem;

  stem = xapian_stem_new_for_language (XAPIAN_STEM_LANGUAGE_NONE, &error);
  g_assert_no_error (error);

  g_object_unref (stem);
}

static void
stem_get_available_languages (void)
{
  char **langs;
  unsigned int n_langs, i;

  langs = xapian_stem_get_available_languages ();
  g_assert_nonnull (langs);

  n_langs = g_strv_length (langs);
  for (i = 0; i < n_langs; i += 1)
    {
      const char *lang = langs[i];
      GError *error = NULL;
      XapianStem *stem;

      if (g_test_verbose ())
        g_test_message ("creating stemmer for language '%s'", lang);
      
      stem = xapian_stem_new_for_language (lang, &error);
      g_assert_no_error (error);
      g_assert_nonnull (stem);

      g_object_unref (stem);
    }

  g_strfreev (langs);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/stem/default", stem_default);
  g_test_add_func ("/stem/none", stem_none);
  g_test_add_func ("/stem/get-available-languages", stem_get_available_languages);

  return g_test_run ();
}
