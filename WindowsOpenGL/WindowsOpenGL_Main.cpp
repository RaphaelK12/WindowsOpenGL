#pragma warning(disable:4018)
#include "pch.h"
#include "WindowsOpengl.h"
#include "resource.h"
#include "manager.h"
#include "Utils.h"

GLContextStruct ctx;

int bQuit = false;
float theta = 0;
char limit = 1;


pKeyMap keymap = new KeyMap;

animation anim;

Word word;

objeto* esfera;
objeto* torus;
objeto* cube;
objeto* grid;
objeto* axis;
objeto* plane;
texto* txt;
GLuint hdrTextures[10];
cTimer t;

frameBuffer* fbo;


char szTitle[128] = "Test Opengl 01";


// Main function startup program
int main(int argc, char* argv[]) {
	t.setTimer("main");
	FILE* f = fopen("XING_T32.TGA", "rb");
	if (!f) f = fopen("../XING_T32.TGA", "rb");
	if (!f) f = fopen("../../XING_T32.TGA", "rb");
	//if(!f) f = fopen("../../concrete_diffuse.tga", "rb");
	img_basis* tx = 0;
	t.setTimer("open XING_T32.TGA");
	tx = read_TGA(f);
		assert(tx);
	if (!tx)
		return 0;
	t.setTimer("read_TGA");
	if (!tx->isOPenGLCompatible())
		tx->convertoToOPenGLCompatible();
	t.setTimer("convertoToOPenGLCompatible");
	startOpengl();
	t.setTimer("startOpengl");
	//glEnable(GL_MULTISAMPLE);
	glGenTextures(1, hdrTextures);

	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, hdrTextures[0]);
	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB16F, ctx.sx, ctx.sy, GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, hdrTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, tx->glInternalFormat, tx->xres, tx->yres, GL_FALSE, tx->glFormat, tx->glType, tx->pixels);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	float borderColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLfloat value, max_anisotropy = 16.0f; /* don't exceed this value...*/

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);

	value = (value > max_anisotropy) ? max_anisotropy : value;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);

	glGenerateMipmap(GL_TEXTURE_2D);
	t.setTimer("Create texture");

	anim.frames.push_back(frame(vec2(0), vec2(0), vec2(0)));
	anim.frames.push_back(frame(vec2(0.5, 0), vec2(0.501, 1), vec2(1, 1)));
	anim.frames.push_back(frame(vec2(1.5, 1), vec2(1.6, 0), vec2(2, 1)));

	//word.addObj(objType::objCone2, float3(0, 0, 0), float3(0, 0, 0), float3(1), uivec3(50, 50, 10), "default.mat");
	//word.addObj(objType::objEsfera2, float3(0, 2, 0), float3(0, 0, 0), float3(1), uivec3(50, 50, 10), "default.mat");
	//word.addObj(objType::objBox, float3(0, -2, 0), float3(0, 0, 0), float3(1), uivec3(10, 10, 10), "default.mat");
	//word.addObj(objType::objCilindro2, float3(0, 4, 0), float3(0, 0, 0), float3(1), uivec3(60, 10, 10), "default.mat");
	//word.addObj(objType::objTorus2, float3(0, -4, 0), float3(0, 0, 0), float3(1), uivec3(60, 60, 10), "default.mat");
	vec3 pos(0, 4, 0);
	for (int i = 0; i < 10; i ++) {
		pos = rotateZDeg(pos, 45);
		word.addObj(objType::objEsfera2, pos, vec3(0, 0, 0), vec3(1), uivec3(60, 60, 10), "default.mat");
	}


	//esfera->atach();
	//torus->atach();
	//cube->atach();
	//grid->atach();
	//axis->atach();

	word.addObj(objType::objBox, float3(5, 0, 0), float3(0, 0, 0), float3(0.1, 0.1, 0.1), uivec3(10, 10, 10), "default.mat");
	word.addObj(objType::objGrid, float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1), uivec3(15, 15, 15),"lineVertexColor.mat");
	word.addObj(objType::objAxis, float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1), uivec3(10, 10, 10), "lineVertexColor.mat");
	plane = new objeto(0, objType::objQuad, float3(0, 0, 0), float3(0, 0, 0), float3(1,1,1), uivec3(50, 50, 10), "screen.mat");
	plane->atach();



	//torus->draw();
	//grid->draw();
	//axis->draw();	
	//axis->malhas[0]->mMaterial->mShader->FS = "VertexColor.fs";
	//axis->malhas[0]->mMaterial->mShader->readSrcFromFilenames();
	//axis->malhas[0]->mMaterial->mShader->generateShadersAndProgram(1);
	//axis->malhas[0]->mMaterial->mShader->use();
	//axis->draw();

	//esfera->draw();
	//esfera->malhas[0]->mMaterial->mShader->VS = "screen.vs";
	//esfera->malhas[0]->mMaterial->mShader->readSrcFromFilenames();
	//esfera->malhas[0]->mMaterial->mShader->generateShadersAndProgram(1);
	//esfera->malhas[0]->mMaterial->mShader->use();

	txt = new texto("abcdefghijklmnopqrstuvwxyz ,.;/\\[]{}´`=+-_()!¹²³£¢¬@#$%¨&*'\"+-*/asdasdkhj	bfksdhgfgh	diasghfh hgfhjksdghfhj \ndhgadesfghadshfshg\nfsdfsd	asd\ndfsdfdsgdsgsdg)");
	t.setTimer("create objs");

	ShowWindow(ctx.hwnd, 1);
	MSG msg;
	t.setTimer("ShowWindow");

	while (!bQuit) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT) {
				bQuit = TRUE;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			t.setTimer("PeekMessage");
		}
		else {
			t.setTimer("wasted");
			ZeroMemory(keymap, sizeof(KeyMap));
			t.setTimer("ZeroMemory");
			processKeyPress(keymap);
			t.setTimer("processKeyPress");
			onRenderScene();
			t.setTimer("onRenderScene");
			glFinish();
			t.setTimer("glFinish");
			SwapBuffers(ctx.hdc);
			t.setTimer("SwapBuffers");
			t.setFrameEnd();
			float fps = t.getMedianFPS(60);
			sprintf(szTitle, "Test Opengl 01 - FPS: %.2f", fps);
			SetWindowTextA(ctx.hwnd, szTitle);
			if (limit) {
				float ms = 0;
				while ((ms=t.getFrameMS()) <= 10.0f)
					Sleep(0);
			}
			t.setTimer("sleeped");
			t.setFrameStart();
			t.setTimer("setFrameStart");
		}
	}
	delete keymap;
	wglMakeCurrent(ctx.hdc, NULL);
	wglDeleteContext(ctx.hglrc);
	ReleaseDC(ctx.hwnd, ctx.hdc);
	DestroyWindow(ctx.hwnd);

	//system("pause");
	//return msg.wParam;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static int cxClient, cyClient, lcxClient, lcyClient, iDeltaPerLine = 120, iAccumDelta;
	ULONG ulScrollLines = 1;
	static int xres, yres;
	static int xWindow=0, yWindow=0;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	ZeroMemory(keymap, sizeof(KeyMap));

	switch (Message) {
		case WM_CREATE:
		{
			printf("WM_CREATE\n");
			break;
		}
		case WM_NULL:
		{
			printf("WM_NULL\n");
		}
		case WM_SIZE:
		{
			GetWindowRect(hwnd, &rc);
			onResize(LOWORD(lParam), HIWORD(lParam), wParam, lParam);
			break;
		}
		case WM_MOVE:
		{
			onMove(LOWORD(lParam), HIWORD(lParam), wParam, lParam);
			break;
		}
		case WM_MOUSEMOVE:
		{
			GetAsyncKeyState(VK_LBUTTON);
			GetAsyncKeyState(VK_MBUTTON);
			onMouseMove(LOWORD(lParam), HIWORD(lParam), wParam, lParam, LOWORD(lParam)- xWindow, HIWORD(lParam)- yWindow);
			xWindow = LOWORD(lParam), yWindow = HIWORD(lParam);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, false, 1);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, false, 2);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, false, 3);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, true, 1);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, true, 2);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			onMousePress(LOWORD(lParam), HIWORD(lParam), lParam, lParam, true, 3);
			return 0;
		}
		//case WM_KEYDOWN:
		//	if ((GetAsyncKeyState(VK_ESCAPE) & 0x01) && bRunning) {
		//		Stop();
		//	}
		//	break;
		//}
		case WM_KEYDOWN:
		{
			return 0;
			printf("WM_KEYDOWN\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		case WM_KEYUP:
		{
			return 0;
			printf("WM_KEYUP\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		//case WM_CHAR:
		//{
		//printf("WM_CHAR\n");
		//	pKeyMap keymap = new KeyMap;
		//	decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		//	delete keymap;
		//	return 0;
		//}
		//case WM_DEADCHAR:
		//{
		//printf("WM_DEADCHAR\n");
		//	pKeyMap keymap = new KeyMap;
		//	decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		//	delete keymap;
		//	return 0;
		//}
		case WM_SYSKEYDOWN:
		{
			return 0;
			printf("WM_SYSKEYDOWN\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		case WM_SYSKEYUP:
		{
			return 0;
			printf("WM_SYSKEYUP\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		case WM_SYSCHAR:
		{
			return 0;
			printf("WM_SYSCHAR\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		case WM_SYSDEADCHAR:
		{
			return 0;
			printf("WM_SYSDEADCHAR\n");
			decodeKeyPress(wParam, wParam, lParam, keymap, Message);
		}
		case WM_MOUSEWHEEL:
		{
			if (iDeltaPerLine == 0)
				break;
			iAccumDelta += (short)HIWORD(wParam); // 120 or -120
			while (iAccumDelta >= iDeltaPerLine) {
				onMouseWhell(1, wParam, lParam);
				iAccumDelta -= iDeltaPerLine;
			}
			while (iAccumDelta <= -iDeltaPerLine) {
				onMouseWhell(-1, wParam, lParam);
				iAccumDelta += iDeltaPerLine;
			}
			break;
		}
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			onPaint(&ps, wParam, lParam);
			EndPaint(hwnd, &ps);
			SwapBuffers(ctx.hdc);
			return 0;
		}
		case WM_SETTINGCHANGE:
		{
			printf("WM_SETTINGCHANGE\n");
			SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
			// ulScrollLines usually equals 3 or 0 (for no scrolling)
			// WHEEL_DELTA equals 120, so iDeltaPerLine will be 40
			if (ulScrollLines)
				iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
			else
				iDeltaPerLine = 0;
			break;
		}
		case WM_CLOSE:
		{
			printf("WM_CLOSE\n");
			DestroyWindow(hwnd);
			break;
		}
		case WM_DESTROY:
		{
			printf("WM_DESTROY\n");
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
				case 0:
				{
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int startOpengl() {

	ctx.sx = 1000;
	ctx.sy = 650;
	ZeroMemory(&ctx.wc, sizeof(WNDCLASS));
	ctx.wc.hInstance = GetModuleHandle(NULL);
	ctx.wc.lpfnWndProc = WndProc;
	ctx.wc.lpszClassName = L"TestOpengl_01";
	ctx.wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	ctx.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	ctx.wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	if (0 == RegisterClass(&ctx.wc)) return GL_TRUE;
	/* create window */
	ctx.hwnd = CreateWindowA("TestOpengl_01", szTitle,
							WS_OVERLAPPEDWINDOW,
							340, 20, ctx.sx, ctx.sy,
							NULL, NULL, NULL, NULL);

	/* get the device context */
	ctx.hdc = GetDC(ctx.hwnd);
	// WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL

	/* find pixel format */

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int visual = ChoosePixelFormat(ctx.hdc, &pfd);
	/* set the pixel format for the dc */
	//SetPixelFormat(ctx.hdc, visual, &pfd);
	SetPixelFormat(ctx.hdc, visual, &pfd); // it is a hack, so i know that this number 80 activate the 4x AntiAlias on my computer
	/* create rendering context */
	ctx.hglrc = wglCreateContext(ctx.hdc);
	wglMakeCurrent(ctx.hdc, ctx.hglrc);
	LoadOGL();
	LoadWGL();

	printf(wglGetExtensionsStringARB(ctx.hdc));

	int attributes[] = {
	  WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	  WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	  WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	  WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	  WGL_COLOR_BITS_ARB, 32,
	  WGL_DEPTH_BITS_ARB, 24,
	  WGL_STENCIL_BITS_ARB, 8,
	  WGL_SAMPLE_BUFFERS_ARB, 1, // Number of buffers (must be 1 at time of writing)
	  WGL_SAMPLES_ARB, 4,        // Number of samples
	  0
	};

	int attrib[] = { WGL_NUMBER_PIXEL_FORMATS_ARB };
	int nResults[1] = { 0 };
	int pixFmt = 1;
	wglGetPixelFormatAttribivARB(ctx.hdc, pixFmt, 0, 1, attrib, nResults);

	GLint pfAttribCount[] = { WGL_NUMBER_PIXEL_FORMATS_ARB };
	//GLint pfAttribList[] = { 
	//	WGL_DRAW_TO_WINDOW_ARB, 
	//	WGL_ACCELERATION_ARB,
	//	WGL_SUPPORT_OPENGL_ARB, 
	//	WGL_DOUBLE_BUFFER_ARB, 
	//	WGL_DEPTH_BITS_ARB, 
	//	WGL_STENCIL_BITS_ARB, 
	//	WGL_RED_BITS_ARB, 
	//	WGL_GREEN_BITS_ARB, 
	//	WGL_BLUE_BITS_ARB, 
	//	WGL_ALPHA_BITS_ARB 
	// };
	GLint pfAttribList[] = {
		//WGL_NUMBER_PIXEL_FORMATS_ARB,
		//WGL_SUPPORT_OPENGL_ARB,
		//WGL_DOUBLE_BUFFER_ARB,
		//WGL_DRAW_TO_WINDOW_ARB,
		//WGL_DRAW_TO_BITMAP_ARB,
		//WGL_ACCELERATION_ARB,
		//WGL_NUMBER_OVERLAYS_ARB,
		//WGL_NUMBER_UNDERLAYS_ARB,
		//WGL_TRANSPARENT_ARB,
		//WGL_STEREO_ARB,
		//WGL_COLOR_BITS_ARB,
		//WGL_RED_BITS_ARB,
		//WGL_RED_SHIFT_ARB,
		//WGL_GREEN_BITS_ARB,
		//WGL_GREEN_SHIFT_ARB,
		//WGL_BLUE_BITS_ARB,
		//WGL_BLUE_SHIFT_ARB,
		//WGL_ALPHA_BITS_ARB,
		//WGL_ALPHA_SHIFT_ARB,
		//WGL_ACCUM_BITS_ARB,
		//WGL_ACCUM_RED_BITS_ARB,
		//WGL_ACCUM_GREEN_BITS_ARB,
		//WGL_ACCUM_BLUE_BITS_ARB,
		//WGL_ACCUM_ALPHA_BITS_ARB,
		//WGL_DEPTH_BITS_ARB,
		//WGL_STENCIL_BITS_ARB,
		//WGL_SHARE_DEPTH_ARB,
		//WGL_SHARE_STENCIL_ARB,
		//WGL_SHARE_ACCUM_ARB,
		//WGL_PIXEL_TYPE_ARB,
		//WGL_TRANSPARENT_RED_VALUE_ARB,
		//WGL_TRANSPARENT_GREEN_VALUE_ARB,
		//WGL_TRANSPARENT_BLUE_VALUE_ARB,
		//WGL_TRANSPARENT_ALPHA_VALUE_ARB,
		//WGL_TRANSPARENT_INDEX_VALUE_ARB,
		//WGL_NEED_PALETTE_ARB,
		//WGL_NEED_SYSTEM_PALETTE_ARB,
		//WGL_SWAP_LAYER_BUFFERS_ARB,
		//WGL_SWAP_METHOD_ARB,
		//WGL_SUPPORT_GDI_ARB,
		//WGL_AUX_BUFFERS_ARB,
		//WGL_SAMPLES_ARB

			WGL_SUPPORT_OPENGL_ARB,
			WGL_SUPPORT_GDI_ARB,
			WGL_DOUBLE_BUFFER_ARB,
			WGL_DRAW_TO_WINDOW_ARB,
			WGL_DRAW_TO_BITMAP_ARB,
			WGL_SWAP_METHOD_ARB,
			WGL_PIXEL_TYPE_ARB,
			WGL_AUX_BUFFERS_ARB,
			WGL_COLOR_BITS_ARB,
			WGL_DEPTH_BITS_ARB,
			WGL_STENCIL_BITS_ARB,
			WGL_RED_BITS_ARB,
			WGL_GREEN_BITS_ARB,
			WGL_BLUE_BITS_ARB,
			WGL_ALPHA_BITS_ARB,
			WGL_ACCUM_BITS_ARB,
			WGL_ACCUM_RED_BITS_ARB,
			WGL_ACCUM_GREEN_BITS_ARB,
			WGL_ACCUM_BLUE_BITS_ARB,
			WGL_ACCUM_ALPHA_BITS_ARB,
			WGL_SAMPLES_ARB,
	};

	FILE* LogPixelFormat = fopen("LogPixelFormat.py", "wb");

	int nPixelFormatCount = 0;
	wglGetPixelFormatAttribivARB(ctx.hdc, 1, 0, 1, pfAttribCount, &nPixelFormatCount);
	for (int i = 1; i < nPixelFormatCount; i++) {
		GLint results[21];
		fprintf(LogPixelFormat, "\nPixel format %d details:\n", i);
		wglGetPixelFormatAttribivARB(ctx.hdc, i, 0, 21, pfAttribList, results);
		//printf(" Draw to Window = %x:\n", results[0]); 
		//printf(" HW Accelerated = %x:\n", results[1]); 
		//printf(" Supports OpenGL = %x:\n", results[2]); 
		//printf(" Double Buffered = %x:\n", results[3]); 
		//printf(" Depth Bits = %x:\n", results[4]); 
		//printf(" Stencil Bits = %x:\n", results[5]); 
		//printf(" Red Bits = %x:\n", results[6]); 
		//printf(" Green Bits = %x:\n", results[7]); 
		//printf(" Blue Bits = %x:\n", results[8]); 
		//printf(" Alpha Bits = %x:\n", results[9]);

		//fprintf(LogPixelFormat, " WGL_NUMBER_PIXEL_FORMATS_ARB = %x:\n", results[0]);
		fprintf(LogPixelFormat, " WGL_SUPPORT_OPENGL_ARB = %d:\n", results[0]);
		fprintf(LogPixelFormat, " WGL_SUPPORT_GDI_ARB = %d:\n", results[1]);
		fprintf(LogPixelFormat, " WGL_DOUBLE_BUFFER_ARB = %d:\n", results[2]);
		fprintf(LogPixelFormat, " WGL_DRAW_TO_WINDOW_ARB = %d:\n", results[3]);
		fprintf(LogPixelFormat, " WGL_DRAW_TO_BITMAP_ARB = %d:\n", results[4]);
		fprintf(LogPixelFormat, " WGL_SWAP_METHOD_ARB = 0x%x:\n", results[5]);
		fprintf(LogPixelFormat, " WGL_PIXEL_TYPE_ARB = 0x%x:\n", results[6]);
		fprintf(LogPixelFormat, " WGL_SAMPLES_ARB = %d:\n", results[20]);
		fprintf(LogPixelFormat, " WGL_AUX_BUFFERS_ARB = %d:\n", results[7]);
		fprintf(LogPixelFormat, " WGL_COLOR_BITS_ARB = %d:\n", results[8]);
		fprintf(LogPixelFormat, " WGL_DEPTH_BITS_ARB = %d:\n", results[9]);
		fprintf(LogPixelFormat, " WGL_STENCIL_BITS_ARB = %d:\n", results[10]);
		fprintf(LogPixelFormat, " WGL_RED_BITS_ARB = %d:\n", results[11]);
		fprintf(LogPixelFormat, " WGL_GREEN_BITS_ARB = %d:\n", results[12]);
		fprintf(LogPixelFormat, " WGL_BLUE_BITS_ARB = %d:\n", results[13]);
		fprintf(LogPixelFormat, " WGL_ALPHA_BITS_ARB = %d:\n", results[14]);
		fprintf(LogPixelFormat, " WGL_ACCUM_BITS_ARB = %d:\n", results[15]);
		fprintf(LogPixelFormat, " WGL_ACCUM_RED_BITS_ARB = %d:\n", results[16]);
		fprintf(LogPixelFormat, " WGL_ACCUM_GREEN_BITS_ARB = %d:\n", results[17]);
		fprintf(LogPixelFormat, " WGL_ACCUM_BLUE_BITS_ARB = %d:\n", results[18]);
		fprintf(LogPixelFormat, " WGL_ACCUM_ALPHA_BITS_ARB = %d:\n", results[19]);
		//fprintf(LogPixelFormat, " WGL_RED_SHIFT_ARB = %d:\n", results[12]);
		//fprintf(LogPixelFormat, " WGL_GREEN_SHIFT_ARB = %d:\n", results[14]);
		//fprintf(LogPixelFormat, " WGL_BLUE_SHIFT_ARB = %d:\n", results[16]);
		//fprintf(LogPixelFormat, " WGL_ALPHA_SHIFT_ARB = %d:\n", results[18]);
		//fprintf(LogPixelFormat, " WGL_ACCELERATION_ARB = %x:\n", results[5]);
		//fprintf(LogPixelFormat, " WGL_NUMBER_OVERLAYS_ARB = %x:\n", results[6]);
		//fprintf(LogPixelFormat, " WGL_NUMBER_UNDERLAYS_ARB = %x:\n", results[7]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_ARB = %x:\n", results[8]);
		//fprintf(LogPixelFormat, " WGL_STEREO_ARB = %x:\n", results[9]);
		//fprintf(LogPixelFormat, " WGL_SHARE_DEPTH_ARB = %x:\n", results[26]);
		//fprintf(LogPixelFormat, " WGL_SHARE_STENCIL_ARB = %x:\n", results[27]);
		//fprintf(LogPixelFormat, " WGL_SHARE_ACCUM_ARB = %x:\n", results[28]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_RED_VALUE_ARB = %x:\n", results[30]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_GREEN_VALUE_ARB = %x:\n", results[31]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_BLUE_VALUE_ARB = %x:\n", results[32]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_ALPHA_VALUE_ARB = %x:\n", results[33]);
		//fprintf(LogPixelFormat, " WGL_TRANSPARENT_INDEX_VALUE_ARB = %x:\n", results[34]);
		//fprintf(LogPixelFormat, " WGL_NEED_PALETTE_ARB = %x:\n", results[35]);
		//fprintf(LogPixelFormat, " WGL_NEED_SYSTEM_PALETTE_ARB = %x:\n", results[36]);
		//fprintf(LogPixelFormat, " WGL_SWAP_LAYER_BUFFERS_ARB = %x:\n", results[37]);
	}
	fprintf(LogPixelFormat, "\n\n\n\n");

	fclose(LogPixelFormat);

	fbo = new frameBuffer(1, uivec2(100, 60) * 10);
	return 1;
}

void processKeyPress(pKeyMap keymap) {
	keymap->lshift = GetAsyncKeyState(VK_LSHIFT);
	keymap->rshift = GetAsyncKeyState(VK_RSHIFT);
	keymap->lctrl = GetAsyncKeyState(VK_LCONTROL);
	keymap->rctrl = GetAsyncKeyState(VK_RCONTROL);
	keymap->lalt = GetAsyncKeyState(VK_LMENU);
	keymap->ralt = GetAsyncKeyState(VK_RMENU);
	keymap->alt = keymap->lalt || keymap->ralt || GetAsyncKeyState(VK_MENU);

	keymap->space = GetAsyncKeyState(VK_SPACE);
	keymap->win = GetAsyncKeyState(VK_LWIN) || GetAsyncKeyState(VK_RWIN);
	keymap->capsl = GetAsyncKeyState(VK_CAPITAL);
	keymap->numl = GetAsyncKeyState(VK_NUMLOCK);
	keymap->scroll = GetAsyncKeyState(VK_SCROLL);
	keymap->up = 0;
	keymap->down = 0;

	WPARAM wParam = 0;

	if (GetAsyncKeyState(VK_DELETE)) {
		cls();
	}
	if (GetAsyncKeyState(VK_SUBTRACT)) {
		activecamera->fov -= 0.01f;
		//activecamera->fov = clamp(activecamera->fov - 0.01f, 0.0001f, 3.14f);

	}
	if (GetAsyncKeyState(VK_ADD)) {
		activecamera->fov += 0.01f;
		//activecamera->fov = clamp(activecamera->fov + 0.01f, 0.0001f, 3.14f);
	}
	if (GetAsyncKeyState(VK_UP)) {
		activecamera->grot.y -= 0.05f;
		//activecamera->rotate(vec3(0,0.05f,0));
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		activecamera->grot.y += 0.05f;
		//activecamera->rotate(vec3(0, -0.05f, 0));
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		activecamera->grot.x += 0.05f;
		//activecamera->rotate(vec3(0, 0, 0.05f));
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		activecamera->grot.x -= 0.05f;
		//activecamera->rotate(vec3(0, 0, -0.05f));
	}
		activecamera->calcMatrix();
	if (GetAsyncKeyState(0x51)) { // Q
		//torus->grot.y  -= 0.05f;
		//esfera->grot.y -= 0.05f;
		//torus->grot.w  -= 0.05f;
		//esfera->grot.w -= 0.05f;
	}
	if (GetAsyncKeyState(0x45)) { // E		 
		//torus->grot.y  += 0.05f;
		//esfera->grot.y += 0.05f;
		//torus->grot.w  += 0.05f;
		//esfera->grot.w += 0.05f;
	}
	if (GetAsyncKeyState(0x52)) { // R
		//torus->grot  = vec4(0, 0, 0.00000001f, 0.0003333);
		//esfera->grot = vec4(0, 0, 0.00000001f, 0.000333);
	}
	if (GetAsyncKeyState(0x57)) { // W
		//torus->grot.x  += 0.05f;
		//esfera->grot.x += 0.05f;
		//torus->grot.w  += 0.05f;
		//esfera->grot.w += 0.05f;
		activecamera->moveF(16);
	}
	if (GetAsyncKeyState(0x53)) { // S		 
		//torus->grot.x  -= 0.05f;
		//esfera->grot.x -= 0.05f;
		//torus->grot.w  -= 0.05f;
		//esfera->grot.w -= 0.05f;
		activecamera->moveB(16);
	}
	if (GetAsyncKeyState(0x41)) { // A		  
		//torus->grot.z   += 0.05f;
		//esfera->grot.z  += 0.05f;
		//torus->grot.w   += 0.05f;
		//esfera->grot.w  += 0.05f;
		activecamera->moveL(16);
	}
	if (GetAsyncKeyState(0x44)) { // D		  
		//torus->grot.z  -= 0.05f;
		//esfera->grot.z -= 0.05f;
		//torus->grot.w  -= 0.05f;
		//esfera->grot.w -= 0.05f;
		activecamera->moveR(16);

	}
	if (GetAsyncKeyState(VK_SPACE)) {
		//torus->malhas[0]->mMaterial->detach();
		//esfera->malhas[0]->mMaterial->detach();
		//torus->malhas[0]->mMaterial = new material("default");
		//esfera->malhas[0]->mMaterial = new material("default");
		//torus->malhas[0]->mMaterial->read(torus->malhas[0]->mMaterial->mName);
		//cube->malhas[0]->mMaterial->read(torus->malhas[0]->mMaterial->mName);
		//esfera->malhas[0]->mMaterial->read(esfera->malhas[0]->mMaterial->mName);
		//grid->malhas[0]->mMaterial->read(grid->malhas[0]->mMaterial->mName);
		//axis->malhas[0]->mMaterial->read(axis->malhas[0]->mMaterial->mName);
		//plane->malhas[0]->mMaterial->read(plane->malhas[0]->mMaterial->mName);
		word.refreshShaders();
		printf("Material Reloaded\n\n");
	}
}

void onRenderScene() {
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	//glEnable(GL_MULTISAMPLE_ARB);
	//glDisable(GL_MULTISAMPLE_ARB);
	glClearColor(0.2f, 0.2f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	fbo->bind();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, hdrTextures[0]);
	glLineWidth(1.f);


	//grid->draw();
	glLineWidth(2.0f);
	//axis->draw();
	glLineWidth(1.0f);
	glDepthFunc(GL_LEQUAL);
	//torus->draw();
	//cube->draw();
	//esfera->draw();
	word.draw();
	fbo->unbind();
	activecamera->aspect = max(float(ctx.sx), 1.0f) / max(float(ctx.sy), 1.0f);
	activecamera->calcMatrix();
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, fbo->textures[0]);
	plane->draw();
}

void onRenderScene2() {
	glEnable(GL_MULTISAMPLE_ARB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(theta, 0.5f, -1.5f, 0.0f);
	//glBegin (GL_TRIANGLES);
	//	glColor3f (1.0f, 0.0f, 0.0f);   glVertex3f (0.0f, 1.0f, 1.0f);
	//	glColor3f (0.0f, 1.0f, 0.0f);   glVertex3f (0.87f, -0.5f, -0.5f);
	//	glColor3f (0.0f, 0.0f, 1.0f);   glVertex3f (-0.87f, -0.5f, -0.5f);
	//glEnd ();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	drawText("abcdefghijklmnopqrstuvwxyz\n", vec2(-1.f, 1.f), vec2(1.f, 1.f), vec4(1.f, 1.f, 1.f, 1.f), vec2(0.f, 0.f));
	glEnable(GL_DEPTH_TEST);

	theta -= 0.1f;

}

void onResize(int x, int y, WPARAM wParam, LPARAM lParam) {
	activecamera->aspect = max(float(x), 1.0f) / max(float(y), 1.0f);
	printf("aspect:%f\n", activecamera->aspect);
	ctx.sx = x;
	ctx.sy = y;
	glViewport(0, 0, x, y);
	onRenderScene();
	SwapBuffers(ctx.hdc);
	fbo->resize(uivec2(x,y)/**1.25f*/);
	printf("WM_SIZE: %i:%i\n", x, y);
}

void onMove(int x, int y, WPARAM wParam, LPARAM lParam) {
	//printf("WM_MOVE\n");
}

void onPaint(PAINTSTRUCT* rc, WPARAM wParam, LPARAM lParam) {
	//printf("WM_PAINT\n");
	onRenderScene();
}

void onMouseWhell(int val, WPARAM wParam, LPARAM lParam) {
	if (val > 0) {
		activecamera->fov  += 0.01f;
	}
	else if (val < 0) {
		activecamera->fov -=  0.01f;
	}
		activecamera->calcMatrix();
	//printf("WM_MOUSEWHEEL %f\n", activecamera->fov);
}

void onMouseMove(int xWindow, int yWindow, WPARAM wParam, LPARAM lParam, int x, int y) {
	//printf("mousemove: x=%i y=%i w=%i l=%i\n", x, y, wParam, lParam);
	if (GetAsyncKeyState(VK_LBUTTON)) {
			activecamera->grot.y -= y * 0.005f;
			activecamera->grot.y -= y * 0.005f;
			activecamera->grot.x += x * 0.005f;
			activecamera->grot.x += x * 0.005f;
		//activecamera->rotate(vec3(0, 0, -y * 0.005f));
		//activecamera->rotate(vec3(x * 0.005f, 0, 0));
	}
	if (GetAsyncKeyState(VK_MBUTTON)) {
		activecamera->fov -= x * 0.005f + y * 0.005f;
	}
}

void onComand(int cmd, WPARAM wParam, LPARAM lParam) {
	printf("WM_COMMAND\n");
}



void onKeyPress(int key, WPARAM wParam, LPARAM lParam, pKeyMap keymap, UINT Message) {
	if (Message == WM_KEYDOWN)
		switch (key) {
			case VK_SUBTRACT:
			{
				activecamera->fov -= 0.01f;
				break;
			}
			case VK_ADD:
			{
				activecamera->fov += 0.01f;
				break;
			}
			case VK_UP:
			{
				activecamera->grot.x += 0.05f;
				break;
			}
			case VK_DOWN:
			{
				activecamera->grot.x -= 0.05f;
				break;
			}
			case VK_LEFT:
			{
				activecamera->grot.y =- 0.05f;
				break;
			}
			case VK_RIGHT:
			{
				activecamera->grot.y -= 0.05f;
				break;
			}
			case 0x51:
			{ // Q
				torus->grot.y  -= 0.05f;
				esfera->grot.y -=0.05f;
				torus->grot.w  -= 0.05f;
				esfera->grot.w -=0.05f;
				break;
			}
			case 0x45:
			{ // E		 
				torus->grot.y = +0.05f;
				esfera->grot.y = +0.05f;
				torus->grot.w = +0.05f;
				esfera->grot.w = +0.05f;
				break;
			}
			case 0x52:
			{ // R
				torus->grot = vec4(0, 0, 0.00000001f, 0.0003333);
				esfera->grot = vec4(0, 0, 0.00000001f, 0.000333);
				break;
			}
			case 0x57:
			{ // W
				torus->grot.x += 0.05f ;
				esfera->grot.x += +0.05f;
				torus->grot.w += 0.05f ;
				esfera->grot.w += +0.05f;
				break;
			}
			case 0x53:
			{ // S		 
				torus->grot.x  -= 0.05f;
				esfera->grot.x -= 0.05f;
				torus->grot.w  -= 0.05f;
				esfera->grot.w -= 0.05f;
				break;				   
			}						   
			case 0x41:				   
			{ // A		  			   
				torus->grot.z  += 0.05f;
				esfera->grot.z += 0.05f;
				torus->grot.w  += 0.05f;
				esfera->grot.w += 0.05f;
				break;				  
			}						   
			case 0x44:				   
			{ // D		  			   
				torus->grot.z  -= 0.05f;
				esfera->grot.z -= 0.05f;
				torus->grot.w  -= 0.05f;
				esfera->grot.w -= 0.05f;
				break;
			}
			case VK_SPACE:
			{
				//torus->malhas[0]->mMaterial->detach();
				//esfera->malhas[0]->mMaterial->detach();
				torus->malhas[0]->mMaterial->read(torus->malhas[0]->mMaterial->mName);
				esfera->malhas[0]->mMaterial->read(esfera->malhas[0]->mMaterial->mName);
				printf("Material Reloaded\n\n");
				break;
			}
			default:
			{
				break;
			}
		}
}

void decodeKeyPress(int key, WPARAM wParam, LPARAM lParam, pKeyMap keymap, UINT Message) {
	//char a[1000];
//printf("%i %i %i %i; %i %i %i %i %i; %i %i %i %i %i %i %i; \n", LOWORD (lParam), HIWORD (lParam),LOWORD (wParam), HIWORD (wParam), HIWORD (lParam)&0x01000000, HIWORD (lParam)&0x02000000, HIWORD (lParam)&0x04000000, HIWORD (lParam)&0x08000000, HIWORD (lParam)&0x10000000, GetKeyState (VK_SHIFT), GetKeyState (VK_LSHIFT), GetKeyState (VK_RSHIFT), GetKeyState (VK_LCONTROL), GetKeyState (VK_RCONTROL), GetKeyState (VK_LMENU), GetKeyState (VK_RMENU));
	//HIWORD (lParam)&0x02000000, HIWORD (lParam)&0x04000000, HIWORD (lParam)&0x08000000, HIWORD (lParam)&0x10000000);
//SetWindowTextA(hwnd, a);
	//keymap->up = (lParam & 0x80000000) >> 31;
	//keymap->down = !(lParam & 0x80000000) >> 31;
	//keymap->lalt = GetAsyncKeyState(VK_LMENU);
	//keymap->ralt = GetAsyncKeyState(VK_RMENU);
	//keymap->lctrl = GetAsyncKeyState(VK_LCONTROL);
	//keymap->lctrl = GetAsyncKeyState(VK_RCONTROL);
	//keymap->lshift = GetAsyncKeyState(VK_LSHIFT);
	//keymap->rshift = GetAsyncKeyState(VK_RSHIFT);
	////keymap->capsl = GetAsyncKeyState(VK_LMENU);
	////keymap->lalt = GetAsyncKeyState(VK_LMENU);
	return;
	if (wParam >= 0x30 && wParam <= 0x39)	// ASCII 0 - 9
		printf("* %c ", wParam);
	else if (wParam >= 0x41 && wParam <= 0x5A)	// ASCII A - Z
		printf("* %c ", wParam);
	else if (wParam >= 0x60 && wParam <= 0x69)	// NUMPAD 0 - 0
		printf("* NUMPAD %c ", wParam - 0x30);
	else if (wParam >= 0x70 && wParam <= 0x87)	// NUMPAD 0 - 0
		printf("* F%i ", wParam - 0x6F);
	else
		switch (wParam) {
			case  VK_LBUTTON:	printf("VK_LBUTTON                      ");	break;
			case  VK_RBUTTON:	printf("VK_RBUTTON                      ");	break;
			case  VK_CANCEL:	printf("VK_CANCEL                       ");	break;
			case  VK_MBUTTON:	printf("VK_MBUTTON                      ");	break;
			case  VK_XBUTTON1:	printf("VK_XBUTTON1                     ");	break;
			case  VK_XBUTTON2:	printf("VK_XBUTTON2                     ");	break;
			case  VK_BACK:	printf("VK_BACK                         ");	break;
			case  VK_TAB:	printf("VK_TAB                          ");	break;
			case  VK_CLEAR:	printf("VK_CLEAR                        ");	break;
			case  VK_RETURN:	printf("VK_RETURN                       ");	break;
			case  VK_SHIFT:	printf("VK_SHIFT                        ");	break;
			case  VK_CONTROL:	printf("VK_CONTROL                      ");	break;
			case  VK_MENU:	printf("VK_MENU                         ");	break;
			case  VK_PAUSE:	printf("VK_PAUSE                        ");	break;
			case  VK_CAPITAL:	printf("VK_CAPITAL                      ");	break;
			case  VK_KANA:	printf("VK_KANA,VK_HANGEUL,VK_HANGUL    ");	break;
			case  VK_JUNJA:	printf("VK_JUNJA                        ");	break;
			case  VK_FINAL:	printf("VK_FINAL                        ");	break;
			case  VK_HANJA:	printf("VK_HANJA,VK_KANJI               ");	break;
			case  VK_ESCAPE:	printf("VK_ESCAPE                       ");	break;
			case  VK_CONVERT:	printf("VK_CONVERT                      ");	break;
			case  VK_NONCONVERT:	printf("VK_NONCONVERT                   ");	break;
			case  VK_ACCEPT:	printf("VK_ACCEPT                       ");	break;
			case  VK_MODECHANGE:	printf("VK_MODECHANGE                   ");	break;
			case  VK_SPACE:	printf("VK_SPACE                        ");	break;
			case  VK_PRIOR:	printf("VK_PRIOR                        ");	break;
			case  VK_NEXT:	printf("VK_NEXT                         ");	break;
			case  VK_END:	printf("VK_END                          ");	break;
			case  VK_HOME:	printf("VK_HOME                         ");	break;
			case  VK_LEFT:	printf("VK_LEFT                         ");	break;
			case  VK_UP:	printf("VK_UP                           ");	break;
			case  VK_RIGHT:	printf("VK_RIGHT                        ");	break;
			case  VK_DOWN:	printf("VK_DOWN                         ");	break;
			case  VK_SELECT:	printf("VK_SELECT                       ");	break;
			case  VK_PRINT:	printf("VK_PRINT                        ");	break;
			case  VK_EXECUTE:	printf("VK_EXECUTE                      ");	break;
			case  VK_SNAPSHOT:	printf("VK_SNAPSHOT                     ");	break;
			case  VK_INSERT:	printf("VK_INSERT                       ");	break;
			case  VK_DELETE:	printf("VK_DELETE                       ");	break;
			case  VK_HELP:	printf("VK_HELP                         ");	break;
			case  VK_LWIN:	printf("VK_LWIN                         ");	break;
			case  VK_RWIN:	printf("VK_RWIN                         ");	break;
			case  VK_APPS:	printf("VK_APPS                         ");	break;
			case  VK_SLEEP:	printf("VK_SLEEP                        ");	break;
			case  VK_MULTIPLY:	printf("VK_MULTIPLY                     ");	break;
			case  VK_ADD:	printf("VK_ADD                          ");	break;
			case  VK_SEPARATOR:	printf("VK_SEPARATOR                    ");	break;
			case  VK_SUBTRACT:	printf("VK_SUBTRACT                     ");	break;
			case  VK_DECIMAL:	printf("VK_DECIMAL                      ");	break;
			case  VK_DIVIDE:	printf("VK_DIVIDE                       ");	break;
			case  VK_NAVIGATION_VIEW:	printf("VK_NAVIGATION_VIEW              ");	break;
			case  VK_NAVIGATION_MENU:	printf("VK_NAVIGATION_MENU              ");	break;
			case  VK_NAVIGATION_UP:	printf("VK_NAVIGATION_UP                ");	break;
			case  VK_NAVIGATION_DOWN:	printf("VK_NAVIGATION_DOWN              ");	break;
			case  VK_NAVIGATION_LEFT:	printf("VK_NAVIGATION_LEFT              ");	break;
			case  VK_NAVIGATION_RIGHT:	printf("VK_NAVIGATION_RIGHT             ");	break;
			case  VK_NAVIGATION_ACCEPT:	printf("VK_NAVIGATION_ACCEPT            ");	break;
			case  VK_NAVIGATION_CANCEL:	printf("VK_NAVIGATION_CANCEL            ");	break;
			case  VK_NUMLOCK:	printf("VK_NUMLOCK                      ");	break;
			case  VK_SCROLL:	printf("VK_SCROLL                       ");	break;
			case  VK_OEM_NEC_EQUAL:	printf("VK_OEM_NEC_EQUAL,VK_OEM_FJ_JISHO");	break;
			case  VK_OEM_FJ_MASSHOU:	printf("VK_OEM_FJ_MASSHOU               ");	break;
			case  VK_OEM_FJ_TOUROKU:	printf("VK_OEM_FJ_TOUROKU               ");	break;
			case  VK_OEM_FJ_LOYA:	printf("VK_OEM_FJ_LOYA                  ");	break;
			case  VK_OEM_FJ_ROYA:	printf("VK_OEM_FJ_ROYA                  ");	break;
			case  VK_LSHIFT:	printf("VK_LSHIFT                       ");	break;
			case  VK_RSHIFT:	printf("VK_RSHIFT                       ");	break;
			case  VK_LCONTROL:	printf("VK_LCONTROL                     ");	break;
			case  VK_RCONTROL:	printf("VK_RCONTROL                     ");	break;
			case  VK_LMENU:	printf("VK_LMENU                        ");	break;
			case  VK_RMENU:	printf("VK_RMENU                        ");	break;
			case  VK_BROWSER_BACK:	printf("VK_BROWSER_BACK                 ");	break;
			case  VK_BROWSER_FORWARD:	printf("VK_BROWSER_FORWARD              ");	break;
			case  VK_BROWSER_REFRESH:	printf("VK_BROWSER_REFRESH              ");	break;
			case  VK_BROWSER_STOP:	printf("VK_BROWSER_STOP                 ");	break;
			case  VK_BROWSER_SEARCH:	printf("VK_BROWSER_SEARCH               ");	break;
			case  VK_BROWSER_FAVORITES:	printf("VK_BROWSER_FAVORITES            ");	break;
			case  VK_BROWSER_HOME:	printf("VK_BROWSER_HOME                 ");	break;
			case  VK_VOLUME_MUTE:	printf("VK_VOLUME_MUTE                  ");	break;
			case  VK_VOLUME_DOWN:	printf("VK_VOLUME_DOWN                  ");	break;
			case  VK_VOLUME_UP:	printf("VK_VOLUME_UP                    ");	break;
			case  VK_MEDIA_NEXT_TRACK:	printf("VK_MEDIA_NEXT_TRACK             ");	break;
			case  VK_MEDIA_PREV_TRACK:	printf("VK_MEDIA_PREV_TRACK             ");	break;
			case  VK_MEDIA_STOP:	printf("VK_MEDIA_STOP                   ");	break;
			case  VK_MEDIA_PLAY_PAUSE:	printf("VK_MEDIA_PLAY_PAUSE             ");	break;
			case  VK_LAUNCH_MAIL:	printf("VK_LAUNCH_MAIL                  ");	break;
			case  VK_LAUNCH_MEDIA_SELECT:	printf("VK_LAUNCH_MEDIA_SELECT          ");	break;
			case  VK_LAUNCH_APP1:	printf("VK_LAUNCH_APP1                  ");	break;
			case  VK_LAUNCH_APP2:	printf("VK_LAUNCH_APP2                  ");	break;
			case  VK_OEM_1:	printf("VK_OEM_1                        ");	break;
			case  VK_OEM_PLUS:	printf("VK_OEM_PLUS                     ");	break;
			case  VK_OEM_COMMA:	printf("VK_OEM_COMMA                    ");	break;
			case  VK_OEM_MINUS:	printf("VK_OEM_MINUS                    ");	break;
			case  VK_OEM_PERIOD:	printf("VK_OEM_PERIOD                   ");	break;
			case  VK_OEM_2:	printf("VK_OEM_2                        ");	break;
			case  VK_OEM_3:	printf("VK_OEM_3                        ");	break;
			case  0xC1:	printf("VK_/ VK_DIVIDE                  ");	break;
			case  VK_GAMEPAD_A:	printf("VK_GAMEPAD_A                    ");	break;
			case  VK_GAMEPAD_B:	printf("VK_GAMEPAD_B                    ");	break;
			case  VK_GAMEPAD_X:	printf("VK_GAMEPAD_X                    ");	break;
			case  VK_GAMEPAD_Y:	printf("VK_GAMEPAD_Y                    ");	break;
			case  VK_GAMEPAD_RIGHT_SHOULDER:	printf("VK_GAMEPAD_RIGHT_SHOULDER       ");	break;
			case  VK_GAMEPAD_LEFT_SHOULDER:	printf("VK_GAMEPAD_LEFT_SHOULDER        ");	break;
			case  VK_GAMEPAD_LEFT_TRIGGER:	printf("VK_GAMEPAD_LEFT_TRIGGER         ");	break;
			case  VK_GAMEPAD_RIGHT_TRIGGER:	printf("VK_GAMEPAD_RIGHT_TRIGGER        ");	break;
			case  VK_GAMEPAD_DPAD_UP:	printf("VK_GAMEPAD_DPAD_UP              ");	break;
			case  VK_GAMEPAD_DPAD_DOWN:	printf("VK_GAMEPAD_DPAD_DOWN            ");	break;
			case  VK_GAMEPAD_DPAD_LEFT:	printf("VK_GAMEPAD_DPAD_LEFT            ");	break;
			case  VK_GAMEPAD_DPAD_RIGHT:	printf("VK_GAMEPAD_DPAD_RIGHT           ");	break;
			case  VK_GAMEPAD_MENU:	printf("VK_GAMEPAD_MENU                 ");	break;
			case  VK_GAMEPAD_VIEW:	printf("VK_GAMEPAD_VIEW                 ");	break;
			case  VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON:	printf("VK_GAMEPAD_LEFT_THUMBSTICK_BUTTO");	break;
			case  VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON:	printf("VK_GAMEPAD_RIGHT_THUMBSTICK_BUTT");	break;
			case  VK_GAMEPAD_LEFT_THUMBSTICK_UP:	printf("VK_GAMEPAD_LEFT_THUMBSTICK_UP   ");	break;
			case  VK_GAMEPAD_LEFT_THUMBSTICK_DOWN:	printf("VK_GAMEPAD_LEFT_THUMBSTICK_DOWN ");	break;
			case  VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT:	printf("VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT");	break;
			case  VK_GAMEPAD_LEFT_THUMBSTICK_LEFT:	printf("VK_GAMEPAD_LEFT_THUMBSTICK_LEFT ");	break;
			case  VK_GAMEPAD_RIGHT_THUMBSTICK_UP:	printf("VK_GAMEPAD_RIGHT_THUMBSTICK_UP  ");	break;
			case  VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN:	printf("VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN");	break;
			case  VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT:	printf("VK_GAMEPAD_RIGHT_THUMBSTICK_RIGH");	break;
			case  VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT:	printf("VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT");	break;
			case  VK_OEM_4:	printf("VK_OEM_4                        ");	break;
			case  VK_OEM_5:	printf("VK_OEM_5                        ");	break;
			case  VK_OEM_6:	printf("VK_OEM_6                        ");	break;
			case  VK_OEM_7:	printf("VK_OEM_7                        ");	break;
			case  VK_OEM_8:	printf("VK_OEM_8                        ");	break;
			case  VK_OEM_AX:	printf("VK_OEM_AX                       ");	break;
			case  VK_OEM_102:	printf("VK_OEM_102                      ");	break;
			case  VK_ICO_HELP:	printf("VK_ICO_HELP                     ");	break;
			case  VK_ICO_00:	printf("VK_ICO_00                       ");	break;
			case  VK_PROCESSKEY:	printf("VK_PROCESSKEY                   ");	break;
			case  VK_ICO_CLEAR:	printf("VK_ICO_CLEAR                    ");	break;
			case  VK_PACKET:	printf("VK_PACKET                       ");	break;
			case  VK_OEM_RESET:	printf("VK_OEM_RESET                    ");	break;
			case  VK_OEM_JUMP:	printf("VK_OEM_JUMP                     ");	break;
			case  VK_OEM_PA1:	printf("VK_OEM_PA1                      ");	break;
			case  VK_OEM_PA2:	printf("VK_OEM_PA2                      ");	break;
			case  VK_OEM_PA3:	printf("VK_OEM_PA3                      ");	break;
			case  VK_OEM_WSCTRL:	printf("VK_OEM_WSCTRL                   ");	break;
			case  VK_OEM_CUSEL:	printf("VK_OEM_CUSEL                    ");	break;
			case  VK_OEM_ATTN:	printf("VK_OEM_ATTN                     ");	break;
			case  VK_OEM_FINISH:	printf("VK_OEM_FINISH                   ");	break;
			case  VK_OEM_COPY:	printf("VK_OEM_COPY                     ");	break;
			case  VK_OEM_AUTO:	printf("VK_OEM_AUTO                     ");	break;
			case  VK_OEM_ENLW:	printf("VK_OEM_ENLW                     ");	break;
			case  VK_OEM_BACKTAB:	printf("VK_OEM_BACKTAB                  ");	break;
			case  VK_ATTN:	printf("VK_ATTN                         ");	break;
			case  VK_CRSEL:	printf("VK_CRSEL                        ");	break;
			case  VK_EXSEL:	printf("VK_EXSEL                        ");	break;
			case  VK_EREOF:	printf("VK_EREOF                        ");	break;
			case  VK_PLAY:	printf("VK_PLAY                         ");	break;
			case  VK_ZOOM:	printf("VK_ZOOM                         ");	break;
			case  VK_NONAME:	printf("VK_NONAME                       ");	break;
			case  VK_PA1:	printf("VK_PA1                          ");	break;
			case  VK_OEM_CLEAR:	printf("VK_OEM_CLEAR                    ");	break;

			default:
				printf("???????????"); break;
		}

	printf("UP:%x pDown:%x alt:%x rep:%i scOEM:%x extended:%x 0x%x 0x%x '",
		   (lParam >> 31) & 0x1,
		   (lParam >> 30) & 0x1,
		   (lParam >> 29) & 0x1,
		   lParam & 0xffff,
		   (lParam >> 16) & 0xff,
		   (lParam >> 24) & 0x1,
		   lParam,
		   wParam);
	//printf("%c", lParam);
	//printf(" '");
	printf("%c", wParam);
	printf("' ");
	printf("L: %i%i%i%i %i%i%i%i:%i%i%i%i %i%i%i%i  %i%i%i%i %i%i%i%i:%i%i%i%i %i%i%i%i"
		   //"    W: %i%i%i%i %i%i%i%i:%i%i%i%i %i%i%i%i  %i%i%i%i %i%i%i%i:%i%i%i%i %i%i%i%i"
		   "\n\n",
		   (lParam >> 31) & 0x1, (lParam >> 30) & 0x1, (lParam >> 29) & 0x1, (lParam >> 28) & 0x1,
		   (lParam >> 27) & 0x1, (lParam >> 26) & 0x1, (lParam >> 25) & 0x1, (lParam >> 24) & 0x1,

		   (lParam >> 23) & 0x1, (lParam >> 22) & 0x1, (lParam >> 21) & 0x1, (lParam >> 20) & 0x1,
		   (lParam >> 19) & 0x1, (lParam >> 18) & 0x1, (lParam >> 17) & 0x1, (lParam >> 16) & 0x1,

		   (lParam >> 15) & 0x1, (lParam >> 14) & 0x1, (lParam >> 13) & 0x1, (lParam >> 12) & 0x1,
		   (lParam >> 11) & 0x1, (lParam >> 10) & 0x1, (lParam >> 9) & 0x1, (lParam >> 8) & 0x1,

		   (lParam >> 7) & 0x1, (lParam >> 6) & 0x1, (lParam >> 5) & 0x1, (lParam >> 4) & 0x1,
		   (lParam >> 3) & 0x1, (lParam >> 2) & 0x1, (lParam >> 1) & 0x1, (lParam >> 0) & 0x1);

	//(lParam & 0x01000000) >> 24, (lParam & 0x02000000) >> 25, (lParam & 0x04000000) >> 26, (lParam & 0x08000000) >> 27,
	//(lParam & 0x10000000) >> 28, (lParam & 0x20000000) >> 29, (lParam & 0x00000000) >> 30, (lParam & 0x80000000) >> 31, // 0x20000000 = alt


	 //(wParam & 0x80000000) >> 31, (wParam & 0x40000000) >> 30, (wParam & 0x20000000) >> 29, (wParam & 0x10000000) >> 28, 
	 //(wParam & 0x08000000) >> 27, (wParam & 0x04000000) >> 26, (wParam & 0x02000000) >> 25, (wParam & 0x01000000) >> 24, 

	 //(wParam & 0x00800000) >> 23, (wParam & 0x00400000) >> 22, (wParam & 0x00200000) >> 21, (wParam & 0x00100000) >> 20, 
	 //(wParam & 0x00080000) >> 19, (wParam & 0x00040000) >> 18, (wParam & 0x00020000) >> 17, (wParam & 0x00010000) >> 16, 

	 //(wParam & 0x00008000) >> 15, (wParam & 0x00004000) >> 14, (wParam & 0x00002000) >> 13, (wParam & 0x00001000) >> 12, 
	 //(wParam & 0x00000800) >> 11, (wParam & 0x00000400) >> 10, (wParam & 0x00000200) >> 9,  (wParam & 0x00000100) >> 8,  

	 //(wParam & 0x00000080) >> 7,  (wParam & 0x00000040) >> 6,  (wParam & 0x00000020) >> 5,  (wParam & 0x00000010) >> 4,   // 0x00000040 = ctrl
	 //(wParam & 0x00000008) >> 3,  (wParam & 0x00000004) >> 2,  (wParam & 0x00000002) >> 1,  (wParam & 0x00000001) >> 0 ); // 0x00000020 = shift



	//(lParam >> 0) & 0x1, (lParam >> 1) & 0x1, (lParam >> 2) & 0x1, (lParam >> 3) & 0x1,
	//(lParam >> 4) & 0x1, (lParam >> 5) & 0x1, (lParam >> 6) & 0x1, (lParam >> 7) & 0x1,
	//(lParam >> 8) & 0x1, (lParam >> 9) & 0x1, (lParam >> 10) & 0x1, (lParam >> 11) & 0x1,
	//(lParam >> 12) & 0x1, (lParam >> 13) & 0x1, (lParam >> 14) & 0x1, (lParam >> 15) & 0x1,
	//(lParam >> 16) & 0x1, (lParam >> 17) & 0x1, (lParam >> 18) & 0x1, (lParam >> 19) & 0x1,
	//(lParam >> 20) & 0x1, (lParam >> 21) & 0x1, (lParam >> 22) & 0x1, (lParam >> 23) & 0x1,
	//(lParam >> 24) & 0x1, (lParam >> 25) & 0x1, (lParam >> 26) & 0x1, (lParam >> 27) & 0x1,
	//(lParam >> 28) & 0x1, (lParam >> 29) & 0x1, (lParam >> 30) & 0x1, (lParam >> 31) & 0x1);

	//(lParam & 0x01000000) >> 24, (lParam & 0x02000000) >> 25, (lParam & 0x04000000) >> 26, (lParam & 0x08000000) >> 27,
	//(lParam & 0x10000000) >> 28, (lParam & 0x20000000) >> 29, (lParam & 0x00000000) >> 30, (lParam & 0x80000000) >> 31, // 0x20000000 = alt

	//(wParam & 0x00000001) >> 0, (wParam & 0x00000002) >> 1, (wParam & 0x00000004) >> 2, (wParam & 0x00000008) >> 3, // 0x00000020 = shift
	//(wParam & 0x00000010) >> 4, (wParam & 0x00000020) >> 5, (wParam & 0x00000040) >> 6, (wParam & 0x00000080) >> 7, // 0x00000040 = ctrl
	//(wParam & 0x00000100) >> 8, (wParam & 0x00000200) >> 9, (wParam & 0x00000400) >> 10, (wParam & 0x00000800) >> 11,
	//(wParam & 0x00001000) >> 12, (wParam & 0x00002000) >> 13, (wParam & 0x00004000) >> 14, (wParam & 0x00008000) >> 15,
	//(wParam & 0x00010000) >> 16, (wParam & 0x00020000) >> 17, (wParam & 0x00040000) >> 18, (wParam & 0x00080000) >> 19,
	//(wParam & 0x00100000) >> 20, (wParam & 0x00200000) >> 21, (wParam & 0x00400000) >> 22, (wParam & 0x00800000) >> 23,
	//(wParam & 0x01000000) >> 24, (wParam & 0x02000000) >> 25, (wParam & 0x04000000) >> 26, (wParam & 0x08000000) >> 27,
	//(wParam & 0x10000000) >> 28, (wParam & 0x20000000) >> 29, (wParam & 0x40000000) >> 30, (wParam & 0x80000000) >> 31);

//onKeyPress(wParam, wParam, lParam, keymap, Message);
}

void onMousePress(int x, int y, WPARAM wParam, LPARAM lParam, int up, int n) {

}






void cls() {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };    // home for the cursor
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return;
	}
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
									(TCHAR)' ',      // Character to write to the buffer
									dwConSize,       // Number of cells to write
									coordScreen,     // Coordinates of first cell
									&cCharsWritten)) // Receive number of characters written
	{
		return;
	}
	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return;
	}
	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
									csbi.wAttributes, // Character attributes to use
									dwConSize,        // Number of cells to set attribute
									coordScreen,      // Coordinates of first cell
									&cCharsWritten))  // Receive number of characters written
	{
		return;
	}
	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, coordScreen);
}




























