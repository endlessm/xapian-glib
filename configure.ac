# bump micro_version to the next even number for each point release
# bump micro_version to the next odd number after each release
m4_define([xapian_major_version], [1])
m4_define([xapian_minor_version], [3])
m4_define([xapian_micro_version], [1])

m4_define([xapian_version], [xapian_major_version.xapian_minor_version.xapian_micro_version])

m4_define([xapian_release_status],
          [m4_if(m4_eval(xapian_micro_version % 2), [1], [git],
                 [m4_if(m4_eval(xapian_minor_version % 2), [1], [snapshot], [release])])])

# bump up by 1 for every micro release with no API changes, otherwise
# set to 0. after release, bump up by 1
m4_define([xapian_interface_age], [1])
m4_define([xapian_binary_age], [m4_eval(100 * xapian_minor_version + xapian_micro_version)])

m4_define([lt_current], [m4_eval(100 * xapian_minor_version + xapian_micro_version - xapian_interface_age)])
m4_define([lt_revision], [xapian_interface_age])
m4_define([lt_age], [m4_eval(xapian_binary_age - xapian_interface_age)])

m4_define([glib_req_version], [2.40.0])

AC_PREREQ([2.63])

AC_INIT([xapian-glib],
        [xapian_version],
        [https://github.com/endlessm/xapian-glib/issues/],
        [xapian-glib],
        [https://github.com/endlessm/xapian-glib])

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build/autotools])
AC_CONFIG_HEADER([xapian-glib/config.h])
AC_CONFIG_SRCDIR([xapian-glib/xapian-glib.h])

AM_INIT_AUTOMAKE([1.11.2 no-define foreign -Wno-portability dist-xz no-dist-gzip tar-ustar])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

AC_SYS_LARGEFILE
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
PKG_PROG_PKG_CONFIG([0.20])

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

dnl === Versioning ============================================================

XAPIAN_GLIB_MAJOR_VERSION=xapian_major_version
XAPIAN_GLIB_MINOR_VERSION=xapian_minor_version
XAPIAN_GLIB_MICRO_VERSION=xapian_micro_version
XAPIAN_GLIB_VERSION=xapian_version
XAPIAN_GLIB_RELEASE_STATUS=xapian_release_status

XAPIAN_GLIB_LT_CURRENT=lt_current
XAPIAN_GLIB_LT_REVISION=lt_revision
XAPIAN_GLIB_LT_AGE=lt_age
XAPIAN_GLIB_LT_VERSION="$XAPIAN_GLIB_LT_CURRENT:$XAPIAN_GLIB_LT_REVISION:$XAPIAN_GLIB_LT_AGE"
XAPIAN_GLIB_LDFLAGS="-version-info $XAPIAN_GLIB_LT_VERSION"

dnl === Platform checks =======================================================

AC_CANONICAL_HOST

AS_IF([test "x$GCC" = "xyes"],
      [
        # make symbols link locally
        BUILD_LDFLAGS="$BUILD_LDFLAGS -Bsymbolic-functions"

        # assorted compiler errors
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Wall -ansi"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=cast-align"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=shadow"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=format"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=format-security"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=format-nonliteral"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=init-self"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=empty-body"
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -Werror=undef"

        AS_CASE(["$host"],
                [*-*-mingw*],
                [
                  dnl on mingw32 we do -fvisibility=hidden and __declspec(dllexport)
                  AC_DEFINE([_XAPIAN_GLIB_EXTERN],
                            [__attribute__((visibility("default"))) __declspec(dllexport) extern],
                            [defines how to decorate public symbols while building])
                  BUILD_CXXFLAGS="$BUILD_CXXFLAGS -fvisibility=hidden"
                  BUILD_LDFLAGS="$BUILD_LDFLAGS -no-undefined"
                ],

                [
                  dnl everywhere else we do -fvisibility=hidden
                  AC_DEFINE([_XAPIAN_GLIB_EXTERN],
                            [__attribute__((visibility("default"))) extern],
                            [defines how to decorate public symbols while building])
                  BUILD_CXXFLAGS="$BUILD_CXXFLAGS -fvisibility-inlines-hidden"
                  BUILD_CXXFLAGS="$BUILD_CXXFLAGS -fvisibility=hidden"
                ])

        AS_CASE(["$host"],
                [arm-*-*],
                [
                  # request byte alignment on ARM
                  BUILD_CXXFLAGS="$BUILD_CXXFLAGS -mstructure-size-boundary=8"
                ])
      ]
)

AM_CONDITIONAL(HAVE_GCC, test "x$GCC" = "xyes")

dnl === Dependencies ==========================================================

AC_ARG_WITH([xapian-config], [AS_HELP_STRING([--with-xapian-config], [The xapian-config binary or 'pkgconfig'])],
            [xapian_config=$withval],
            [xapian_config=pkgconfig])

AS_IF([test "x$xapian_config" = xpkgconfig], [
  PKG_CHECK_MODULES(XAPIAN, [xapian-core])
  BUILD_CXXFLAGS="$BUILD_CXXFLAGS $XAPIAN_CFLAGS"
  XAPIAN_VERSION="`$PKG_CONFIG --modversion xapian-core`"
], [
  AC_CHECK_PROG([XAPIAN_CONFIG], $xapian_config, $xapian_config, no)
  AS_IF([test "x$XAPIAN_CONFIG" = xno], [
    AC_MSG_ERROR("*** No Xapian headers found. You need to install xapian-core ***")
  ])
  BUILD_CXXFLAGS="$BUILD_CXXFLAGS `$XAPIAN_CONFIG --cxxflags`"
  XAPIAN_LIBS="`$XAPIAN_CONFIG --ltlibs`"
  XAPIAN_VERSION="`$XAPIAN_CONFIG --version`"
])

PKG_CHECK_MODULES(XAPIAN_GLIB, [gobject-2.0 >= glib_req_version gio-2.0])

dnl === Enable debug level ====================================================

m4_define([debug_default], [m4_if(m4_eval(xapian_minor_version % 2), [1], [yes], [minimum])])

AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
                              [turn on debugging @<:@default=]debug_default[@:>@])],
              [],
              [enable_debug=debug_default])

