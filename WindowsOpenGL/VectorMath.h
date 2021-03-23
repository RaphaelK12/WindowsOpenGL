#pragma once
#include "mathBase.h"
#include "VectorsGLM.h"

//////////////////////////////////////            //////////////////////////////////////
//////////////////////////////////////    vec2    //////////////////////////////////////
//////////////////////////////////////            //////////////////////////////////////


vec2  operator+(float p_scalar, const vec2& p_vec);
vec2  operator-(float p_scalar, const vec2& p_vec);
vec2  operator*(float p_scalar, const vec2& p_vec);
vec2  operator/(float p_scalar, const vec2& p_vec);

vec2  operator+(const vec2& p_vec, float p_scalar);
vec2  operator-(const vec2& p_vec, float p_scalar);
vec2  operator*(const vec2& p_vec, float p_scalar);
vec2  operator/(const vec2& p_vec, float p_scalar);

int   print(const vec2& v);
void from_json(const json& j, vec2& v);
void from_json(const json& j, vec3& v);
void from_json(const json& j, vec4& v);

vec2  mix(const vec2& a, const vec2& b, float factor);
vec2  clamp(const vec2& v, float& a, float& b);
vec2  clamp(const vec2& v, const vec2& a, const vec2& b);
vec2  saturate(const vec2& v);
vec2  smoothstep(const vec2& v, float a, float b);
vec2  smoothstep(const vec2& v, const vec2& a, const vec2& b);
vec2  smootherstep(const vec2& v, float a, float b);
vec2  smootherstep(const vec2& v, const vec2& a, const vec2& b);

vec2  normalize(const vec2& v);
float distance(const vec2& v, const vec2& val);
float dot(const vec2& v, const vec2& val);
float GetAngle(const vec2& v, const vec2& rcVect);
float sqr(const vec2& v);
float length(const vec2& v);
float len_sq(const vec2& v);
float cross(const vec2& v, const vec2& val);

// 0 ~ 1
vec2  rotateTurn(const vec2& v, float val);
// 0 ~ 360
vec2  rotateDeg(const vec2& v, float val);
// 0 ~ 400	
vec2  rotateGrad(const vec2& v, float val);
// 0 ~ 2*F_PI (6.283185)	
vec2  rotateRad(const vec2& v, float val);	

void  fitImage(const vec2& screen, const vec2& im, float zoom, vec2& rcStart, vec2& rcEnd);

float DistanceToLine(const vec2& base, const vec2& rclDirect, const vec2& point);
float pDistance(const vec2& point, const vec2& l1, const vec2& l2);

float magnitude(const vec2& v);

float angle(const vec2& v);
bool  is_normalized(const vec2& v);
float distance_to(const vec2& v, const vec2& p_vec2);
float distance_squared_to(const vec2& v, const vec2& p_vec2);
float angle_to(const vec2& v, const vec2& p_vec2);
float angle_to_point(const vec2& v, const vec2& p_vec2);
float dot(const vec2& v, const vec2& p_other);
float cross(const vec2& v, const vec2& p_other);
vec2  floor(const vec2& v);
vec2  ceil(const vec2& v);
vec2  round(const vec2& v);
vec2  rotated(const vec2& v, float p_by);
vec2  posmod(const vec2& v, float p_mod);
vec2  posmodv(const vec2& v, const vec2& p_modv);
vec2  project(const vec2& v, const vec2& p_b);
vec2  snapped(const vec2& v, const vec2& p_by);
vec2  clamped(const vec2& v, float p_len);
vec2  cubic_interpolate(const vec2& v, const vec2& p_b, const vec2& p_pre_a, const vec2& p_post_b, float p_t);
vec2  move_toward(const vec2& v, const vec2& p_to, float p_delta);
vec2  slide(const vec2& v, const vec2& p_normal);
vec2  reflect(const vec2& v, const vec2& p_normal);
vec2  bounce(const vec2& v, const vec2& p_normal);
bool  is_equal_approx(const vec2& v, const vec2& p_v);



