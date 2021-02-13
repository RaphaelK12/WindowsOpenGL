#include "pch.h"

#include "VectorMath.h"


//////////////////////////////////////            //////////////////////////////////////
//////////////////////////////////////    vec2    //////////////////////////////////////
//////////////////////////////////////            //////////////////////////////////////

vec2  operator+(float p_scalar, const vec2& p_vec) {
	vec2 tmp;
	tmp.x = p_scalar + p_vec.x;
	tmp.y = p_scalar + p_vec.y;
	return tmp;
}
vec2  operator-(float p_scalar, const vec2& p_vec) {
	vec2 tmp;
	tmp.x = p_scalar - p_vec.x;
	tmp.y = p_scalar - p_vec.y;
	return tmp;
}
vec2  operator*(float p_scalar, const vec2& p_vec) {
	vec2 tmp;
	tmp.x = p_scalar * p_vec.x;
	tmp.y = p_scalar * p_vec.y;
	return tmp;
}
vec2  operator/(float p_scalar, const vec2& p_vec) {
	vec2 tmp;
	tmp.x = p_scalar / p_vec.x;
	tmp.y = p_scalar / p_vec.y;
	return tmp;
}

vec2  operator+(const vec2& p_vec, float p_scalar) {
	vec2 tmp;
	tmp.x = p_scalar + p_vec.x;
	tmp.y = p_scalar + p_vec.y;
	return tmp;
}
vec2  operator-(const vec2& p_vec, float p_scalar) {
	vec2 tmp;
	tmp.x = p_scalar - p_vec.x;
	tmp.y = p_scalar - p_vec.y;
	return tmp;
}
vec2  operator*(const vec2& p_vec, float p_scalar) {
	vec2 tmp;
	tmp.x = p_scalar * p_vec.x;
	tmp.y = p_scalar * p_vec.y;
	return tmp;
}
vec2  operator/(const vec2& p_vec, float p_scalar) {
	vec2 tmp;
	tmp.x = p_scalar / p_vec.x;
	tmp.y = p_scalar / p_vec.y;
	return tmp;
}

int   print(const vec2& v) {
	return printf("x = %f,	y = %f,	len = %f", v.x, v.y, length(v));
}

vec2  mix(const vec2& a, const vec2& b, float factor) {
	return a * (1 - factor) + b * factor;
}
vec2  clamp(const vec2& v, float& a, float& b) {
	vec2 tmp;
	tmp.x = clamp(v.x, a, b);
	tmp.y = clamp(v.y, a, b);
	return tmp;
}
vec2  clamp(const vec2& v, const vec2& a, const  vec2& b) {
	vec2 tmp;
	tmp.x = clamp(v.x, a.x, b.x);
	tmp.y = clamp(v.y, a.y, b.y);
	return tmp;
}
vec2  saturate(const vec2& v) {
	vec2 tmp;
	tmp.x = saturate(v.x);
	tmp.y = saturate(v.y);
	return tmp;
}
vec2  smoothstep(const vec2& v, float a, float b) {
	vec2 tmp;
	tmp.x = smoothstep(v.x, a, b);
	tmp.y = smoothstep(v.y, a, b);
	return tmp;
}
vec2  smoothstep(const vec2& v, const vec2& a, const  vec2& b) {
	vec2 tmp;
	tmp.x = smoothstep(v.x, a.x, b.x);
	tmp.y = smoothstep(v.y, a.y, b.y);
	return tmp;
}
vec2  smootherstep(const vec2& v, float a, float b) {
	vec2 tmp;
	tmp.x = smootherstep(v.x, a, b);
	tmp.y = smootherstep(v.y, a, b);
	return tmp;
}
vec2  smootherstep(const vec2& v, const vec2& a, const vec2& b) {
	vec2 tmp;
	tmp.x = smootherstep(v.x, a.x, b.x);
	tmp.y = smootherstep(v.y, a.y, b.y);
	return tmp;
}

