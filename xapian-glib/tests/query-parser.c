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
  GError *error = NULL;
  XapianQueryParser *query_parser = xapian_query_parser_new ();

  xapian_query_parser_set_default_op (query_parser, XAPIAN_QUERY_OP_AND);

  g_object_unref (query_parser);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/query-parser/default", query_parser_default);
  g_test_add_func ("/query-parser/set-default-op", query_parser_set_default_op);

  return g_test_run ();
}
