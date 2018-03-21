/* Copyright 2014  Endless Mobile
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __XAPIAN_GLIB_MACROS_H__
#define __XAPIAN_GLIB_MACROS_H__

#include <glib.h>
#include "xapian-glib-version.h"

#ifndef _XAPIAN_GLIB_EXTERN
#define _XAPIAN_GLIB_EXTERN extern
#endif

#ifdef XAPIAN_GLIB_DISABLE_DEPRECATION_WARNINGS
#define XAPIAN_GLIB_DEPRECATED _XAPIAN_GLIB_EXTERN
#define XAPIAN_GLIB_DEPRECATED_FOR(f) _XAPIAN_GLIB_EXTERN
#define XAPIAN_GLIB_UNAVAILABLE(maj,min) _XAPIAN_GLIB_EXTERN
#else
#define XAPIAN_GLIB_DEPRECATED G_DEPRECATED _XAPIAN_GLIB_EXTERN
#define XAPIAN_GLIB_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f) _XAPIAN_GLIB_EXTERN
#define XAPIAN_GLIB_UNAVAILABLE(maj,min) G_UNAVAILABLE(maj,min) _XAPIAN_GLIB_EXTERN
#endif

/* XXX: Each new cycle should add a new version symbol here */

/**
 * XAPIAN_GLIB_VERSION_2_0:
 *
 * A pre-processor macro that evaluates to the 2.0 version of Xapian-GLib,
 * in a format that can be used by %XAPIAN_GLIB_VERSION_MIN_REQUIRED and
 * %XAPIAN_GLIB_VERSION_MAX_ALLOWED.
 *
 * Since: 2.0
 */
#define XAPIAN_GLIB_VERSION_2_0 (G_ENCODE_VERSION (2, 0))

/* Evaluates to the current stable version.
 *
 * For development cycles, this means the next stable cycle.
 */
#if (XAPIAN_GLIB_MINOR_VERSION == 99)
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION + 1, 0))
#elif (XAPIAN_GLIB_MINOR_VERSION % 2)
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION + 1))
#else
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION))
#endif

/* Evaluates to the previous stable version */
#if (XAPIAN_GLIB_MINOR_VERSION == 99)
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION + 1, 0))
#elif (XAPIAN_GLIB_MINOR_VERSION % 2)
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION - 1))
#else
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION - 2))
#endif

/**
 * XAPIAN_GLIB_VERSION_MIN_REQUIRED:
 *
 * A pre-processor symbol that should be defined by the user prior to including
 * the `xapian-glib.h` header.
 *
 * The value should be one of the predefined Xapian-GLib version macros,
 * for instance: %XAPIAN_GLIB_VERSION_1_0, %XAPIAN_GLIB_VERSION_1_2, ...
 *
 * This symbol defines the earliest version of Xapian-GLib that a project
 * is required to be able to compile against.
 *
 * If the compiler is configured to warn about the use of deprecated symbols,
 * then using symbols that were deprecated in version %XAPIAN_GLIB_VERSION_MIN_REQUIRED,
 * or earlier versions, will emit a deprecation warning, but using functions
 * deprecated in later versions will not.
 *
 * See also: %XAPIAN_GLIB_VERSION_MAX_ALLOWED
 *
 * Since: 1.0
 */
#ifndef XAPIAN_GLIB_VERSION_MIN_REQUIRED
# define XAPIAN_GLIB_VERSION_MIN_REQUIRED      (XAPIAN_GLIB_VERSION_CUR_STABLE)
#endif

/**
 * XAPIAN_GLIB_VERSION_MAX_ALLOWED:
 *
 * A pre-processor symbol that should be defined by the user prior to including
 * the `xapian-glib.h` header.
 *
 * The value should be one of the predefined Xapian-GLib version macros,
 * for instance: %XAPIAN_GLIB_VERSION_1_0, %XAPIAN_GLIB_VERSION_1_2, ...
 *
 * This symbol defines the earliest version of Xapian-GLib that a project
 * is required to be able to compile against.
 *
 * If the compiler is configured to warn about the use of deprecated symbols,
 * then using symbols that were deprecated in version %XAPIAN_GLIB_VERSION_MAX_ALLOWED,
 * or later versions, will emit a deprecation warning, but using functions
 * deprecated in later versions will not.
 *
 * See also: %XAPIAN_GLIB_VERSION_MIN_REQUIRED
 *
 * Since: 1.0
 */
#ifndef XAPIAN_GLIB_VERSION_MAX_ALLOWED
# if XAPIAN_GLIB_VERSION_MIN_REQUIRED > XAPIAN_GLIB_VERSION_PREV_STABLE
#  define XAPIAN_GLIB_VERSION_MAX_ALLOWED      (XAPIAN_GLIB_VERSION_MIN_REQUIRED)
# else
#  define XAPIAN_GLIB_VERSION_MAX_ALLOWED      (XAPIAN_GLIB_VERSION_CUR_STABLE)
# endif
#endif

/* sanity checks */
#if XAPIAN_GLIB_VERSION_MAX_ALLOWED < XAPIAN_GLIB_VERSION_MIN_REQUIRED
#error "XAPIAN_GLIB_VERSION_MAX_ALLOWED must be >= XAPIAN_GLIB_VERSION_MIN_REQUIRED"
#endif
#if XAPIAN_GLIB_VERSION_MIN_REQUIRED < XAPIAN_GLIB_VERSION_2_0
#error "XAPIAN_GLIB_VERSION_MIN_REQUIRED must be >= XAPIAN_GLIB_VERSION_2_0"
#endif

/* XXX: Every new stable minor release should add a set of macros here */

#if XAPIAN_GLIB_VERSION_MIN_REQUIRED >= XAPIAN_GLIB_VERSION_2_0
# define XAPIAN_GLIB_DEPRECATED_IN_2_0                XAPIAN_GLIB_DEPRECATED
# define XAPIAN_GLIB_DEPRECATED_IN_2_0_FOR(f)         XAPIAN_GLIB_DEPRECATED_FOR(f)
#else
# define XAPIAN_GLIB_DEPRECATED_IN_2_0                _XAPIAN_GLIB_EXTERN
# define XAPIAN_GLIB_DEPRECATED_IN_2_0_FOR(f)         _XAPIAN_GLIB_EXTERN
#endif

#if XAPIAN_GLIB_VERSION_MAX_ALLOWED < XAPIAN_GLIB_VERSION_2_0
# define XAPIAN_GLIB_AVAILABLE_IN_2_0                 XAPIAN_GLIB_UNAVAILABLE(2, 0)
#else
# define XAPIAN_GLIB_AVAILABLE_IN_2_0                 _XAPIAN_GLIB_EXTERN
#endif

#endif /* __XAPIAN_GLIB_MACROS_H__ */
