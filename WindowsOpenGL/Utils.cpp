#include "pch.h"
#include "Utils.h"


float3 rotate_around_axis(float3& p, float3& axis, float angle)
{
	float costheta = cosf(angle);
	float sintheta = sinf(angle);
	float3 r;

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

LONGLONG freq = 0, count1 = 0, count2 = 0;
LONGLONG resetTime() {
	LONGLONG tmp = count1 - count2;
	QueryPerformanceCounter((LARGE_INTEGER*)&count1);
	QueryPerformanceCounter((LARGE_INTEGER*)&count2);
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return tmp;
}
float getTime() {
	return float((count2 - count1) * (1000000.0 / freq));
}
float setTime() {
	QueryPerformanceCounter((LARGE_INTEGER*)&count2);
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return float((count2 - count1) * (1000000.0 / freq));
}

LONGLONG resetTime2() {
	LONGLONG tmp = count1 - count2;
	count1 = __rdtsc();
	count2 = count1;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return tmp;
}
float getTime2() {
	return float((count2 - count1) * (1000000.0 / freq));
}
float setTime2() {
	count2 = __rdtsc();
	return float((count2 - count1) * (1000000.0 / freq));
}



frame::frame(): c1(0,0), c2(0,0), p(0,0) {}
frame::frame(vec2 _c1, vec2 _c2, vec2 _p): c1(_c1), c2(_c2), p(_p) {}

frame::~frame() {}
frame::frame(const frame& f) {
	c1 = f.c1;
	c2 = f.c2;
	p  = f.p;
}
frame& frame::operator = (const frame& f)   {
	c1 = f.c1;
	c2 = f.c2;
	p  = f.p;
	return *this;
}
frame& frame::operator = (const frame* f)  {
	c1 = f->c1;
	c2 = f->c2;
	p  = f->p;
	return *this;
}


animation::~animation() {}
animation::animation() {}

animation::animation(std::vector<frame>& f) {
	if (f.size() >= 1) {
		frames.resize(f.size());
		for (uint i = 0; i < f.size(); i++) {
			frames[i] = f[i];
		}
	}
}

animation::animation(animation& a) {
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
}

animation::animation(animation* a) {
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}

}

animation& animation::operator = (animation& a){
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
	return *this;
}

animation& animation::operator = (animation* a){
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}
	return *this;
}

float animation::getFrame(float t){
	int sz = frames.size();
	int vl = sz/2;
	int cur = vl;
	float r = 0;
	frame f1;
	frame f2;
	t = fmodf(t, frames[sz - 1].p.x);
	if (t < 0.f)
		t += frames[sz - 1].p.x;
	if (t > frames[sz - 1].p.x)
		t -= frames[sz - 1].p.x;
	if (sz == 0)
		return 0;
	if ((sz == 1) || (t == 0.f))
		return frames[0].p.y;
	while (vl>0) {
		vl /= 2;
		if (frames[cur].p.x >= t) {
			cur += vl;
		}
		else if (frames[cur].p.x <= t) {
			cur -= vl;
		}
	}
	if (frames[cur].p.x <= t && frames[cur + 1].p.x >= t) {
		f1 = frames[cur];
		f2 = frames[cur + 1];
	}
	else if (frames[cur-1].p.x <= t && frames[cur ].p.x >= t) {
		f1 = frames[cur-1];
		f2 = frames[cur];
	}
	float time = (t-f1.p.x) /(f2.p.x - f1.p.x);
	return fBezier2(f1, f2, time);
}

float fBezier2(frame& f1, frame& f2, float t) {
	vec2 r = bezier4(f1.p, f2.c1, f2.c2, f2.p, t);
	return r.y;
}


bezier2d::~bezier2d() {}
bezier2d::bezier2d() {}

bezier2d::bezier2d(std::vector<bz2d>& f) {
	if (f.size() >= 1) {
		frames.resize(f.size());
		for (uint i = 0; i < f.size(); i++) {
			frames[i] = f[i];
		}
	}
}

bezier2d::bezier2d(bezier2d& a) {
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
}

bezier2d::bezier2d(bezier2d* a) {
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}

}

bezier2d& bezier2d::operator = (bezier2d& a){
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
	return *this;
}

bezier2d& bezier2d::operator = (bezier2d* a){
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}
	return *this;
}

float2 bezier2d::getPos(uint p1, uint p2, float t){
	uint sz = frames.size();
	p1 = p1 % sz;
	if (p1 < 0)
		p1 += sz;
	if (p1 >sz)
		p1 -= sz;
	p2 = p2 % sz;
	if (p2 < 0)
		p2 += sz;
	if (p2 >sz)
		p2 -= sz;
	float2 r = bezier4(frames[p1].p, frames[p2].c1, frames[p2].c2, frames[p2].p, t);
	return r;
}



bezier3d::~bezier3d() {}
bezier3d::bezier3d() {}

bezier3d::bezier3d(std::vector<bz3d>& f) {
	if (f.size() >= 1) {
		frames.resize(f.size());
		for (uint i = 0; i < f.size(); i++) {
			frames[i] = f[i];
		}
	}
}

bezier3d::bezier3d(bezier3d& a) {
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
}

bezier3d::bezier3d(bezier3d* a) {
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}

}

bezier3d& bezier3d::operator = (bezier3d& a){
	if (a.frames.size() >= 1) {
		frames.resize(a.frames.size());
		for (uint i = 0; i < a.frames.size(); i++) {
			frames[i] = a.frames[i];
		}
	}
	return *this;
}

bezier3d& bezier3d::operator = (bezier3d* a){
	if (a->frames.size() >= 1) {
		frames.resize(a->frames.size());
		for (uint i = 0; i < a->frames.size(); i++) {
			frames[i] = a->frames[i];
		}
	}
	return *this;
}

float3 bezier3d::getPos(uint p1, uint p2, float t){
	uint sz = frames.size();
	p1 = p1 % sz;
	if (p1 < 0)
		p1 += sz;
	if (p1 >sz)
		p1 -= sz;
	p2 = p2 % sz;
	if (p2 < 0)
		p2 += sz;
	if (p2 >sz)
		p2 -= sz;
	float3 r = bezier4(frames[p1].p, frames[p2].c1, frames[p2].c2, frames[p2].p, t);
	return r;
}