//////////////////////////////////////            //////////////////////////////////////
//////////////////////////////////////    vec3    //////////////////////////////////////
//////////////////////////////////////            //////////////////////////////////////


vec3  operator+(float p_scalar, const vec3& p_vec);
vec3  operator-(float p_scalar, const vec3& p_vec);
vec3  operator*(float p_scalar, const vec3& p_vec);
vec3  operator/(float p_scalar, const vec3& p_vec);

int   print(const vec3& v);

float dot(const vec3& v, const vec3& val);
float sqr(const vec3& v);
float len_sq(const vec3& v);
float length(const vec3& v);
float distance(const vec3& v, const vec3& val);
float magnitude(const vec3& v);
float GetAngle(const vec3& v, const vec3& rcVect);

vec3  mix(const vec3& a, const vec3& b, float factor);
vec3  clamp(const vec3& v, float a, float b);
vec3  clamp(const vec3& v, const vec3& a, const vec3& b);
vec3  saturate(const vec3& v);
vec3  vmod(vec3 v, float m);

vec3  smoothstep(const vec3& v, float a, float b);
vec3  smoothstep(const vec3& v, const vec3& a, const vec3& b);

vec3  smootherstep(const vec3& v, float a, float b);
vec3  smootherstep(const vec3& v, const vec3& a, const vec3& b);

vec3  normalize(const vec3& v);
vec3  cross(const vec3& v, const vec3& val);
vec3  sqrt(const vec3& a);

vec3  reflect(const vec3& I, const vec3& N);
vec3  refract(const vec3& I, const vec3& N, float eta);
vec3  calcNormal(const vec3& v1, const vec3& v2, const vec3& v3);
vec3  faceNormal(const vec3& v1, const vec3& v2, const vec3& v3);
vec3  faceNormal2(const vec3& v1, const vec3& v2, const vec3& v3);
vec3  faceforward(const vec3& N, const vec3& I, const vec3& Nref);
												
// 0 ~ 1						
vec3  rotateXTurn(const vec3& v, float val);	
// 0 ~ 360
vec3  rotateXDeg(const vec3& v, float val);		
// 0 ~ 400
vec3  rotateXGrad(const vec3& v, float val);	
// 0 ~ 2*F_PI (6.283185)
vec3  rotateXRad(const vec3& v, float val);		
												
// 0 ~ 1
vec3  rotateYTurn(const vec3& v, float val);	
// 0 ~ 360
vec3  rotateYDeg(const vec3& v, float val);		
// 0 ~ 400
vec3  rotateYGrad(const vec3& v, float val);	
// 0 ~ 2*F_PI (6.283185)
vec3  rotateYRad(const vec3& v, float val);		
												
// 0 ~ 1	
vec3  rotateZTurn(const vec3& v, float val);	
// 0 ~ 360
vec3  rotateZDeg(const vec3& v, float val);		
// 0 ~ 400
vec3  rotateZGrad(const vec3& v, float val);	
// 0 ~ 2*F_PI (6.283185)
vec3  rotateZRad(const vec3& v, float val);		
													
// 0 ~ 1    
vec3  rotateTurn(const vec3& v, const vec3& val);	
// 0 ~ 360
vec3  rotateDeg(const vec3& v, const vec3& val); 	
// 0 ~ 400
vec3  rotateGrad(const vec3& v, const vec3& val);	
// 0 ~ 2*F_PI (6.283185)
vec3  rotateRad(const vec3& v, const vec3& val); 	

vec3  rotateAroundAxis(const vec3& p, const vec3& axis, float angle);

vec3  rotateAxis(const vec3& v, const vec3& axis, float theta);

float DistanceToLine(const vec3& base, const vec3& rclDirect, const vec3& point);

float distanceToLine(const vec3& P, const vec3& A, const vec3& B);



// Formula de Heron
template <class T>
float area(T A, T B, T C) {
	float a = length(A - B);
	float b = length(A - C);
	float c = length(B - C);
	float p = (a + b + c) / 2;
	float s = sqrt(p * (p - a) * (p - b) * (p - c));
	return s;
};

