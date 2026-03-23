#include <glib.h>
#include "xapian-glib.h"

static void
query_parser_default (void)
{
  XapianQueryParser *query_parser = xapian_query_parser_new ();

  g_assert_nonnull (query_parser);
  g_assert_true (XAPIAN_IS_QUERY_PARSER (query_parser));

  g_object_unref (query_parser);
}

static void
query_parser_set_default_op (void)
{
  XapianQueryParser *query_parser = xapian_query_parser_new ();

  xapian_query_parser_set_default_op (query_parser, XAPIAN_QUERY_OP_AND);

  g_object_unref (query_parser);
}

static void
query_parser_flags (void)
{
  GError *error = NULL;
  XapianQueryParser *query_parser = xapian_query_parser_new ();

  XapianQuery *q = NULL;

  q = xapian_query_parser_parse_query_full (query_parser,
                                            "w?ldc*d",
                                            XAPIAN_QUERY_PARSER_FEATURE_WILDCARD_GLOB,
                                            "",
                                            &error);
  g_assert_nonnull (q);
  g_assert_cmpstr (xapian_query_get_description (q), ==, "Query(WILDCARD SYNONYM w?ldc*d)");
  g_object_unref (q);

  q = xapian_query_parser_parse_query_full (query_parser,
                                            "fuzi~",
                                            XAPIAN_QUERY_PARSER_FEATURE_FUZZY,
                                            "",
                                            &error);
  g_assert_nonnull (q);
  g_assert_cmpstr (xapian_query_get_description (q), ==, "Query(EDIT_DISTANCE SYNONYM fuzi~2)");
  g_object_unref (q);

  q = xapian_query_parser_parse_query_full (query_parser,
                                            "\"phrase search\"",
                                            XAPIAN_QUERY_PARSER_FEATURE_DEFAULT,
                                            "",
                                            &error);
  g_assert_nonnull (q);
  g_assert_cmpstr (xapian_query_get_description (q), ==, "Query((phrase@1 PHRASE 2 search@2))");
  g_object_unref (q);

  q = xapian_query_parser_parse_query_full (query_parser,
                                            "\"phrase search\"",
                                            XAPIAN_QUERY_PARSER_FEATURE_DEFAULT |
                                            XAPIAN_QUERY_PARSER_FEATURE_NO_POSITIONS,
                                            "",
                                            &error);
  g_assert_nonnull (q);
  g_assert_cmpstr (xapian_query_get_description (q), ==, "Query((phrase@1 AND search@2))");
  g_object_unref (q);

  g_object_unref (query_parser);
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/query-parser/default", query_parser_default);
  g_test_add_func ("/query-parser/set-default-op", query_parser_set_default_op);
  g_test_add_func ("/query-parser/flags", query_parser_flags);

  return g_test_run ();
}
