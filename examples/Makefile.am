noinst_PROGRAMS = \
	simplesearch \
	simplestopper

simplesearch_CFLAGS = -I$(top_srcdir)/xapian-glib -I$(top_builddir)/xapian-glib $(XAPIAN_GLIB_CFLAGS)
simplesearch_LDADD = $(XAPIAN_GLIB_LIBS) $(top_builddir)/xapian-glib/libxapian-glib-1.0.la
simplesearch_SOURCES = simplesearch.c

simplestopper_CFLAGS = -I$(top_srcdir)/xapian-glib -I$(top_builddir)/xapian-glib $(XAPIAN_GLIB_CFLAGS)
simplestopper_LDADD = $(XAPIAN_GLIB_LIBS) $(top_builddir)/xapian-glib/libxapian-glib-1.0.la
simplestopper_SOURCES = simplestopper.c
