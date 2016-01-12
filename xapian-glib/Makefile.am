# preamble

NULL =

AM_CPPFLAGS =
AM_CFLAGS =
BUILT_SOURCES =

CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES =

lib_LTLIBRARIES =
bin_PROGRAMS =

# build tests after
SUBDIRS = . tests

# sources
source_h_public = \
	xapian-database.h \
	xapian-document.h \
	xapian-enquire.h \
	xapian-enums.h \
	xapian-glib-macros.h \
	xapian-glib-types.h \
	xapian-mset.h \
	xapian-posting-source.h \
	xapian-value-posting-source.h \
	xapian-value-weight-posting-source.h \
	xapian-query.h \
	xapian-query-parser.h \
	xapian-stem.h \
	xapian-simple-stopper.h \
	xapian-stopper.h \
	xapian-term-generator.h \
	xapian-utils.h \
	xapian-writable-database.h \
	$(NULL)

source_h_private = \
	xapian-database-private.h \
	xapian-document-private.h \
	xapian-error-private.h \
	xapian-mset-private.h \
	xapian-posting-source-private.h \
	xapian-private.h \
	xapian-query-private.h \
	xapian-stem-private.h \
	xapian-stopper-private.h \
	xapian-value-posting-source-private.h \
	$(NULL)

source_c_public = \
	xapian-database.cc \
	xapian-document.cc \
	xapian-enquire.cc \
	xapian-enums.cc \
	xapian-error.cc \
	xapian-mset.cc \
	xapian-mset-iterator.cc \
	xapian-posting-source.cc \
	xapian-value-posting-source.cc \
	xapian-value-weight-posting-source.cc \
	xapian-query.cc \
	xapian-query-parser.cc \
	xapian-stem.cc \
	xapian-simple-stopper.cc \
	xapian-stopper.cc \
	xapian-term-generator.cc \
	xapian-utils.cc \
	xapian-writable-database.cc \
	$(NULL)

source_c_private =

DISTCLEANFILES += xapian-glib-version.h
EXTRA_DIST += xapian-glib-version.h.in

# headers
xapianincludedir = $(includedir)/xapian-glib-1.0
nodist_xapianinclude_DATA = $(top_builddir)/xapian-glib/xapian-glib-version.h
dist_xapianinclude_DATA = \
	$(source_h_public) \
	$(top_srcdir)/xapian-glib/xapian-glib.h \
	$(NULL)

# library
libxapian_glib_1_0_la_CPPFLAGS = \
	-DXAPIAN_GLIB_COMPILATION=1 \
	-DG_LOG_DOMAIN=\"XapianGlib\" \
	-I$(top_srcdir)/xapian-glib \
	-I$(top_builddir)/xapian-glib \
	$(XAPIAN_GLIB_DEBUG_CFLAGS) \
	$(XAPIAN_GLIB_CFLAGS) \
	$(NULL)

libxapian_glib_1_0_la_LIBADD = $(XAPIAN_GLIB_LIBS) $(XAPIAN_LIBS)
libxapian_glib_1_0_la_LDFLAGS = $(XAPIAN_GLIB_LDFLAGS) -export-dynamic
libxapian_glib_1_0_la_SOURCES = $(source_h_public) $(source_h_private) $(source_c_public) $(source_c_private)

# Use a C linker for GCC, not C++; Don't link to libstdc++
if HAVE_GCC
libxapian_glib_1_0_la_LINK = $(LINK) $(libxapian_glib_1_0_la_LDFLAGS)
else
libxapian_glib_1_0_la_LINK = $(CXXLINK) $(libxapian_glib_1_0_la_LDFLAGS)
endif

lib_LTLIBRARIES += libxapian-glib-1.0.la

# pkg-config
pcfiles = xapian-glib-1.0.pc

xapian-glib-1.0.pc: xapian-glib.pc
	$(AM_V_GEN)cp -f $< $@

pkgconfig_DATA = $(pcfiles)
pkgconfigdir = $(libdir)/pkgconfig
CLEANFILES += $(pcfiles)
EXTRA_DIST += xapian-glib.pc.in

# introspection
-include $(INTROSPECTION_MAKEFILE)

if HAVE_INTROSPECTION
INTROSPECTION_GIRS = Xapian-1.0.gir

Xapian-1.0.gir: libxapian-glib-1.0.la Makefile

Xapian_1_0_gir_NAMESPACE = Xapian
Xapian_1_0_gir_VERSION = 1.0
Xapian_1_0_gir_LIBS = libxapian-glib-1.0.la
Xapian_1_0_gir_FILES = $(source_h_public) $(source_c_public) $(top_builddir)/xapian-glib/xapian-glib-version.h
Xapian_1_0_gir_CFLAGS = -DXAPIAN_GLIB_COMPILATION=1 -I$(top_srcdir) -I$(top_builddir) $(XAPIAN_GLIB_CFLAGS)
Xapian_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
Xapian_1_0_gir_SCANNERFLAGS = --warn-all --pkg-export xapian-glib-1.0 --c-include "xapian-glib.h"

girdir = $(datadir)/gir-1.0
dist_gir_DATA = Xapian-1.0.gir

typelibsdir = $(libdir)/girepository-1.0/
typelibs_DATA = Xapian-1.0.typelib

CLEANFILES += $(dist_gir_DATA) $(typelibs_DATA)
endif # HAVE_INTROSPECTION
