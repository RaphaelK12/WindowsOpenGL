#include "pch.h"
#include "Utils.h"
#include "cTimer.h"
#pragma once

cTimer::cTimer() :
	fr(0),
	t1(0),
	t2(0),
	f1(0),
	f2(0) {
	//c2=c1=0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&fr);
	QueryPerformanceCounter((LARGE_INTEGER*)&f2);
	t1 = t2 = f1 = f2;
	timers.reserve(2000);
	FPSs.reserve(2000);
	framesMS.reserve(2000);

	timers.push_back(tuple(f1, 0,"cTimer Startup"));
}
cTimer::~cTimer() {};
void  cTimer::clear() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&fr);
	QueryPerformanceCounter((LARGE_INTEGER*)&f2);
	timers.clear();
	framesMS.clear();
	FPSs.clear();
	t1 = t2 = f1 = f2;
	timers.push_back(tuple(f1, 0,"cTimer Restart"));
}

void  cTimer::setTimer(const string& info) {
	t1 = t2;
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
	timers.push_back(tuple(t2, getTimerMS(),  info));
}
void  cTimer::setTimer(string info) {
	t1 = t2;
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
	timers.push_back(tuple(t2, getTimerMS(), info));
}
void  cTimer::setTimer(const char* info) {
	t1 = t2;
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
	timers.push_back(tuple(t2, getTimerMS(), info));
}
float cTimer::getTimerMS() {
	return (t2 - t1) * (1000000.0f / fr) / 1000.0f;
}
float cTimer::getTimerS() {
	return (t2 - t1) * (1000000.0f / fr) / 1000000.0f;
}

void  cTimer::setFrameStart() {
	QueryPerformanceCounter((LARGE_INTEGER*)&f1);
	t1 = t2;
	t2 = f1;
	timers.push_back(tuple(f1, getTimerMS(),"Frame Startup"));
}
void  cTimer::setFrameEnd() {
	QueryPerformanceCounter((LARGE_INTEGER*)&f2);
	t1 = t2;
	t2 = f2;
	timers.push_back(tuple(f2, getFrameMS(),  "__________Frame End_________"));
	framesMS.push_back( getFrameMS());
	FPSs.push_back(getFPS());
}
float cTimer::getFrameMS(){
	LONGLONG t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	return (t - f1) * (1000000.0f / fr) / 1000.0f;
}
float cTimer::getFrameS (){
	LONGLONG t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	return (t - f1) * (1000000.0f / fr) / 1000000.0f;
}
float cTimer::getFPS() {
	QueryPerformanceCounter((LARGE_INTEGER*)&f2);
	//return (f2 - f1) * (1000000.0f / fr) / 1000.0f;
	return 1.0f / (((f2 - f1) * (1000000.0f / fr)) / 1000000.0f);
}
float cTimer::getMedianFPS(uint number) {
	number = min(number, FPSs.size());
	float fps = 0;
	for (uint i = FPSs.size()-1; i > FPSs.size() - number; i--) {
		fps += FPSs[i];
	}
	return fps / number;
}
	
