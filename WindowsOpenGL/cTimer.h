#pragma once
typedef std::tuple< LONGLONG, float, string> tuple;
class cTimer {
public:
	vector<tuple> timers;
	vector<float> FPSs;
	vector<float> framesMS;
	LONGLONG fr; // processador frequencia
	LONGLONG t1; // timer1
	LONGLONG t2; // timer2
	LONGLONG f1; // counter1
	LONGLONG f2; // counter2
	cTimer();
	~cTimer();
	void   clear();

	void   setTimer(const string &info);
	void   setTimer(string info);
	void   setTimer(const char* info);
	float  getTimerMS();
	float  getTimerS();

	void   setFrameStart();
	void   setFrameEnd();
	float  getFrameMS();
	float  getFrameS();
	float  getFPS();
	float  getMedianFPS(uint number);
};
