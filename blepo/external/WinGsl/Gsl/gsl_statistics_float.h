/* statistics/gsl_statistics_float.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Jim Davies, Brian Gough
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

#ifndef __GSL_STATISTICS_FLOAT_H__
#define __GSL_STATISTICS_FLOAT_H__

#include <stddef.h>

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

WINGSLDLL_API double gsl_stats_float_mean (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_variance (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_sd (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_variance_with_fixed_mean (const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_sd_with_fixed_mean (const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_absdev (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_skew (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_kurtosis (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_lag1_autocorrelation (const float data[], const size_t stride, const size_t n);

WINGSLDLL_API double gsl_stats_float_covariance (const float data1[], const size_t stride1,const float data2[], const size_t stride2, const size_t n);

WINGSLDLL_API double gsl_stats_float_variance_m (const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_sd_m (const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_absdev_m (const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_skew_m_sd (const float data[], const size_t stride, const size_t n, const double mean, const double sd);
WINGSLDLL_API double gsl_stats_float_kurtosis_m_sd (const float data[], const size_t stride, const size_t n, const double mean, const double sd);
WINGSLDLL_API double gsl_stats_float_lag1_autocorrelation_m (const float data[], const size_t stride, const size_t n, const double mean);

WINGSLDLL_API double gsl_stats_float_covariance_m (const float data1[], const size_t stride1,const float data2[], const size_t stride2, const size_t n, const double mean1, const double mean2);

/* DEFINED FOR FLOATING POINT TYPES ONLY */

WINGSLDLL_API double gsl_stats_float_wmean (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_wvariance (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_wsd (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_wvariance_with_fixed_mean (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_wsd_with_fixed_mean (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double mean);
WINGSLDLL_API double gsl_stats_float_wabsdev (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_wskew (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);
WINGSLDLL_API double gsl_stats_float_wkurtosis (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n);

WINGSLDLL_API double gsl_stats_float_wvariance_m (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double wmean);
WINGSLDLL_API double gsl_stats_float_wsd_m (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double wmean);
WINGSLDLL_API double gsl_stats_float_wabsdev_m (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double wmean);
WINGSLDLL_API double gsl_stats_float_wskew_m_sd (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double wmean, const double wsd);
WINGSLDLL_API double gsl_stats_float_wkurtosis_m_sd (const float w[], const size_t wstride, const float data[], const size_t stride, const size_t n, const double wmean, const double wsd);

/* END OF FLOATING POINT TYPES */

WINGSLDLL_API double gsl_stats_float_pvariance (const float data1[], const size_t stride1, const size_t n1, const float data2[], const size_t stride2, const size_t n2);
WINGSLDLL_API double gsl_stats_float_ttest (const float data1[], const size_t stride1, const size_t n1, const float data2[], const size_t stride2, const size_t n2);

WINGSLDLL_API float gsl_stats_float_max (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API float gsl_stats_float_min (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API void gsl_stats_float_minmax (float * min, float * max, const float data[], const size_t stride, const size_t n);

WINGSLDLL_API size_t gsl_stats_float_max_index (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API size_t gsl_stats_float_min_index (const float data[], const size_t stride, const size_t n);
WINGSLDLL_API void gsl_stats_float_minmax_index (size_t * min_index, size_t * max_index, const float data[], const size_t stride, const size_t n);

WINGSLDLL_API double gsl_stats_float_median_from_sorted_data (const float sorted_data[], const size_t stride, const size_t n) ;
WINGSLDLL_API double gsl_stats_float_quantile_from_sorted_data (const float sorted_data[], const size_t stride, const size_t n, const double f) ;

__END_DECLS

#endif /* __GSL_STATISTICS_FLOAT_H__ */
