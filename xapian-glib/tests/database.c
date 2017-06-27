#include <glib.h>
#include <glib/gstdio.h>
#include "xapian-glib.h"

/* Remove a directory and all files directly inside it. */
static void
delete_database (const char *dir)
{
  GDir *d = g_dir_open (dir, 0, NULL);
  const char *name;

  while ((name = g_dir_read_name (d)) != NULL)
    {
      char *path;

      if ((name[0] == '.' && name[1] == '\0') ||
          (name[0] == '.' && name[1] == '.' && name[2] == '\0'))
        continue;

      path = g_build_filename (dir, name, NULL);
      g_unlink (path);
      g_free (path);
    }

  g_dir_close (d);

  g_rmdir (dir);
}

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

static void
database_writable_new (void)
{
  GError *error = NULL;
  XapianWritableDatabase *db =
    xapian_writable_database_new ("doesexist",
                                  XAPIAN_DATABASE_ACTION_CREATE,
                                  &error);

  g_assert_nonnull (db);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  g_assert_true (XAPIAN_IS_WRITABLE_DATABASE (db));
  g_assert_true (XAPIAN_IS_DATABASE (db));

  g_object_unref (db);
  g_assert_null (db);

  g_assert_true (g_file_test ("doesexist", G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR));

  delete_database ("doesexist");
}

static void
database_writable_backend_glass (void)
{
  GError *error = NULL;
  XapianWritableDatabase *db =
    xapian_writable_database_new_with_backend ("glass-db",
                                               XAPIAN_DATABASE_ACTION_CREATE,
                                               XAPIAN_DATABASE_BACKEND_GLASS,
                                               &error);

  g_assert_nonnull (db);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  g_assert_true (XAPIAN_IS_WRITABLE_DATABASE (db));
  g_assert_true (XAPIAN_IS_DATABASE (db));

  g_object_unref (db);
  g_assert_null (db);

  g_assert_true (g_file_test ("glass-db", G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR));
  g_assert_true (g_file_test ("glass-db/termlist.glass", G_FILE_TEST_EXISTS));

  delete_database ("glass-db");
}

static void
database_writable_flags_no_termlist (void)
{
  GError *error = NULL;
  XapianWritableDatabase *db =
    xapian_writable_database_new_full ("glass-db",
                                       XAPIAN_DATABASE_ACTION_CREATE,
                                       XAPIAN_DATABASE_BACKEND_GLASS,
                                       XAPIAN_DATABASE_FLAGS_NO_TERMLIST,
                                       &error);

  g_assert_nonnull (db);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  g_assert_true (XAPIAN_IS_WRITABLE_DATABASE (db));
  g_assert_true (XAPIAN_IS_DATABASE (db));

  g_object_unref (db);
  g_assert_null (db);

  g_assert_true (g_file_test ("glass-db", G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR));
  g_assert_false (g_file_test ("glass-db/termlist.glass", G_FILE_TEST_EXISTS));

  delete_database ("glass-db");
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/database/new/empty", database_new_empty);
  g_test_add_func ("/database/new/non-existent", database_new_nonexistent);
  g_test_add_func ("/database/writable/new", database_writable_new);
  g_test_add_func ("/database/writable/backend/glass", database_writable_backend_glass);
  g_test_add_func ("/database/writable/flags/no-termlist", database_writable_flags_no_termlist);

  return g_test_run ();
}
