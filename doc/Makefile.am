DOC_MODULE = xapian-glib
DOC_MAIN_SGML_FILE = xapian-glib-docs.xml
DOC_SOURCE_DIR = $(top_srcdir)/xapian-glib

SCAN_OPTIONS = --rebuild-types
SCANGOBJ_OPTIONS =
MKDB_OPTIONS = --output-format=xml --name-space=xapian
FIXXREF_OPTIONS =

HFILE_GLOB = $(top_srcdir)/xapian-glib/*.h $(top_builddir)/xapian-glib/*.h
CFILE_GLOB = $(top_srcdir)/xapian-glib/*.cc

IGNORE_HFILES = \
	$(top_builddir)/xapian-glib/config.h \
	$(top_srcdir)/xapian-glib/xapian-database-private.h \
	$(top_srcdir)/xapian-glib/xapian-document-private.h \
	$(top_srcdir)/xapian-glib/xapian-error-private.h \
	$(top_srcdir)/xapian-glib/xapian-mset-private.h \
	$(top_srcdir)/xapian-glib/xapian-query-private.h \
	$(top_srcdir)/xapian-glib/xapian-stem-private.h \
	$(top_srcdir)/xapian-glib/xapian-glib-macros.h \
	$(top_srcdir)/xapian-glib/xapian-glib.h

EXTRA_HFILES =

HTML_IMAGES =

content_files =
expand_content_files = 

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(XAPIAN_GLIB_CFLAGS)
GTKDOC_LIBS = $(top_builddir)/xapian-glib/libxapian-glib-1.0.la $(XAPIAN_GLIB_LIBS)

include $(top_srcdir)/gtk-doc.make