AS_CASE([$enable_debug],

        [yes],
        [
          test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
          XAPIAN_GLIB_DEBUG_CFLAGS="-DXAPIAN_GLIB_ENABLE_DEBUG"
        ],

        [minimum],
        [
          XAPIAN_GLIB_DEBUG_CFLAGS="-DXAPIAN_GLIB_ENABLE_DEBUG -DG_DISABLE_CAST_CHECKS"
        ],

        [no],
        [
          XAPIAN_GLIB_DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"
        ],

        [AC_MSG_ERROR([Unknown argument to --enable-debug])]
)

dnl === Test suite ============================================================

GLIB_TESTS

dnl === Code coverage =========================================================

AC_ARG_ENABLE([code-coverage],
              [AS_HELP_STRING([--enable-code-coverage], [Enable code coverage])],
              [use_gcov=$enableval],
              [use_gcov=no])

AS_IF([test "x$use_gcov" = "xyes"],
      [
        dnl we need gcc:
        AS_IF([test "$GCC" != "yes"], [AC_MSG_ERROR([GCC is required for --enable-code-coverage])])

        dnl Check if ccache is being used
        AC_CHECK_PROG(SHTOOL, shtool, shtool)
        AS_CASE([`$SHTOOL path $CC`],
                [*ccache*], [gcc_ccache=yes],
                [gcc_ccache=no])

        if test "$gcc_ccache" = "yes" && (test -z "$CCACHE_DISABLE" || test "$CCACHE_DISABLE" != "1"); then
          AC_MSG_ERROR([ccache must be disabled when --enable-code-coverage option is used. You can disable ccache by setting environment variable CCACHE_DISABLE=1.])
        fi

        ltp_version_list="1.6 1.7 1.8 1.9 1.10"
        AC_CHECK_PROG(LTP, lcov, lcov)
        AC_CHECK_PROG(LTP_GENHTML, genhtml, genhtml)

        if test "$LTP"; then
          AC_CACHE_CHECK([for ltp version], xapian_cv_ltp_version,
                         [
                           xapian_cv_ltp_version=invalid
                           ltp_version=`$LTP -v 2>/dev/null | $SED -e 's/^.* //'`
                           for ltp_check_version in $ltp_version_list; do
                             if test "$ltp_version" = "$ltp_check_version"; then
                               xapian_cv_ltp_version="$ltp_check_version (ok)"
                             fi
                           done
                         ])
        else
          ltp_msg="To enable code coverage reporting you must have one of the following LTP versions installed: $ltp_version_list"
          AC_MSG_ERROR([$ltp_msg])
        fi

        case $xapian_cv_ltp_version in
          ""|invalid[)]
          ltp_msg="You must have one of the following versions of LTP: $ltp_version_list (found: $ltp_version)."
          AC_MSG_ERROR([$ltp_msg])
          LTP="exit 0;"
          ;;
        esac

        if test -z "$LTP_GENHTML"; then
          AC_MSG_ERROR([Could not find genhtml from the LTP package])
        fi

        AC_DEFINE(HAVE_GCOV, 1, [Whether you have gcov])

        dnl Remove all optimization flags from CFLAGS
        m4_changequote({,})
        BUILD_CXXFLAGS=`echo "$BUILD_CXXFLAGS" | $SED -e 's/-O[0-9]*//g'`
        m4_changequote([,])

        dnl Define the special gcc flags
        BUILD_CXXFLAGS="$BUILD_CXXFLAGS -O0 -fprofile-arcs -ftest-coverage"
        XAPIAN_GLIB_LIBS="$XAPIAN_GLIB_LIBS -lgcov"
      ])

