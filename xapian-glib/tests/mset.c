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
mset_iterator (void)
{
  GError *error = NULL;
  XapianWritableDatabase *wdb =
    xapian_writable_database_new_full ("mset-db",
                                       XAPIAN_DATABASE_ACTION_CREATE_OR_OVERWRITE,
                                       XAPIAN_DATABASE_BACKEND_DEFAULT,
                                       0,
                                       &error);

  g_assert_nonnull (wdb);

  g_object_add_weak_pointer (G_OBJECT (wdb), (gpointer *) &wdb);

  XapianDocument *doc = xapian_document_new ();

  g_object_add_weak_pointer (G_OBJECT (doc), (gpointer *) &doc);

  xapian_document_add_term (doc, "all");
  xapian_document_add_term (doc, "one");
  xapian_writable_database_add_document (wdb, doc, NULL, &error);
  xapian_document_remove_term (doc, "one");

  xapian_document_add_term (doc, "some");
  xapian_document_add_term (doc, "two");
  xapian_writable_database_add_document (wdb, doc, NULL, &error);
  xapian_document_remove_term (doc, "two");

  xapian_document_add_term (doc, "three");
  xapian_document_add_term (doc, "last");
  xapian_writable_database_add_document (wdb, doc, NULL, &error);

  g_object_unref (wdb);
  g_assert_null (wdb);

  g_object_unref (doc);
  g_assert_null (doc);

  XapianDatabase *db =
    xapian_database_new_with_path ("mset-db", &error);

  g_object_add_weak_pointer (G_OBJECT (db), (gpointer *) &db);

  XapianEnquire *enquire = xapian_enquire_new (db, &error);

  g_object_add_weak_pointer (G_OBJECT (enquire), (gpointer *) &enquire);

  g_assert_nonnull (enquire);

  g_autoptr(XapianQuery) q = xapian_query_new_for_term ("some");

  g_assert_nonnull (q);
  g_assert_cmpstr (xapian_query_get_description (q), ==, "Query(some)");

  xapian_enquire_set_query (enquire, q, 0);

  XapianMSet *mset = xapian_enquire_get_mset (enquire, 0, 10, &error);
  g_object_add_weak_pointer (G_OBJECT (mset), (gpointer *) &mset);

  g_assert_nonnull (mset);
  g_assert_false (xapian_mset_is_empty (mset));
  g_assert_cmpint (xapian_mset_get_size (mset), ==, 2);

  XapianMSetIterator *msetit = xapian_mset_get_begin (mset);
  g_object_add_weak_pointer (G_OBJECT (msetit), (gpointer *) &msetit);

  g_assert_true (xapian_mset_iterator_next (msetit));
  g_assert_cmpint (xapian_mset_iterator_get_doc_id (msetit, &error), ==, 2);
  g_assert_true (xapian_mset_iterator_next (msetit));
  g_assert_cmpint (xapian_mset_iterator_get_doc_id (msetit, &error), ==, 3);
  g_assert_false (xapian_mset_iterator_next (msetit));

  g_object_unref (msetit);
  g_assert_null (msetit);

  msetit = xapian_mset_get_end (mset);
  g_object_add_weak_pointer (G_OBJECT (msetit), (gpointer *) &msetit);

  g_assert_true (xapian_mset_iterator_prev (msetit));
  g_assert_cmpint (xapian_mset_iterator_get_doc_id (msetit, &error), ==, 3);
  g_assert_true (xapian_mset_iterator_prev (msetit));
  g_assert_cmpint (xapian_mset_iterator_get_doc_id (msetit, &error), ==, 2);
  g_assert_false (xapian_mset_iterator_prev (msetit));

  g_object_unref (msetit);
  g_assert_null (msetit);

  g_object_unref (enquire);
  g_assert_null (enquire);

  g_object_unref (mset);
  g_assert_null (mset);

  g_object_unref (db);
  g_assert_null (db);

  delete_database ("mset-db");
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/mset/iterator", mset_iterator);

  return g_test_run ();
}
