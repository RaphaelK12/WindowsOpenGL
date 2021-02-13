#pragma once
#include "numdef.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>

#ifndef PI
#  define PI 3.14159265358979323846 /* pi */
#endif

#define F_UNIT_EPSILON 0.0002f
#define F_CMP_EPSILON  0.00002f

#define D_UNIT_EPSILON 0.00001
#define D_CMP_EPSILON  0.000001

#define F_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
#define D_EPSILON      2.2204460492503131e-016 // smallest such that 1.0+DBL_EPSILON != 1.0


#define D_CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)
#define F_CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)

#define F_CMP_NORMALIZE_TOLERANCE 0.000001f
#define F_CMP_POINT_IN_PLANE_EPSILON 0.00001f

#define D_CMP_NORMALIZE_TOLERANCE 0.000001
#define D_CMP_POINT_IN_PLANE_EPSILON 0.00001

#define F_PI      3.14159265358979323846f /* pi */
#define F_2PI     6.2831853071795864769252867666f /* 2*pi */
#define F_4PI     12.566370614359172953850573533118f /* 2*pi */
#define F_1_PI    0.318309886183790671538f /* 1/pi */
#define F_PI_2    1.57079632679489661923f /* pi/2 */
#define F_PI_4    0.78539816339744830962f /* pi/4 */
#define F_SQRT2   1.4142135623730950488016887242f /* sqrt(2) */
#define F_SQRT3   1.73205080756887729352f /* sqrt(3) */
#define F_SQRT1_2 0.7071067811865475244008443621048490f /* 1/sqrt(2) */
#define F_SQRT1_3 0.57735026918962576450f /* 1/sqrt(3) */
#define F_E       2.7182818284590452353602874714f /* e euler */
#define F_LOG2E   1.4426950408889634074f /* log_2 e */
#define F_LOG10E  0.43429448190325182765f /* log_10 e */
#define F_LN2     0.6931471805599453094172321215f /* log_e 2 */
#define F_LN10    2.30258509299404568402f /* log_e 10 */

#define D_PI      3.1415926535897932384626433832795 /* pi */
#define D_2PI     6.2831853071795864769252867666 /* 2*pi */
#define D_4PI     12.566370614359172953850573533118 /* 4*pi */
#define D_1_PI    0.318309886183790671538 /* 1/pi */
#define D_PI_2    1.57079632679489661923 /* pi/2 */
#define D_PI_4    0.78539816339744830962 /* pi/4 */
#define D_SQRT2   1.4142135623730950488016887242 /* sqrt(2) */
#define D_SQRT3   1.73205080756887729352 /* sqrt(3) */
#define D_SQRT1_2 0.7071067811865475244008443621048490 /* 1/sqrt(2) */
#define D_SQRT1_3 0.57735026918962576450 /* 1/sqrt(3) */
#define D_E       2.7182818284590452353602874714 /* e euler*/
#define D_LOG2E   1.4426950408889634074 /* log_2 e */
#define D_LOG10E  0.43429448190325182765 /* log_10 e */
#define D_LN2     0.6931471805599453094172321215 /* log_e 2 */
#define D_LN10    2.30258509299404568402 /* log_e 10 */

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

template <class T, class R>
T min(T a, R b) {
	return(a < (T)b ? a : b);
};
template <class T, class R>
T max(T a, R b) {
	return(a > (T)b ? a : b);
};

inline float Turn2Deg (float val) {// 0 ... 360
	return  val * 360;
}
inline float Turn2Grad(float val) {// 0 ... 400
	return  val * 400;
}
inline float Turn2Rad (float val) {// 0 ... 2*F_PI
	return val * F_2PI;
}
inline float Deg2Turn (float val) {// 0 ... 1
	return  val / 360.f;
}
inline float Deg2Grad (float val) {// 0 ... 400
	return  (val / 360.f) * 400;
}
inline float Deg2Rad  (float val) {// 0 ... 2*F_PI
	return  (val / 180.f) * F_PI;
}
inline float Grad2Turn(float val) {// 0 ... 1
	return  val / 400.f;
}
inline float Grad2Deg (float val) {// 0 ... 360
	return  val / 400.f * 360;
}
inline float Grad2Rad (float val) {// 0 ... 2*F_PI
	return val / 200.f * F_PI;
}
inline float Rad2Turn (float val) {// 0 ... 1
	return val / F_2PI;
}
inline float Rad2Deg  (float val) {// 0 ... 360
	return  val / F_PI * 180.f;
}
inline float Rad2Grad (float val) {// 0 ... 400
	return val / F_PI * 200.f;
}

float mix(float a, float b, float factor);
float fact(float x);
float clamp(float v, float a, float b);
float snapped(float p_value, float p_step);
float fposmod(float x, float y);
float saturate(float v);
float smoothstep(float v, float a, float b);
float smootherstep(float v, float a, float b);
bool  is_equal_approx(float a, float b, float eps);

template <typename T>
T bezier3(T a, T b, T c, float t) {
	T e = mix(a, b, t);
	T f = mix(b, c, t);

	return  mix(e, f, t);
}

template <typename T>
T bezier4(T a, T b, T c, T d, float t) {
	T e = mix(a, b, t);
	T f = mix(b, c, t);
	T g = mix(c, d, t);

	T h = mix(e, f, t);
	T i = mix(f, g, t);

	return mix(h, i, t);
}

template <typename T>
T bezier5(T a, T b, T c, T d, T e, float t) {
	T f = mix(a, b, t);
	T g = mix(b, c, t);
	T h = mix(c, d, t);
	T i = mix(d, e, t);

	T j = mix(f, g, t);
	T k = mix(g, h, t);
	T l = mix(h, i, t);

	T m = mix(j, k, t);
	T n = mix(k, l, t);

	return mix(m, n, t);
};










