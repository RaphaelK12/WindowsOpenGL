#pragma once
#ifndef _wogl_h
#define _wogl_h 1
//#include "stdafx.h"

typedef struct GLContextStruct
{
	HWND hwnd;
	HDC hdc;
	HGLRC hglrc;
	GLuint sx;
	GLuint sy;
	WNDCLASS wc;

} GLContext;

typedef struct KeyMap
{
	ushort lshift : 1;	//1
	ushort rshift : 1;	//2
	ushort lctrl : 1;	//3
	ushort rctrl : 1;	//4
	ushort lalt : 1;	//5
	ushort ralt : 1;	//6
	ushort alt : 1;	    //7
	ushort space : 1;	//8
	ushort win : 1;		//9
	ushort capsl : 1;	//10
	ushort numl : 1;	//11
	ushort scroll : 1;	//12
	ushort up : 1;		//13
	ushort down : 1;	//14

} KeyMap, * pKeyMap;
//int a[sizeof KeyMap];

int startOpengl();
void onRenderScene(void);
void onResize(int x, int y, WPARAM wParam, LPARAM lParam);
void onMove(int x, int y, WPARAM wParam, LPARAM lParam);
void onPaint(PAINTSTRUCT* rc, WPARAM wParam, LPARAM lParam);
void onMouseWhell(int val, WPARAM wParam, LPARAM lParam);
void onMouseMove(int xWindow, int yWindow, WPARAM wParam, LPARAM lParam, int x, int y);
void onMousePress(int x, int y, WPARAM wParam, LPARAM lParam, int up, int n);
void onComand(int cmd, WPARAM wParam, LPARAM lParam);
void onKeyPress2(int key, WPARAM wParam, LPARAM lParam, pKeyMap keymap);
void decodeKeyPress(int key, WPARAM wParam, LPARAM lParam, pKeyMap keymap, UINT Message);
void processKeyPress(pKeyMap keymap);
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);


inline void printAsciiTable() {
	FILE* ASSItable = fopen("ASSItable.py", "wb");
	char c = 0;
	for (char i = 0; i < 16; i++) {
		for (char j = 0; j < 16; j++) {
			fprintf(ASSItable, "%x: ''%c''			", c, c);
			c++;
		}
		fprintf(ASSItable, "\r\n");
	}
	fclose(ASSItable);

}
void cls();



#endif
