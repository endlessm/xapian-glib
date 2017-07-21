#include <glib.h>
#include "xapian-glib.h"

static void
document_new_empty (void)
{
  XapianDocument *empty = xapian_document_new ();

  g_assert_nonnull (empty);
  g_assert_cmpint (xapian_document_get_values_count (empty), ==, 0);

  g_object_unref (empty);
}

static void
document_add_value (void)
{
  XapianDocument *doc = xapian_document_new ();
  char *val;

  g_object_add_weak_pointer (G_OBJECT (doc), (gpointer *) &doc);

  xapian_document_add_value (doc, 42, "hello world");

  val = xapian_document_get_value (doc, 42);
  g_assert_cmpstr (val, ==, "hello world");
  g_free (val);

  g_object_unref (doc);
  g_assert_null (doc);
}

static void
document_remove_value (void)
{
  XapianDocument *doc = xapian_document_new ();
  char *val;

  g_object_add_weak_pointer (G_OBJECT (doc), (gpointer *) &doc);

  xapian_document_add_value (doc, 42, "hello world");
  g_assert_cmpint (xapian_document_get_values_count (doc), ==, 1);

  val = xapian_document_get_value (doc, 42);
  g_assert_cmpstr (val, ==, "hello world");

  g_free (val);

  xapian_document_remove_value (doc, 42);
  g_assert_cmpint (xapian_document_get_values_count (doc), ==, 0);
  val = xapian_document_get_value (doc, 42);
  g_assert_cmpstr (val, ==, "");
  g_free (val);

  g_object_unref (doc);
  g_assert_null (doc);
}

static void
document_clear_values (void)
{
  XapianDocument *doc = xapian_document_new ();
  char *val;

  g_object_add_weak_pointer (G_OBJECT (doc), (gpointer *) &doc);

  xapian_document_add_value (doc, 42, "hello world");
  g_assert_cmpint (xapian_document_get_values_count (doc), ==, 1);

  val = xapian_document_get_value (doc, 42);
  g_assert_cmpstr (val, ==, "hello world");
  g_free (val);

  xapian_document_clear_values (doc);
  g_assert_cmpint (xapian_document_get_values_count (doc), ==, 0);
  val = xapian_document_get_value (doc, 42);
  g_assert_cmpstr (val, ==, "");
  g_free (val);

  g_object_unref (doc);
  g_assert_null (doc);
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/document/new-empty", document_new_empty);
  g_test_add_func ("/document/add-value", document_add_value);
  g_test_add_func ("/document/remove-value", document_remove_value);
  g_test_add_func ("/document/clear-values", document_clear_values);

  return g_test_run ();
}
