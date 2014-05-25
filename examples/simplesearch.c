#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <xapian-glib.h>

static gboolean show_version = FALSE;

int
main (int argc, char *argv[])
{
  static GOptionEntry options[] = {
    { "version", 0, 0, G_OPTION_ARG_NONE, &show_version, "Show version and exit", NULL },
    { NULL },
  };

  GError *error = NULL;
  GOptionContext *context;
  const char *db_path;
  XapianDatabase *db;
  XapianEnquire *enquire;
  XapianQueryParser *qp;
  XapianStem *stemmer;
  XapianQuery *query;
  XapianMSet *matches;
  XapianMSetIterator iter = XAPIAN_MSET_ITERATOR_INIT;
  GString *query_str;
  char *desc;
  int i;

  g_set_prgname ("simplesearch");

  context = g_option_context_new ("DB_PATH QUERY");
  g_option_context_add_main_entries (context, options, NULL);
  g_option_context_set_ignore_unknown_options (context, TRUE);
  g_option_context_set_help_enabled (context, TRUE);

  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      if (error != NULL)
        g_printerr ("%s\n", error->message);

      g_printerr ("Usage: %s DB_PATH QUERY\n", argv[0]);

      g_clear_error (&error);
      return EXIT_FAILURE;
    }

  if (show_version)
    {
      g_print ("%s:\n"
               "Xapian-GLib version: %d.%d.%d\n"
               "Xapian version: %d.%d.%d\n",
               argv[0],
               XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION, XAPIAN_GLIB_MICRO_VERSION,
               xapian_glib_major_version (),
               xapian_glib_minor_version (),
               xapian_glib_revision ());
      return EXIT_SUCCESS;
    }

  g_option_context_free (context);

  if (argc < 3)
    {
      g_printerr ("Usage: %s DB_PATH QUERY\n", argv[0]);
      return EXIT_FAILURE;
    }

  db_path = argv[1];

  db = xapian_database_new_with_path (db_path, &error);
  if (error != NULL)
    {
      g_printerr ("Unable to open database at '%s': %s\n",
                  db_path,
                  error->message);
      g_error_free (error);
      return EXIT_FAILURE;
    }

  query_str = g_string_new (NULL);
  for (i = 2; i < argc; i++)
    {
      g_string_append_c (query_str, ' ');
      g_string_append (query_str, argv[i]);
    }

  g_print ("Querying database at '%s' with '%s'\n", db_path, query_str->str);

  stemmer = xapian_stem_new_for_language ("english", &error);
  if (error != NULL)
    {
      g_printerr ("Unable to retrieve stemmer for english: %s\n",
                  error->message);

      g_error_free (error);
      g_string_free (query_str, TRUE);

      g_object_unref (db);

      return EXIT_FAILURE;
    }

  qp = xapian_query_parser_new ();
  xapian_query_parser_set_stemmer (qp, stemmer);
  xapian_query_parser_set_stemming_strategy (qp, XAPIAN_STEM_STRATEGY_STEM_SOME);
  xapian_query_parser_set_database (qp, db);

  query = xapian_query_parser_parse_query (qp, query_str->str, &error);
  if (error != NULL)
    {
      g_printerr ("Unable to parse query '%s': %s\n",
                  query_str->str,
                  error->message);

      g_error_free (error);
      g_string_free (query_str, TRUE);

      g_object_unref (qp);
      g_object_unref (stemmer);
      g_object_unref (db);

      return EXIT_FAILURE;
    }

  desc = xapian_query_get_description (query);
  g_print ("Parsed query: '%s'\n", desc);
  g_free (desc);

  enquire = xapian_enquire_new (db, &error);
  g_assert (enquire != NULL);
  if (error != NULL)
    {
      g_printerr ("Unable to open database at '%s' for enquire: %s\n",
                  db_path,
                  error->message);

      g_error_free (error);
      g_string_free (query_str, TRUE);

      g_object_unref (qp);
      g_object_unref (stemmer);
      g_object_unref (db);

      return EXIT_FAILURE;
    }

  xapian_enquire_set_query (enquire, query, xapian_query_get_length (query));

  matches = xapian_enquire_get_mset (enquire, 0, 10, &error);
  if (error != NULL)
    {
      g_printerr ("Unable to retrieve matches: %s\n", error->message);

      g_error_free (error);
      g_string_free (query_str, TRUE);

      g_object_unref (qp);
      g_object_unref (stemmer);
      g_object_unref (enquire);
      g_object_unref (db);

      return EXIT_FAILURE;
    }

  g_print ("%d results found.\n", xapian_mset_get_matches_estimated (matches));
  g_print ("Matches 1-%d:\n\n", xapian_mset_get_size (matches));

  xapian_mset_iterator_init (&iter, matches);

  while (xapian_mset_iterator_next (&iter))
    {
      XapianDocument *doc = xapian_mset_iterator_get_document (&iter, &error);
      char *data;

      if (error != NULL)
        {
          g_printerr ("Unable to retrieve document: %s\n", error->message);
          g_error_free (error);
          continue;
        }

      data = xapian_document_get_data (doc);

      g_print ("%d: %.3f docid=%u [%s]\n",
               xapian_mset_iterator_get_rank (&iter) + 1,
               xapian_mset_iterator_get_weight (&iter),
               xapian_mset_iterator_get_docid (&iter),
               data);

      g_free (data);
    }

  xapian_mset_iterator_clear (&iter);

  g_string_free (query_str, TRUE);

  g_object_unref (matches);
  g_object_unref (query);
  g_object_unref (qp);
  g_object_unref (stemmer);
  g_object_unref (enquire);
  g_object_unref (db);

  return EXIT_SUCCESS;
}
