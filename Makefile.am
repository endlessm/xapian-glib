ACLOCAL_AMFLAGS = -I build/autotools

SUBDIRS = xapian-glib examples

EXTRA_DIST = \
	build/autotools/tap-driver.sh \
	build/autotools/tap-test \
	build/autotools/glib-tap.mk \
	build/autotools/glibtests.m4 \
	README.md \
	COPYING

DISTCHECK_CONFIGURE_FLAGS = --enable-maintainer-flags

if ENABLE_GCOV
# use recursive makes in order to ignore errors during check/perf
lcov:
	-$(MAKE) $(AM_MAKEFLAGS) check
	$(MAKE) $(AM_MAKEFLAGS) genlcov

# we have to massage the lcov.info file slightly to hide the effect of libtool
# placing the objects files in the .libs/ directory separate from the *.c
genlcov:
	$(LTP) --directory $(top_builddir) --capture --output-file xapian-glib-lcov.info --test-name XAPIAN_GLIB_TEST --no-checksum
	$(SED) -e 's#.libs/##' < xapian-glib-lcov.info > xapian-glib-lcov.info.tmp
	LANG=C $(LTP_GENHTML) --prefix $(top_builddir) --output-directory xapian-glib-lcov --title "Xapian-GLib Code Coverage" --show-details xapian-glib-lcov.info.tmp
	rm -f xapian-glib-lcov.info.tmp

lcov-clean:
	-$(LTP) --directory $(top_builddir) -z
	-$(RM) -rf xapian-glib-lcov.info xapian-glib-lcov
else
lcov genlcov lcov-clean:
	@echo You need to configure Graphene with --enable-code-coverage
endif

.PHONY: lcov genlcov lcov-clean
