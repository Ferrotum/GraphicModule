#pragma once
#ifndef OPENGLAUX_H_
#define OPENGLAUX_H_

#define _USE_OPENGL_DESCTOP_
#define _OPENGL_VERSION_ 450
#define _OPENGL_ES_VERSION_ 320

#if defined (__GLEW_H__)
#elif defined(_WIN64) || defined (_WIN32)
#include "OpenGLFunctionAddress.h"
#include "OpenGLConstants.h"

#else
static_assert(false, "====   Can compile for Windows only now.   ====");
#endif // 

#endif // OPENGLAUX_H_