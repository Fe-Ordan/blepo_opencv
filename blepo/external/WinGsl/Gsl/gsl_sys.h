/* sys/gsl_sys.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __GSL_SYS_H__
#define __GSL_SYS_H__

#include "..\\WinGslDll.inl"

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

WINGSLDLL_API double gsl_log1p (const double x);
WINGSLDLL_API double gsl_expm1 (const double x);
WINGSLDLL_API double gsl_hypot (const double x, const double y);
WINGSLDLL_API double gsl_acosh (const double x);
WINGSLDLL_API double gsl_asinh (const double x);
WINGSLDLL_API double gsl_atanh (const double x);

WINGSLDLL_API int gsl_isnan (const double x);
WINGSLDLL_API int gsl_isinf (const double x);
WINGSLDLL_API int gsl_finite (const double x);

WINGSLDLL_API double gsl_nan (void);
WINGSLDLL_API double gsl_posinf (void);
WINGSLDLL_API double gsl_neginf (void);
WINGSLDLL_API double gsl_fdiv (const double x, const double y);

WINGSLDLL_API double gsl_coerce_double (const double x);
WINGSLDLL_API float gsl_coerce_float (const float x);
WINGSLDLL_API long double gsl_coerce_long_double (const long double x);

WINGSLDLL_API double gsl_ldexp(const double x, const int e);
WINGSLDLL_API double gsl_frexp(const double x, int * e);

WINGSLDLL_API int gsl_fcmp (const double x1, const double x2, const double epsilon);

__END_DECLS

#endif /* __GSL_SYS_H__ */