vec2  normalize(const vec2& v) {
	float l = v.x * v.x + v.y * v.y;
	vec2 tmp(v);
	if (l != 0 && l != 1.f) {
		l = sqrt(l);
		tmp.x /= l;
		tmp.y /= l;
	}
	return tmp;
}
float distance(const vec2& v, const vec2& val) {
	return abs(length(v - val));
}
float dot(const vec2& v, const vec2& val) {
	return (v.x * val.x) + (v.y * val.y);
}
float GetAngle(const vec2& v, const vec2& rcVect) {
	float divid, fNum;

	divid = length(v) * length(rcVect);

	if ((divid < -1e-10f) || (divid > 1e-10f)) {
		fNum = dot(v, rcVect) / divid;
		if (fNum < -1)
			return F_PI;
		else if (fNum > 1)
			return 0.0F;
		else
			return acos(fNum);
	}
	else
		return 1.f; // division by zero
}
float sqr(const vec2& v) {
	return ((v.x * v.x) + (v.y * v.y));
}
float length(const vec2& v) {
	float l = (v.x * v.x) + (v.y * v.y);
	if (l != 0)
		l = sqrt(l);
	return l;
}
float len_sq(const vec2& v) {
	return (v.x * v.x) + (v.y * v.y);
}
float cross(const vec2& v, const vec2& val) {
	return (v.x * val.y) - (v.y * val.x);
}

