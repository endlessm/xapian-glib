include $(top_srcdir)/build/autotools/glib-tap.mk

LDADD = $(top_builddir)/xapian-glib/libxapian-glib-1.0.la $(XAPIAN_GLIB_LIBS)
AM_CPPFLAGS = -g -O0 \
	$(XAPIAN_GLIB_CFLAGS) \
	$(XAPIAN_GLIB_DEBUG_CFLAGS) \
	-I$(top_srcdir)/xapian-glib \
	-I$(top_builddir)/xapian-glib
DEFS = -DG_LOG_DOMAIN=\"Xapian-GLib\" -DEXEEXT=\"$(EXEEXT)\"

test_programs = \
	document \
	stem