AM_CONDITIONAL(ENABLE_GCOV, test "x$use_gcov" = "xyes")

dnl === Introspection =========================================================

GOBJECT_INTROSPECTION_CHECK([1.40.0])

dnl === API reference =========================================================

GTK_DOC_CHECK([1.20], [--flavour no-tmpl])

dnl === Output ================================================================

AC_SUBST(XAPIAN_GLIB_MAJOR_VERSION)
AC_SUBST(XAPIAN_GLIB_MICRO_VERSION)
AC_SUBST(XAPIAN_GLIB_MINOR_VERSION)
AC_SUBST(XAPIAN_GLIB_VERSION)
AC_SUBST(XAPIAN_GLIB_RELEASE_STATUS)
AC_SUBST(XAPIAN_GLIB_CFLAGS)
AC_SUBST(XAPIAN_GLIB_DEBUG_CFLAGS)
AC_SUBST(XAPIAN_GLIB_LIBS)
AC_SUBST(XAPIAN_LIBS)
AC_SUBST(BUILD_CXXFLAGS)
AC_SUBST(BUILD_LDFLAGS)

AC_CONFIG_FILES([
        Makefile

        xapian-glib/Makefile
        xapian-glib/xapian-glib.pc
        xapian-glib/xapian-glib-version.h

        xapian-glib/tests/Makefile

        examples/Makefile

        doc/Makefile
])

AC_OUTPUT

dnl === Summary ===============================================================

AC_MSG_NOTICE([
Xapian-GLib - $VERSION (${XAPIAN_GLIB_RELEASE_STATUS})

Build configuration:

  • Prefix: ${prefix}
  • Debug level: ${enable_debug}
  • Xapian version: ${XAPIAN_VERSION} (${xapian_config})
  • Compiler flags: ${BUILD_CXXFLAGS} ${BUILD_LDFLAGS}
  • Introspection: ${have_introspection}
])
