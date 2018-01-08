#include "xapian-glib.h"

static void
query_new_for_terms (void)
{
  const char *terms[] = {
    "rey", "finn", "rose", NULL,
  };
  g_autoptr(XapianQuery) q = xapian_query_new_for_terms (XAPIAN_QUERY_OP_OR, terms);

  g_assert_nonnull (q);
  g_assert_cmpint (xapian_query_get_length (q), ==, 3);
  g_assert_cmpstr (xapian_query_get_description (q), ==,
                   "Query((rey OR finn OR rose))");
}

static void
query_new_for_queries (void)
{
  GSList *queries = g_slist_prepend (NULL, xapian_query_new_for_term ("luke"));
  queries = g_slist_prepend (queries, xapian_query_new_for_term ("leia"));
  queries = g_slist_prepend (queries, xapian_query_new_match_all ());

  g_autoptr(XapianQuery) q = xapian_query_new_for_queries (XAPIAN_QUERY_OP_AND_NOT, queries);

  g_assert_nonnull (q);
  g_assert_cmpint (xapian_query_get_length (q), ==, 3);
  g_assert_cmpstr (xapian_query_get_description (q), ==,
                   "Query((<alldocuments> AND_NOT leia AND_NOT luke))");

  g_slist_free_full (queries, g_object_unref);
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/query/new-for-terms", query_new_for_terms);
  g_test_add_func ("/query/new-for-queries", query_new_for_queries);

  return g_test_run ();
}
