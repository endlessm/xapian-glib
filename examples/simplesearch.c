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
  const char *query;
  XapianDatabase *db;
  XapianEnquire *enquire;
  GString *query_str;
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

  enquire = xapian_enquire_new (db, &error);
  if (error != NULL)
    {
      g_printerr ("Unable to open database at '%s' for enquire: %s\n",
                  db_path,
                  error->message);
      g_error_free (error);
      g_object_unref (db);
      return EXIT_FAILURE;
    }

  query_str = g_string_new (NULL);
  for (i = argc + 2; i < argc; i++)
    g_string_append (query_str, argv[i]);

  g_print ("Querying database at '%s': %s\n", db_path, query_str->str);

  g_string_free (query_str, TRUE);

  g_object_unref (enquire);
  g_object_unref (db);

  return EXIT_SUCCESS;
}
