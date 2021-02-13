// pch.h: esse é um arquivo de cabeçalho pré-compilado.
// Os arquivos listados a seguir são compilados somente uma vez, melhorando o desempenho dos builds futuros.
// Isso também afeta o desempenho do IntelliSense, incluindo a conclusão de código e muitos recursos de navegação de código.
// No entanto, os arquivos listados aqui serão TODOS recompilados se qualquer um deles for atualizado entre builds.
// Não adicione aqui arquivos que você atualizará com frequência, pois isso anula a vantagem de desempenho.
#pragma once

#ifndef PCH_H
#define PCH_H

// adicione os cabeçalhos que você deseja pré-compilar aqui

// framework]
//#define WIN32_LEAN_AND_MEAN             // Excluir itens raramente utilizados dos cabeçalhos do Windows
#define _CRT_SECURE_NO_WARNINGS 1
#define _USE_MATH_DEFINES 1
#include "targetver.h"
// Arquivos de Cabeçalho do Windows
#include <windows.h>
// Arquivos de Cabeçalho C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


// #include <windows.h>
#include <WinBase.h>
// #include <minwindef.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <fstream>
#include <sstream>
#include <istream>
 #include <string>
#include <iostream>
#include <limits.h>
#include <tchar.h>
 #include <stdlib.h>
using std::vector;
using std::string;
#include "VectorsGLM.h"

#include "..\..\ImagemEditorTRK3\Source\Imagem_003\Imagem_003.h"
#include "..\..\ImagemEditorTRK3\Source\Imagem_003\Loaders.h"
#include <gl/GL.h>
#include "core+ext.h"
#include "GLgetFunctions.h"

#define _avoid_unessessary_include_defines 1
#if _avoid_unessessary_include_defines
#define WGL_3DL_stereo_control 1
#define WGL_EXT_display_color_table 1
#define WGL_EXT_make_current_read 1
#define WGL_EXT_pbuffer 1
#define WGL_EXT_pixel_format 1
#define WGL_I3D_digital_video_control 1
#define WGL_I3D_gamma 1
#define WGL_I3D_image_buffer 1
#define WGL_I3D_swap_frame_lock 1
#define WGL_I3D_swap_frame_usage 1
#define WGL_NV_copy_image 1
#define WGL_NV_delay_before_swap 1
#define WGL_NV_gpu_affinity 1
#define WGL_NV_present_video 1
#define WGL_NV_vertex_array_range 1
#define WGL_NV_video_capture 1
#define WGL_NV_video_output 1
#define WGL_OML_sync_control 1
#endif
#include "wgl+wglext.h"
#if _avoid_unessessary_include_defines
#undef WGL_3DL_stereo_control 
#undef WGL_EXT_display_color_table 
#undef WGL_EXT_make_current_read 
#undef WGL_EXT_pbuffer 
#undef WGL_EXT_pixel_format 
#undef WGL_I3D_digital_video_control 
#undef WGL_I3D_gamma 
#undef WGL_I3D_image_buffer 
#undef WGL_I3D_swap_frame_lock 
#undef WGL_I3D_swap_frame_usage 
#undef WGL_NV_copy_image 
#undef WGL_NV_delay_before_swap 
#undef WGL_NV_gpu_affinity 
#undef WGL_NV_present_video 
#undef WGL_NV_vertex_array_range 
#undef WGL_NV_video_capture 
#undef WGL_NV_video_output 
#undef WGL_OML_sync_control 
#endif
#include "WGlGetFunctions.h"


//#include <gl/GLU.h>
//#include <gl/GLAUX.H>
//#include <tchar.h>
//#include "gl3w.h"
//#include <gl/glcorearb.h>
//#include <gl/glext.h>
//#include <gl/wgl.h>
//#include <gl/wglext.h>
//#include <gl/glxext.h>
//#include <gl/GLAUX.h>
//#include <gl/GLU.h>
//#include <gl/glx.h>


struct matrix_block {
	mat4     M;
	mat4     V;
	mat4     P;
	mat4     MV;
	mat4     MVP;
};



//#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "../../ImagemEditorTRK3/Output/Win32_Debug/Imagem_003.lib")



#endif //PCH_H

