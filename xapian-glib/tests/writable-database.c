#include <glib.h>
#include "xapian-glib.h"

static void
writable_database_new_empty (void)
{
  XapianWritableDatabase *db = xapian_writable_database_new (NULL, XAPIAN_DATABASE_ACTION_CREATE, NULL);

  g_assert_nonnull (db);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  g_assert_true (XAPIAN_IS_WRITABLE_DATABASE (db));

  if (g_test_verbose ())
    {
      char *desc = xapian_database_get_description (XAPIAN_DATABASE (db));
      char *uuid = xapian_database_get_uuid (XAPIAN_DATABASE (db));

      g_test_message ("Opened database '%s' with UUID: '%s'", desc, uuid);

      g_free (desc);
      g_free (uuid);
    }

  g_object_unref (db);
  g_assert_null (db);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/writable-database/new-empty", writable_database_new_empty);

  return g_test_run ();
}