vec2 rotateTurn(const vec2& v, float val) {
	val = val * 2 * F_PI;
	float fsin, fcos;
	vec2 tmp;
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec2 rotateDeg(const vec2& v, float val) {
	val = val / 180.f * F_PI;
	float fsin, fcos;
	vec2 tmp;
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec2 rotateGrad(const vec2& v, float val) {
	val = val / 200.f * F_PI;
	float fsin, fcos;
	vec2 tmp;
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec2 rotateRad(const vec2& v, float val) {
	float fsin, fcos;
	vec2 tmp;
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}

void  fitImage(const vec2& screen, const vec2& im, float zoom, vec2& rcStart, vec2& rcEnd) {
	//vec2 rc;
	float w1 = 1, w2 = 1;
	float x = 1, y = 1, scx = 1, scy = 1;
	float x_start = 2, y_start = 2, x_end = 4, y_end = 4;
	float bmx = im.x * zoom;
	float bmy = im.y * zoom;

	{ // if screnn < image then rescale image to fit in window
		scx = screen.x - 4;
		scy = screen.y - 4;
		x = bmx;
		y = bmy;

		if (scx < x) w1 = scx / x;
		if (scy < y) w2 = scy / y;
		x = (bmx)*min(w1, w2);
		y = (bmy)*min(w1, w2);
		x_end = floorf(x + 0.5f);
		y_end = floorf(y + 0.5f);

		x_start = floorf((screen.x / 2.f - x / 2.f) + 0.5f);
		y_start = floorf((screen.y / 2.f - y / 2.f) + 0.5f);
		x_start = max(x_start, 2.f);
		y_start = max(y_start, 2.f);
	};
	{ // rescale to window size, maintaining aspect
		scx = screen.x - 4;
		scy = screen.y - 4;
		x = bmx;
		y = bmy;

		/*if (scx < x)*/ w1 = scx / x;
		/*if (scy < y)*/ w2 = scy / y;
		x = (bmx)*min(w1, w2);
		y = (bmy)*min(w1, w2);
		x_end = floorf(x + 0.5f);
		y_end = floorf(y + 0.5f);
		x_start = floorf((screen.x / 2.f - x / 2.f) + 0.5f);
		y_start = floorf((screen.y / 2.f - y / 2.f) + 0.5f);
		x_start = max(x_start, 2.f);
		y_start = max(y_start, 2.f);
	};


	rcStart.x = x_start;
	rcStart.y = y_start;
	rcEnd.x = x_end;
	rcEnd.y = y_end;
}

float DistanceToLine(const vec2& base, const vec2& rclDirect, const vec2& point) {
	return fabs((cross(rclDirect, base - point))) / length(rclDirect);
}
float pDistance(const vec2& point, const vec2& l1, const vec2& l2) {

	float  A = point.x - l1.x;
	float  B = point.y - l1.y;
	float  C = l2.x - l1.x;
	float  D = l2.y - l1.y;

	float  dot = A * C + B * D;
	float  len_sq = C * C + D * D;
	float  param = -1;
	if (len_sq != 0) //in case of 0 length line
		param = dot / len_sq;

	float  xx, yy;

	if (param < 0) {
		xx = l1.x;
		yy = l1.y;
	}
	else if (param > 1) {
		xx = l2.x;
		yy = l2.y;
	}
	else {
		xx = l1.x + param * C;
		yy = l1.y + param * D;
	}

	float  dx = point.x - xx;
	float  dy = point.y - yy;
	return sqrt(dx * dx + dy * dy);
}

float magnitude(const vec2& v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

float angle(const vec2& v) {
	return atan2(v.y, v.x);
}
bool  is_normalized(const vec2& v) {
	// use length_squared() instead of length() to avoid sqrt(), makes it more stringent.
	return is_equal_approx(len_sq(v), 1.0, F_EPSILON);
}
float distance_to(const vec2& v, const vec2& p_vec2) {
	return sqrt((v.x - p_vec2.x) * (v.x - p_vec2.x) + (v.y - p_vec2.y) * (v.y - p_vec2.y));
}
float distance_squared_to(const vec2& v, const vec2& p_vec2) {
	return (v.x - p_vec2.x) * (v.x - p_vec2.x) + (v.y - p_vec2.y) * (v.y - p_vec2.y);
}
float angle_to(const vec2& v, const vec2& p_vec2) {
	return atan2(cross(v, p_vec2), dot(v, p_vec2));
}
float angle_to_point(vec2& v, const vec2& p_vec2) {
	return atan2(v.y - p_vec2.y, v.x - p_vec2.x);
}
vec2  floor(const vec2& v) {
	return vec2(floor(v.x), floor(v.y));
}
vec2  ceil(const vec2& v) {
	return vec2(ceil(v.x), ceil(v.y));
}
vec2  round(const vec2& v) {
	return vec2(round(v.x), round(v.y));
}
vec2  rotated(const vec2& v, float p_by) {
	float sine = sin(p_by);
	float cosi = cos(p_by);
	return vec2(
		v.x * cosi - v.y * sine,
		v.x * sine + v.y * cosi);
}
vec2  posmod(const vec2& v, float p_mod) {
	return vec2(fposmod(v.x, p_mod), fposmod(v.y, p_mod));
}
vec2  posmodv(const vec2& v, const vec2& p_modv) {
	return vec2(fposmod(v.x, p_modv.x), fposmod(v.y, p_modv.y));
}
vec2  project(const vec2& v, const vec2& p_b) {
	return p_b * (dot(v, p_b) / len_sq(p_b));
}
vec2  snapped(const vec2& v, const vec2& p_by) {
	return vec2(
		snapped(v.x, p_by.x),
		snapped(v.y, p_by.y));
}
vec2  clamped(const vec2& v, float p_len) {
	float l = length(v);
	vec2 ve = v;
	if (l > 0 && p_len < l) {
		ve /= l;
		ve *= p_len;
	}

	return v;
}
vec2  cubic_interpolate(const vec2& v, const vec2& p_b, const vec2& p_pre_a, const vec2& p_post_b, float p_t) {
	vec2 p0 = p_pre_a;
	vec2 p1 = v;
	vec2 p2 = p_b;
	vec2 p3 = p_post_b;

	float t = p_t;
	float t2 = t * t;
	float t3 = t2 * t;

	vec2 out;
	out = 0.5 * ((p1 * 2.0) +
				 (-p0 + p2) * t +
				 (2.0 * p0 - 5.0 * p1 + 4 * p2 - p3) * t2 +
				 (-p0 + 3.0 * p1 - 3.0 * p2 + p3) * t3);
	return out;
}
vec2  move_toward(const vec2& v, const vec2& p_to, float p_delta) {
	vec2 vd = p_to - v;
	float len = length(vd);
	return len <= p_delta || len < F_CMP_EPSILON ? p_to : v + vd / len * p_delta;
}
vec2  slide(const vec2& v, const vec2& p_normal) {// slide returns the component of the vector along the given plane, specified by its normal vector.
#ifdef MATH_CHECKS
	ERR_FAIL_COND_V_MSG(!p_normal.is_normalized(), vec2(), "The normal vec2 must be normalized.");
#endif
	return v - p_normal * dot(v, p_normal);
}
vec2  reflect(const vec2& v, const vec2& p_normal) {
#ifdef MATH_CHECKS
	ERR_FAIL_COND_V_MSG(!p_normal.is_normalized(), vec2(), "The normal vec2 must be normalized.");
#endif
	return 2.0 * p_normal * dot(v, p_normal) - v;
}
vec2  bounce(const vec2& v, const vec2& p_normal) {
	return -reflect(v, p_normal);
}
bool  is_equal_approx(const vec2& v, const vec2& p_v) {
	return is_equal_approx(v.x, p_v.x, F_CMP_EPSILON) && is_equal_approx(v.y, p_v.y, F_CMP_EPSILON);
}



//////////////////////////////////////            //////////////////////////////////////
//////////////////////////////////////    vec3    //////////////////////////////////////
//////////////////////////////////////            //////////////////////////////////////

vec3  operator+(float p_scalar, const vec3& p_vec) {
	vec3 tmp;
	tmp.x = p_scalar + p_vec.x;
	tmp.y = p_scalar + p_vec.y;
	tmp.z = p_scalar + p_vec.z;
	return tmp;
}
vec3  operator-(float p_scalar, const vec3& p_vec) {
	vec3 tmp;
	tmp.x = p_scalar - p_vec.x;
	tmp.y = p_scalar - p_vec.y;
	tmp.z = p_scalar - p_vec.z;
	return tmp;
}
vec3  operator*(float p_scalar, const vec3& p_vec) {
	vec3 tmp;
	tmp.x = p_scalar * p_vec.x;
	tmp.y = p_scalar * p_vec.y;
	tmp.z = p_scalar * p_vec.z;
	return tmp;
}
vec3  operator/(float p_scalar, const vec3& p_vec) {
	vec3 tmp;
	tmp.x = p_scalar / p_vec.x;
	tmp.y = p_scalar / p_vec.y;
	tmp.z = p_scalar / p_vec.z;
	return tmp;
}

int   print(const vec3& v) {
	return printf("x = %f,	y = %f,	z = %f,	len = %f\n", v.x, v.y, v.z, length(v));
}

float length(const vec3& v) {
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
float len_sq(const vec3& v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}
float distance(const vec3& v, const vec3& val) {
	return abs(length(v - val));
}
float dot(const vec3& v, const vec3& val) {
	return (v.x * val.x) + (v.y * val.y) + (v.z * val.z);
}
float sqr(const vec3& v) {
	return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
float GetAngle(const vec3& v, const vec3& rcVect) {
	float divid, fNum;

	divid = length(v) * length(rcVect);

	if ((divid < -1e-10f) || (divid > 1e-10f)) {
		fNum = dot(v, rcVect) / divid;
		if (fNum < -1)
			return F_PI;
		else if (fNum > 1)
			return 0.0F;
		else
			return acos(fNum);
	}
	else
		return 1.f; // division by zero
}
float magnitude(const vec3& v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

vec3  mix(const vec3& a, const vec3& b, float factor) {
	return a * (1 - factor) + b * factor;
}
vec3  clamp(const vec3& v, float a, float b) {
	vec3 tmp;
	tmp.x = clamp(v.x, a, b);
	tmp.y = clamp(v.y, a, b);
	tmp.z = clamp(v.z, a, b);
	return tmp;
}
vec3  clamp(const vec3& v, const vec3& a, const vec3& b) {
	vec3 tmp;
	tmp.x = clamp(v.x, a.x, b.x);
	tmp.y = clamp(v.y, a.y, b.y);
	tmp.z = clamp(v.z, a.z, b.z);
	return tmp;
}
vec3  saturate(const vec3& v) {
	vec3 tmp;
	tmp.x = saturate(v.x);
	tmp.y = saturate(v.y);
	tmp.z = saturate(v.z);
	return tmp;
}

vec3  smoothstep(const vec3& v, float a, float b) {
	vec3 tmp;
	tmp.x = smoothstep(v.x, a, b);
	tmp.y = smoothstep(v.y, a, b);
	tmp.z = smoothstep(v.z, a, b);
	return tmp;
}
vec3  smoothstep(const vec3& v, const vec3& a, const vec3& b) {
	vec3 tmp;
	tmp.x = smoothstep(v.x, a.x, b.x);
	tmp.y = smoothstep(v.y, a.y, b.y);
	tmp.z = smoothstep(v.z, a.z, b.z);
	return tmp;
}
vec3  smootherstep(const vec3& v, float a, float b) {
	vec3 tmp;
	tmp.x = smootherstep(v.x, a, b);
	tmp.y = smootherstep(v.y, a, b);
	tmp.z = smootherstep(v.z, a, b);
	return tmp;
}
vec3  smootherstep(const vec3& v, const vec3& a, const vec3& b) {
	vec3 tmp;
	tmp.x = smootherstep(v.x, a.x, b.x);
	tmp.y = smootherstep(v.y, a.y, b.y);
	tmp.z = smootherstep(v.z, a.z, b.z);
	return tmp;
}

vec3  normalize(const vec3& v) {
	vec3 tmp(v);
	float fLen = length(v);
	if (fLen != 0.f && fLen != 1.f) {
		tmp.x /= fLen;
		tmp.y /= fLen;
		tmp.z /= fLen;
	}
	return tmp;
}
vec3  cross(const vec3& v, const vec3& val) {
	vec3 tmp;
	tmp.x = (v.y * val.z) - (v.z * val.y);
	tmp.y = (v.z * val.x) - (v.x * val.z);
	tmp.z = (v.x * val.y) - (v.y * val.x);
	return tmp;
}
vec3  sqrt(const vec3& a) {
	return vec3(sqrt(a.x), sqrt(a.y), sqrt(a.z));
}
vec3  faceforward(const vec3& N, const vec3& I, const vec3& Nref) {
	if (dot(Nref, I) < 0.0f) {
		return N;
	}
	return -N;
}
vec3  reflect(const vec3& I, const vec3& N) {
	return I - N * dot(N, I) * 2.0f;
}
vec3  refract(const vec3& I, const vec3& N, float eta) {
	float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
	if (k < 0.0f)
		return vec3(0.0f);
	else
		return I * eta - (N * sqrt(k)) + eta * dot(N, I);
}
vec3  calcNormal(const vec3& v1, const vec3& v2, const vec3& v3) {
	vec3 n1, n2;
	n1 = v2 - v1;
	n2 = v2 - v3;
	return cross(n1, n2);
}
vec3  faceNormal(const vec3& v1, const vec3& v2, const vec3& v3) {
	vec3 n1, n2;
	// Calculate two vectors from the three points
	n1 = v1 - v2;
	n2 = v2 - v3;
	// Take the cross product of the two vectors to get
	// the normal vector and
	// Normalize the vector (shorten length to one)
	return normalize(cross(n1, n2));
}

vec3  rotateXTurn(const vec3& v, float val) { // 0 ~ 1
	val = val * 2 * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.y = (v.y * fcos) - (v.z * fsin);
	tmp.z = (v.y * fsin) + (v.z * fcos);
	return tmp;
}
vec3  rotateXDeg(const vec3& v, float val) { // 0 ~ 360
	val = val / 180.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.y = (v.y * fcos) - (v.z * fsin);
	tmp.z = (v.y * fsin) + (v.z * fcos);
	return tmp;
}
vec3  rotateXGrad(const vec3& v, float val) { // 0 ~ 400
	val = val / 200.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.y = (v.y * fcos) - (v.z * fsin);
	tmp.z = (v.y * fsin) + (v.z * fcos);
	return tmp;
}
vec3  rotateXRad(const vec3& v, float val) { // 0 ~ 2*F_PI (6.283185)
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.y = (v.y * fcos) - (v.z * fsin);
	tmp.z = (v.y * fsin) + (v.z * fcos);
	return tmp;
}

vec3  rotateYTurn(const vec3& v, float val) {
	val = val * 2 * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.z * fsin) + (v.x * fcos);
	tmp.z = (v.z * fcos) - (v.x * fsin);
	return tmp;
}
vec3  rotateYDeg(const vec3& v, float val) {
	val = val / 180.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.z * fsin) + (v.x * fcos);
	tmp.z = (v.z * fcos) - (v.x * fsin);
	return tmp;
}
vec3  rotateYGrad(const vec3& v, float val) {
	val = val / 200.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.z * fsin) + (v.x * fcos);
	tmp.z = (v.z * fcos) - (v.x * fsin);
	return tmp;
}
vec3  rotateYRad(const vec3& v, float val) {
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.z * fsin) + (v.x * fcos);
	tmp.z = (v.z * fcos) - (v.x * fsin);
	return tmp;
}

vec3  rotateZTurn(const vec3& v, float val) {
	val = val * 2 * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec3  rotateZDeg(const vec3& v, float val) {
	val = val / 180.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec3  rotateZGrad(const vec3& v, float val) {
	val = val / 200.f * F_PI;
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}
vec3  rotateZRad(const vec3& v, float val) {
	float fsin, fcos;
	vec3 tmp(v);
	fsin = sin(val);
	fcos = cos(val);
	tmp.x = (v.x * fcos) - (v.y * fsin);
	tmp.y = (v.x * fsin) + (v.y * fcos);
	return tmp;
}

vec3  rotateTurn(const vec3& v, const vec3& val) {
	vec3 tmp(v);
	tmp = rotateZTurn(v, val.z);
	tmp = rotateYTurn(tmp, val.y);
	tmp = rotateXTurn(tmp, val.x);
	return tmp;
}
vec3  rotateDeg(const vec3& v, const vec3& val) {
	vec3 tmp(v);
	tmp = rotateZDeg(v, val.z);
	tmp = rotateYDeg(tmp, val.y);
	tmp = rotateXDeg(tmp, val.x);
	return tmp;
}
vec3  rotateGrad(const vec3& v, const vec3& val) {
	vec3 tmp(v);
	tmp = rotateZGrad(v, val.z);
	tmp = rotateYGrad(tmp, val.y);
	tmp = rotateXGrad(tmp, val.x);
	return tmp;
}
vec3  rotateRad(const vec3& v, const vec3& val) {
	vec3 tmp(v);
	tmp = rotateZRad(v, val.z);
	tmp = rotateYRad(tmp, val.y);
	tmp = rotateXRad(tmp, val.x);
	return tmp;
}

vec3  rotateAroundAxis(const vec3& p, const vec3& axis, float angle) {
	float costheta = cosf(angle);
	float sintheta = sinf(angle);
	vec3 r;

	r.x = ((costheta + (1 - costheta) * axis.x * axis.x) * p.x) +
		(((1 - costheta) * axis.x * axis.y - axis.z * sintheta) * p.y) +
		(((1 - costheta) * axis.x * axis.z + axis.y * sintheta) * p.z);

	r.y = (((1 - costheta) * axis.x * axis.y + axis.z * sintheta) * p.x) +
		((costheta + (1 - costheta) * axis.y * axis.y) * p.y) +
		(((1 - costheta) * axis.y * axis.z - axis.x * sintheta) * p.z);

	r.z = (((1 - costheta) * axis.x * axis.z - axis.y * sintheta) * p.x) +
		(((1 - costheta) * axis.y * axis.z + axis.x * sintheta) * p.y) +
		((costheta + (1 - costheta) * axis.z * axis.z) * p.z);

	return r;
}

vec3  rotateAxis(const vec3& v, const vec3& axis, float theta) {
	float cos_theta = cos(theta);
	float sin_theta = sin(theta);
	return (v * cos_theta) + (cross(axis, v) * sin_theta) + (axis * dot(axis, v)) * (1 - cos_theta);
}

float DistanceToLine(const vec3& base, const vec3& rclDirect, const vec3& point) {
	return fabs(length(cross(rclDirect, base - point))) / length(rclDirect);
}

float distanceToLine(const vec3& P, const vec3& A, const vec3& B) {
	vec3 v1 = B - A, v2 = P - A;
	return length(cross(v1, v2)) / length(v1);
}



