noinst_PROGRAMS = \
	simplesearch

simplesearch_CFLAGS = -I$(top_srcdir)/xapian-glib -I$(top_builddir)/xapian-glib $(XAPIAN_GLIB_CFLAGS)
simplesearch_LDADD = $(XAPIAN_GLIB_LIBS) $(top_builddir)/xapian-glib/libxapian-glib-1.0.la
simplesearch_SOURCES = simplesearch.c
