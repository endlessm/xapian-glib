#include <glib.h>
#include "xapian-glib.h"

static void
database_new_empty (void)
{
  XapianDatabase *db = xapian_database_new (NULL);

  g_assert_nonnull (db);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  g_assert (XAPIAN_IS_DATABASE (db));

  g_assert_nonnull (xapian_database_get_uuid (db));
  g_assert_cmpstr (xapian_database_get_uuid (db), ==, "");

  g_object_unref (db);
  g_assert_null (db);
}

static void
database_new_nonexistent (void)
{
  GError *error = NULL;
  XapianDatabase *db = xapian_database_new_with_path ("doesnotexist", &error);

  g_assert_null (db);
  g_assert_error (error, XAPIAN_ERROR, XAPIAN_ERROR_DATABASE_OPENING);

  g_error_free (error);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/database/new/empty", database_new_empty);
  g_test_add_func ("/database/new/non-existent", database_new_nonexistent);

  return g_test_run ();
}
