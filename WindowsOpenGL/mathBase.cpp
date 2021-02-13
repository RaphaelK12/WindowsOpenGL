#include "pch.h"
#include "mathBase.h"

bool  is_equal_approx(float a, float b, float eps) {
	float t = abs(a - b);
	return (t < eps);
}

float fact(float x) {
	if (x > 1)
		return fact(x - 1);
	return 1;
}

float mix(float a, float b, float factor) {
	return a * (1 - factor) + b * factor;
}
float clamp(float v, float a, float b) {
	if (v < a)
		return a;
	if (v > b)
		return b;
	return v;
}
float saturate(float v) {
	if (v < 0.f)
		return 0.f;
	if (v > 1.f)
		return 1.f;
	return v;
}

float smoothstep(float v, float a, float b) {
	// Scale, bias and saturate v to 0..1 range
	v = clamp((v - a) / (b - a), 0.0, 1.0);
	// Evaluate polynomial
	return v * v * (3 - 2 * v);
}
float smootherstep(float v, float a, float b) {
	// Scale, and clamp v to 0..1 range
	v = clamp((v - a) / (b - a), 0.0, 1.0);
	// Evaluate polynomial
	return v * v * v * (v * (v * 6 - 15) + 10);
}
float snapped(float p_value, float p_step) {
	if (p_step != 0) {
		p_value = floor(p_value / p_step + 0.5f) * p_step;
	}
	return p_value;
}

float fposmod(float x, float y) {
	return (x >= 0) ? fmod(x, y) : y - fmod(-x, y);
}

uint  halfbits_to_floatbits(ushort h) {
	ushort h_exp, h_sig;
	uint f_sgn, f_exp, f_sig;

	h_exp = (h & 0x7c00u);
	f_sgn = ((uint)h & 0x8000u) << 16;
	switch (h_exp) {
		case 0x0000u: /* 0 or subnormal */
			h_sig = (h & 0x03ffu);
			/* Signed zero */
			if (h_sig == 0) {
				return f_sgn;
			}
			/* Subnormal */
			h_sig <<= 1;
			while ((h_sig & 0x0400u) == 0) {
				h_sig <<= 1;
				h_exp++;
			}
			f_exp = ((uint)(127 - 15 - h_exp)) << 23;
			f_sig = ((uint)(h_sig & 0x03ffu)) << 13;
			return f_sgn + f_exp + f_sig;
		case 0x7c00u: /* inf or NaN */
			/* All-ones exponent and a copy of the significand */
			return f_sgn + 0x7f800000u + (((uint)(h & 0x03ffu)) << 13);
		default: /* normalized */
			/* Just need to adjust the exponent and shift */
			return f_sgn + (((uint)(h & 0x7fffu) + 0x1c000u) << 13);
	}
}

float halfptr_to_float(const ushort* h) {
	union {
		uint u32;
		float f32;
	} u;

	u.u32 = halfbits_to_floatbits(*h);
	return u.f32;
}

float half_to_float(const ushort h) {
	return halfptr_to_float(&h);
}

ushort make_half_float(float f) {
	union {
		float fv;
		uint ui;
	} ci;
	ci.fv = f;

	uint x = ci.ui;
	uint sign = (unsigned short)(x >> 31);
	uint mantissa;
	uint exp;
	ushort hf;

	// get mantissa
	mantissa = x & ((1 << 23) - 1);
	// get exponent bits
	exp = x & (0xFF << 23);
	if (exp >= 0x47800000) {
		// check if the original single precision float number is a NaN
		if (mantissa && (exp == (0xFF << 23))) {
			// we have a single precision NaN
			mantissa = (1 << 23) - 1;
		}
		else {
			// 16-bit half-float representation stores number as Inf
			mantissa = 0;
		}
		hf = (((ushort)sign) << 15) | (ushort)((0x1F << 10)) |
			(ushort)(mantissa >> 13);
	}
	// check if exponent is <= -15
	else if (exp <= 0x38000000) {
		/*// store a denorm half-float value or zero
	exp = (0x38000000 - exp) >> 23;
	mantissa >>= (14 + exp);
	hf = (((uint16_t)sign) << 15) | (uint16_t)(mantissa);
	*/
		hf = 0; //denormals do not work for 3D, convert to zero
	}
	else {
		hf = (((ushort)sign) << 15) |
			(ushort)((exp - 0x38000000) >> 13) |
			(ushort)(mantissa >> 13);
	}

	return hf;
}




