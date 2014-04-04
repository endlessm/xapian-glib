# bump micro_version to the next even number for each point release
# bump micro_version to the next odd number after each release
m4_define([xapian_major_version], [0])
m4_define([xapian_minor_version], [99])
m4_define([xapian_micro_version], [1])

m4_define([xapian_version], [xapian_major_version.xapian_minor_version.xapian_micro_version])

m4_define([xapian_release_status],
          [m4_if(m4_eval(xapian_micro_version % 2), [1], [git],
                 [m4_if(m4_eval(xapian_minor_version % 2), [1], [snapshot],
                                                              [release])])])

# bump up by 1 for every micro release with no API changes, otherwise
# set to 0. after release, bump up by 1
m4_define([xapian_interface_age], [1])
m4_define([xapian_binary_age], [m4_eval(100 * xapian_minor_version + xapian_micro_version)])

m4_define([lt_current], [m4_eval(100 * xapian_minor_version + xapian_micro_version - xapian_interface_age)])
m4_define([lt_revision], [xapian_interface_age])
m4_define([lt_age], [m4_eval(xapian_binary_age - xapian_interface_age)])

m4_define([glib_req_version], [2.38.0])

AC_PREREQ([2.63])

AC_INIT([xapian-glib],
        [xapian_version],
        [],
        [xapian-glib],
        [])

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build/autotools])
AC_CONFIG_HEADER([xapian-glib/config.h])
AC_CONFIG_SRCDIR([xapian-glib/xapian-glib.h])

AM_INIT_AUTOMAKE([1.13 no-define foreign -Wno-portability dist-xz no-dist-gzip tar-ustar])
AM_SILENT_RULES([yes])

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

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
AC_SUBST(XAPIAN_GLIB_MAJOR_VERSION)
AC_SUBST(XAPIAN_GLIB_MICRO_VERSION)
AC_SUBST(XAPIAN_GLIB_MINOR_VERSION)
AC_SUBST(XAPIAN_GLIB_VERSION)
AC_SUBST(XAPIAN_GLIB_RELEASE_STATUS)

XAPIAN_GLIB_LT_CURRENT=lt_current
XAPIAN_GLIB_LT_REVISION=lt_revision
XAPIAN_GLIB_LT_AGE=lt_age
XAPIAN_GLIB_LT_VERSION="$XAPIAN_GLIB_LT_CURRENT:$XAPIAN_GLIB_LT_REVISION:$XAPIAN_GLIB_LT_AGE"
XAPIAN_GLIB_LDFLAGS="-version-info $XAPIAN_GLIB_LT_VERSION"

dnl === Platform checks =======================================================

platform_linux=no
platform_win32=no

AC_CANONICAL_HOST

AS_IF([test "x$GCC" = "xyes"],
      [
        # make symbols link locally
        LDFLAGS="$LDFLAGS -Bsymbolic-functions"

        # assorted compiler errors
        CXXFLAGS="$CXXFLAGS -Werror=cast-align"
        CXXFLAGS="$CXXFLAGS -Werror=shadow"
        CXXFLAGS="$CXXFLAGS -Werror=format"
        CXXFLAGS="$CXXFLAGS -Werror=format-security"
        CXXFLAGS="$CXXFLAGS -Werror=format-nonliteral"
        CXXFLAGS="$CXXFLAGS -Werror=init-self"
        CXXFLAGS="$CXXFLAGS -Werror=empty-body"

        AS_CASE(["$host"],
                [*-*-mingw*],
                [
                  dnl on mingw32 we do -fvisibility=hidden and __declspec(dllexport)
                  AC_DEFINE([_XAPIAN_GLIB_EXTERN],
                            [__attribute__((visibility("default"))) __declspec(dllexport) extern],
                            [defines how to decorate public symbols while building])
                  CXXFLAGS="$CXXFLAGS -fvisibility=hidden"
                  LDFLAGS="$LDFLAGS -no-undefined"
                ],

                [
                  dnl everywhere else we do -fvisibility=hidden
                  AC_DEFINE([_XAPIAN_GLIB_EXTERN],
                            [__attribute__((visibility("default"))) extern],
                            [defines how to decorate public symbols while building])
                  CXXFLAGS="$CXXFLAGS -fvisibility-inlines-hidden"
                  CXXFLAGS="$CXXFLAGS -fvisibility=hidden"
                ])

        AS_CASE(["$host"],
                [arm-*-*],
                [
                  # request byte alignment on ARM
                  CXXFLAGS="$CXXFLAGS -mstructure-size-boundary=8"
                ])
      ]
)

AM_CONDITIONAL(HAVE_GCC, test "x$GCC" = "xyes")

dnl === Dependencies ==========================================================

AC_CHECK_PROG([XAPIAN_CONFIG], [xapian-config], [xapian-config])
AS_IF([test "$XAPIAN_CONFIG"],
      [ found_xapian=yes ],
      [ AC_MSG_ERROR(["*** No Xapian headers found ***"]) ])

CXXFLAGS="$CXXFLAGS `$XAPIAN_CONFIG --cxxflags`"

XAPIAN_LIBS="`$XAPIAN_CONFIG --ltlibs`"
AC_SUBST(XAPIAN_LIBS)

PKG_CHECK_MODULES(XAPIAN_GLIB, [gobject-2.0 >= glib_req_version gio-2.0])
AC_SUBST(XAPIAN_GLIB_CFLAGS)
AC_SUBST(XAPIAN_GLIB_LIBS)

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

AC_SUBST(XAPIAN_GLIB_DEBUG_CFLAGS)

dnl === Test suite ============================================================

GLIB_TESTS

dnl === Introspection =========================================================

GOBJECT_INTROSPECTION_CHECK([1.38.0])

AC_CONFIG_FILES([
        Makefile

        xapian-glib/Makefile
        xapian-glib/xapian-glib.pc
        xapian-glib/xapian-glib-version.h

        xapian-glib/tests/Makefile
])

AC_OUTPUT

dnl === Summary ===============================================================

AC_MSG_NOTICE([
Xapian-GLib - $VERSION (${XAPIAN_GLIB_RELEASE_STATUS})

Build configuration:

  • Prefix: ${prefix}
  • Debug level: ${enable_debug}
  • Compiler flags: ${CXXFLAGS}
  • Introspection: ${have_introspection}
])
