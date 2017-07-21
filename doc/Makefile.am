DOC_MODULE = xapian-glib
DOC_MAIN_SGML_FILE = xapian-glib-docs.xml
DOC_SOURCE_DIR = $(top_srcdir)/xapian-glib $(top_builddir)/xapian-glib

SCAN_OPTIONS = --rebuild-types
SCANGOBJ_OPTIONS =
MKDB_OPTIONS = --output-format=xml --name-space=xapian --source-suffixes=c,h,cc
FIXXREF_OPTIONS =

HFILE_GLOB = $(top_srcdir)/xapian-glib/*.h $(top_builddir)/xapian-glib/*.h
CFILE_GLOB = $(top_srcdir)/xapian-glib/*.cc

IGNORE_HFILES = \
	config.h \
	xapian-database-private.h \
	xapian-document-private.h \
	xapian-error-private.h \
	xapian-mset-private.h \
	xapian-posting-source-private.h \
	xapian-value-posting-source-private.h \
	xapian-query-private.h \
	xapian-stem-private.h \
	xapian-stopper-private.h \
	xapian-term-iterator-private.h \
	xapian-glib-macros.h \
	xapian-glib.h

EXTRA_HFILES =

HTML_IMAGES =

content_files =
expand_content_files = 

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(XAPIAN_GLIB_CFLAGS)
GTKDOC_LIBS = $(top_builddir)/xapian-glib/libxapian-glib-1.0.la $(XAPIAN_GLIB_LIBS)

include $(top_srcdir)/gtk-doc.make

if ENABLE_GTK_DOC
TESTS_ENVIRONMENT = \
	DOC_MODULE=$(DOC_MODULE) \
	DOC_MAIN_SGML_FILE=$(DOC_MAIN_SGML_FILE) \
	SRCDIR=$(abs_srcdir) \
	BUILDDIR=$(abs_builddir)

TESTS = $(GTKDOC_CHECK)
endif
