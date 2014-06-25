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

#define XAPIAN_GLIB_AVAILABLE_IN_ALL _XAPIAN_GLIB_EXTERN

#define XAPIAN_GLIB_VERSION_1_0 (G_ENCODE_VERSION (1, 0))

#define XAPIAN_GLIB_VERSION_1_2 (G_ENCODE_VERSION (1, 2))

#if (XAPIAN_GLIB_MINOR_VERSION == 99)
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION + 1, 0))
#elif (XAPIAN_GLIB_MINOR_VERSION % 2)
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION + 1))
#else
#define XAPIAN_GLIB_VERSION_CUR_STABLE         (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION))
#endif

/* evaluates to the previous stable version */
#if (XAPIAN_GLIB_MINOR_VERSION == 99)
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION + 1, 0))
#elif (XAPIAN_GLIB_MINOR_VERSION % 2)
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION - 1))
#else
#define XAPIAN_GLIB_VERSION_PREV_STABLE        (G_ENCODE_VERSION (XAPIAN_GLIB_MAJOR_VERSION, XAPIAN_GLIB_MINOR_VERSION - 2))
#endif

#ifndef XAPIAN_GLIB_VERSION_MIN_REQUIRED
# define XAPIAN_GLIB_VERSION_MIN_REQUIRED      (XAPIAN_GLIB_VERSION_CUR_STABLE)
#endif

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
#if XAPIAN_GLIB_VERSION_MIN_REQUIRED < XAPIAN_GLIB_VERSION_1_0
#error "XAPIAN_GLIB_VERSION_MIN_REQUIRED must be >= XAPIAN_GLIB_VERSION_1_0"
#endif

/* XXX: Every new stable minor release should add a set of macros here */

#if XAPIAN_GLIB_VERSION_MIN_REQUIRED >= XAPIAN_GLIB_VERSION_1_0
# define XAPIAN_GLIB_DEPRECATED_IN_1_0                XAPIAN_GLIB_DEPRECATED
# define XAPIAN_GLIB_DEPRECATED_IN_1_0_FOR(f)         XAPIAN_GLIB_DEPRECATED_FOR(f)
#else
# define XAPIAN_GLIB_DEPRECATED_IN_1_0                _XAPIAN_GLIB_EXTERN
# define XAPIAN_GLIB_DEPRECATED_IN_1_0_FOR(f)         _XAPIAN_GLIB_EXTERN
#endif

/* unconditional: we can't have an earlier API version */
#define XAPIAN_GLIB_AVAILABLE_IN_1_0                  _XAPIAN_GLIB_EXTERN

#if XAPIAN_GLIB_VERSION_MIN_REQUIRED >= XAPIAN_GLIB_VERSION_1_2
# define XAPIAN_GLIB_DEPRECATED_IN_1_2                XAPIAN_GLIB_DEPRECATED
# define XAPIAN_GLIB_DEPRECATED_IN_1_2_FOR(f)         XAPIAN_GLIB_DEPRECATED_FOR(f)
#else
# define XAPIAN_GLIB_DEPRECATED_IN_1_2                _XAPIAN_GLIB_EXTERN
# define XAPIAN_GLIB_DEPRECATED_IN_1_2_FOR(f)         _XAPIAN_GLIB_EXTERN
#endif

#if XAPIAN_GLIB_VERSION_MAX_ALLOWED < XAPIAN_GLIB_VERSION_1_2
# define XAPIAN_GLIB_AVAILABLE_IN_1_2                 XAPIAN_GLIB_UNAVAILABLE(1, 2)
#else
# define XAPIAN_GLIB_AVAILABLE_IN_1_2                 _XAPIAN_GLIB_EXTERN
#endif

#endif /* __XAPIAN_GLIB_MACROS_H__ */
