#pragma once
#ifndef OPENGLFUNCTIONADDRESS_H_
#define OPENGLFUNCTIONADDRESS_H_

#if defined (__GLEW_H__)
#elif defined (_USE_QT_FRAMEWORK_)
#elif defined(_WIN64) || defined (_WIN32)

#include "Windows.h"
#include <cstdint>

// OpenGL types

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;
typedef int_fast64_t GLint64;

// wglFunc

#ifndef _FUNC_CALL_AGR_
#define _FUNC_CALL_AGR_ __stdcall
#endif // _FUNC_CALL_AGR_

typedef void* (_FUNC_CALL_AGR_ *WGLGETPOCADDRESS)(const char*);
typedef HGLRC (_FUNC_CALL_AGR_ *WGLCREATECONTEXT)(HDC);
typedef BOOL (_FUNC_CALL_AGR_ *WGLMAKECURRENT)(HDC, HGLRC);
typedef BOOL (_FUNC_CALL_AGR_ *WGLDELETECONTEXT)(HGLRC);
typedef BOOL (_FUNC_CALL_AGR_ *WGLSWAPBUFFERS)(HDC);
typedef HGLRC(_FUNC_CALL_AGR_ *WGLGETCURRENTCONTEXT)();

// OpenGL 1.1

typedef void (_FUNC_CALL_AGR_ *GLBLENDFUNC)(GLenum sfactor, GLenum dfactor);
typedef void (_FUNC_CALL_AGR_ *GLCLEAR)(GLbitfield mask);
typedef void (_FUNC_CALL_AGR_ *GLCULLFACE)(GLenum mode);
typedef void (_FUNC_CALL_AGR_ *GLDRAWARRAYS)(GLenum mode, GLint first, GLsizei count);
typedef void (_FUNC_CALL_AGR_ *GLDRAWELEMENTS)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void (_FUNC_CALL_AGR_ *GLENABLE)(GLenum cap);
typedef void (_FUNC_CALL_AGR_ *GLFINISH)();
typedef void (_FUNC_CALL_AGR_ *GLFRONTFACE)(GLenum mode);
typedef const GLubyte* (_FUNC_CALL_AGR_ *GLGETSTRING)(GLenum name);
typedef void (_FUNC_CALL_AGR_ *GLLINEWIDTH)(GLfloat width);
typedef void (_FUNC_CALL_AGR_ *GLPOINTSIZE)(GLfloat size);
typedef void (_FUNC_CALL_AGR_ *GLVIEWPORT)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (_FUNC_CALL_AGR_ *GLPOLYGONMODE)(GLenum face, GLenum mode);
typedef void (_FUNC_CALL_AGR_ *GLGETINTEGERV)(GLenum pname, GLint* data);
typedef GLenum(_FUNC_CALL_AGR_ *GLGETERROR)(void);
typedef void (_FUNC_CALL_AGR_ *GLGETFLOATV)(GLenum pname, GLfloat* data);
typedef void (_FUNC_CALL_AGR_ *GLDISABLE)(GLenum cap);
typedef void (_FUNC_CALL_AGR_ *GLBINDBUFFER)(GLenum target, GLuint buffer);
typedef void (_FUNC_CALL_AGR_ *GLGENBUFFERS)(GLsizei n, GLuint* buffers);
typedef GLboolean(_FUNC_CALL_AGR_ *GLUNMAPBUFFER)(GLenum target);
typedef void (_FUNC_CALL_AGR_ *GLBUFFERDATA)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (_FUNC_CALL_AGR_ *GLDEPTHMASK)(GLboolean flag);
typedef void (_FUNC_CALL_AGR_ *GLGENTEXTURES)(GLsizei n, GLuint* textures);
typedef void (_FUNC_CALL_AGR_ *GLBINDTEXTURE)(GLenum target, GLuint texture);
typedef void (_FUNC_CALL_AGR_ *GLTEXIMAGE2D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data);
typedef void (_FUNC_CALL_AGR_ *GLTEXSUBIMAGE2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);
typedef void (_FUNC_CALL_AGR_ *GLDEPTHFUNC)(GLenum func);

// OpenGL > 1.1

typedef const GLubyte* (_FUNC_CALL_AGR_ *GLGETSTRINGI)(GLenum name, GLuint index);
typedef GLboolean(_FUNC_CALL_AGR_ *GLISTEXTURE)(GLuint texture);
typedef GLenum(_FUNC_CALL_AGR_ *GLCHECKFRAMEBUFFERSTATUS)(GLenum target);
typedef GLuint (_FUNC_CALL_AGR_ *GLCREATEPROGRAM)(void);
typedef GLuint (_FUNC_CALL_AGR_ *GLCREATESHADER)(GLenum shaderType);
typedef void (_FUNC_CALL_AGR_ *GLATTACHSHADER)(GLuint program, GLuint shader);
typedef void (_FUNC_CALL_AGR_ *GLBEGINQUERY)(GLenum target, GLuint id);
typedef void (_FUNC_CALL_AGR_ *GLBEGINTRANSFORMFEEDBACK)(GLenum primitiveMode);
typedef void (_FUNC_CALL_AGR_ *GLBINDBUFFERBASE)(GLenum target, GLuint index, GLuint buffer);
typedef void (_FUNC_CALL_AGR_ *GLBINDBUFFERRANGE)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (_FUNC_CALL_AGR_ *GLBINDFRAMEBUFFER)(GLenum target, GLuint framebuffer);
typedef void (_FUNC_CALL_AGR_ *GLBINDIMAGETEXTURE)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (_FUNC_CALL_AGR_ *GLBINDTRANSFORMFEEDBACK)(GLenum target, GLuint id);
typedef void (_FUNC_CALL_AGR_ *GLBINDVERTEXARRAY)(GLuint array);
typedef void (_FUNC_CALL_AGR_ *GLBINDVERTEXBUFFER)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (_FUNC_CALL_AGR_ *GLBLENDFUNCI)(GLuint buf, GLenum sfactor, GLenum dfactor);
typedef void (_FUNC_CALL_AGR_ *GLBUFFERSTORAGE)(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
typedef void (_FUNC_CALL_AGR_ *GLBUFFERSUBDATA)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (_FUNC_CALL_AGR_ *GLCOMPILESHADER)(GLuint shader);
typedef void (_FUNC_CALL_AGR_ *GLDELETEBUFFERS)(GLsizei n, const GLuint* buffers);
typedef void (_FUNC_CALL_AGR_ *GLDELETEFRAMEBUFFERS)(GLsizei n, GLuint* framebuffers);
typedef void (_FUNC_CALL_AGR_ *GLDELETEPROGRAM)(GLuint program);
typedef void (_FUNC_CALL_AGR_ *GLDELETEQUERIES)(GLsizei n, const GLuint* ids);
typedef void (_FUNC_CALL_AGR_ *GLDELETESHADER)(GLuint shader);
typedef void (_FUNC_CALL_AGR_ *GLDELETETEXTURES)(GLsizei n, const GLuint* textures);
typedef void (_FUNC_CALL_AGR_ *GLDELETETRANSFORMFEEDBACKS)(GLsizei n, const GLuint* ids);
typedef void (_FUNC_CALL_AGR_ *GLDELETEVERTEXARRAYS)(GLsizei n, const GLuint* arrays);
typedef void (_FUNC_CALL_AGR_ *GLDISABLEI)(GLenum cap, GLuint index);
typedef void (_FUNC_CALL_AGR_ *GLDISPATCHCOMPUTE)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (_FUNC_CALL_AGR_ *GLDRAWARRAYSINSTANCED)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (_FUNC_CALL_AGR_ *GLDRAWELEMENTSINSTANCED)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
typedef void (_FUNC_CALL_AGR_ *GLDRAWELEMENTSINSTANCEDBASEVERTEX)(GLenum mode, GLsizei count, GLenum type, void* indices, GLsizei instancecount, GLint basevertex);
typedef void (_FUNC_CALL_AGR_ *GLENABLEI)(GLenum cap, GLuint index);
typedef void (_FUNC_CALL_AGR_ *GLENABLEVERTEXATTRIBARRAY)(GLuint index);
typedef void (_FUNC_CALL_AGR_ *GLENDQUERY)(GLenum target);
typedef void (_FUNC_CALL_AGR_ *GLENDTRANSFORMFEEDBACK)(void);
typedef void (_FUNC_CALL_AGR_ *GLGENVERTEXARRAYS)(GLsizei n, GLuint* arrays);
typedef void (_FUNC_CALL_AGR_ *GLGETACTIVEUNIFORMBLOCKIV)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETBOOLEANV)(GLenum pname, GLboolean* data);
typedef void (_FUNC_CALL_AGR_ *GLGETBUFFERSUBDATA)(GLenum target, GLintptr offset, GLsizeiptr size, void* data);
typedef void (_FUNC_CALL_AGR_ *GLGETFLOATI_V)(GLenum target, GLuint index, GLfloat* data);
typedef void (_FUNC_CALL_AGR_ *GLGETFRAMEBUFFERPARAMETERIV)(GLenum target, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETINTEGER64I_V)(GLenum target, GLuint index, GLint64* data);
typedef void (_FUNC_CALL_AGR_ *GLGETINTEGER64V)(GLenum pname, GLint64* data);
typedef void (_FUNC_CALL_AGR_ *GLGETINTEGERI_V)(GLenum target, GLuint index, GLint* data);
typedef void (_FUNC_CALL_AGR_ *GLGETPROGRAMINFOLOG)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (_FUNC_CALL_AGR_ *GLGETPROGRAMINTERFACEIV)(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETPROGRAMIV)(GLuint program, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETPROGRAMRESOURCEIV)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei* length, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETPROGRAMRESOURCENAME)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, char* name);
typedef void (_FUNC_CALL_AGR_ *GLGETQUERYOBJECTI64V)(GLuint id, GLenum pname, GLint64* params);
typedef void (_FUNC_CALL_AGR_ *GLGETRENDERBUFFERPARAMETERIV)(GLenum target, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLGETSHADERINFOLOG)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (_FUNC_CALL_AGR_ *GLGETSHADERIV)(GLuint shader, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_ *GLLINKPROGRAM)(GLuint program);
typedef void (_FUNC_CALL_AGR_ *GLMEMORYBARRIER)(GLbitfield barriers);
typedef void (_FUNC_CALL_AGR_ *GLMEMORYBARRIERBYREGION)(GLbitfield barriers);
typedef void (_FUNC_CALL_AGR_ *GLPATCHPARAMETERI)(GLenum pname, GLint value);
typedef void (_FUNC_CALL_AGR_ *GLPRIMITIVERESTARTINDEX)(GLuint index);
typedef void (_FUNC_CALL_AGR_ *GLSHADERSOURCE)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void (_FUNC_CALL_AGR_ *GLSHADERSTORAGEBLOCKBINDING)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
typedef void (_FUNC_CALL_AGR_ *GLTEXIMAGE3DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (_FUNC_CALL_AGR_ *GLTEXSTORAGE2DMULTISAMPLE)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (_FUNC_CALL_AGR_ *GLUNIFORMBLOCKBINDING)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (_FUNC_CALL_AGR_ *GLUSEPROGRAM)(GLuint program);
typedef void (_FUNC_CALL_AGR_ *GLVERTEXATTRIBBINDING)(GLuint attribindex, GLuint bindingindex);
typedef void (_FUNC_CALL_AGR_ *GLVERTEXATTRIBDIVISOR)(GLuint index, GLuint divisor);
typedef void (_FUNC_CALL_AGR_ *GLVERTEXATTRIBFORMAT)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (_FUNC_CALL_AGR_ *GLVERTEXBINDINGDIVISOR)(GLuint bindingindex, GLuint divisor);
typedef void* (_FUNC_CALL_AGR_ *GLMAPBUFFERRANGE)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (_FUNC_CALL_AGR_ *GLGENFRAMEBUFFERS)(GLsizei n, GLuint* framebuffers);
typedef void (_FUNC_CALL_AGR_ *GLFRAMEBUFFERTEXTURE)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (_FUNC_CALL_AGR_ *GLDRAWBUFFER)(GLenum buf);
typedef void (_FUNC_CALL_AGR_ *GLTEXSTORAGE2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (_FUNC_CALL_AGR_ *GLCLEARBUFFERFV)(GLenum buffer, GLint drawBuffer, const GLfloat* value);
typedef void (_FUNC_CALL_AGR_ *GLBLITFRAMEBUFFER)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (_FUNC_CALL_AGR_ *GLREADBUFFER)(GLenum src);
typedef void (_FUNC_CALL_AGR_ *GLREADPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data);
typedef void (_FUNC_CALL_AGR_ *GLREADNPIXELS)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);
#if !defined(_USE_OPENGL_ES_)
typedef void (_FUNC_CALL_AGR_* GLENABLEVERTEXARRAYATTRIB)(GLuint vaobj, GLuint index);
typedef void (_FUNC_CALL_AGR_* GLCREATEQUERIES)(GLenum target, GLsizei n, GLuint* ids);
typedef void (_FUNC_CALL_AGR_* GLCREATETRANSFORMFEEDBACKS)(GLsizei n, GLuint* ids);
typedef void (_FUNC_CALL_AGR_* GLBINDTEXTUREUNIT)(GLuint unit, GLuint texture);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYBINDINGDIVISOR)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef GLboolean(_FUNC_CALL_AGR_* GLUNMAPNAMEDBUFFER)(GLuint buffer);
typedef GLenum(_FUNC_CALL_AGR_* GLCHECKNAMEDFRAMEBUFFERSTATUS)(GLuint framebuffer, GLenum target);
typedef void (_FUNC_CALL_AGR_ *GLGETDOUBLEI_V)(GLenum target, GLuint index, GLdouble* data);
typedef void (_FUNC_CALL_AGR_ *GLGETDOUBLEV)(GLenum pname, GLdouble* data);
typedef void (_FUNC_CALL_AGR_* GLBLITNAMEDFRAMEBUFFER)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (_FUNC_CALL_AGR_* GLCLEARNAMEDFRAMEBUFFERFV)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value);
typedef void (_FUNC_CALL_AGR_* GLCLEARNAMEDFRAMEBUFFERUIV)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value);
typedef void (_FUNC_CALL_AGR_* GLCLEARTEXIMAGE)(GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
typedef void (_FUNC_CALL_AGR_* GLCOPYTEXTURESUBIMAGE1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (_FUNC_CALL_AGR_* GLCOPYTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (_FUNC_CALL_AGR_* GLCOPYTEXTURESUBIMAGE3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (_FUNC_CALL_AGR_* GLCREATEBUFFERS)(GLsizei n, GLuint* buffers);
typedef void (_FUNC_CALL_AGR_* GLCREATEFRAMEBUFFERS)(GLsizei n, GLuint* framebuffers);
typedef void (_FUNC_CALL_AGR_* GLCREATETEXTURES)(GLenum target, GLsizei n, GLuint* textures);
typedef void (_FUNC_CALL_AGR_* GLCREATEVERTEXARRAYS)(GLsizei n, GLuint* arrays);
typedef void (_FUNC_CALL_AGR_* GLGETACTIVESUBROUTINENAME)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
typedef void (_FUNC_CALL_AGR_* GLGETNAMEDBUFFERSUBDATA)(GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
typedef void (_FUNC_CALL_AGR_* GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_* GLGETNAMEDFRAMEBUFFERPARAMETERIV)(GLuint framebuffer, GLenum pname, GLint* param);
typedef void (_FUNC_CALL_AGR_* GLGETNAMEDRENDERBUFFERPARAMETERIV)(GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_* GLGETTEXTUREIMAGE)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
typedef void (_FUNC_CALL_AGR_* GLGETTEXTURELEVELPARAMETERIV)(GLuint texture, GLint level, GLenum pname, GLint* params);
typedef void (_FUNC_CALL_AGR_* GLGETTEXTURESUBIMAGE)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
typedef void (_FUNC_CALL_AGR_* GLNAMEDBUFFERSTORAGE)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
typedef void (_FUNC_CALL_AGR_* GLNAMEDBUFFERSUBDATA)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (_FUNC_CALL_AGR_* GLNAMEDFRAMEBUFFERDRAWBUFFER)(GLuint framebuffer, GLenum buf);
typedef void (_FUNC_CALL_AGR_* GLNAMEDFRAMEBUFFERDRAWBUFFERS)(GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (_FUNC_CALL_AGR_* GLNAMEDFRAMEBUFFERTEXTURE)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (_FUNC_CALL_AGR_* GLTEXTUREPARAMETERI)(GLuint texture, GLenum pname, GLint param);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESTORAGE1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESTORAGE2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESTORAGE2DMULTISAMPLE)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESTORAGE3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESTORAGE3DMULTISAMPLE)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESUBIMAGE1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESUBIMAGE2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (_FUNC_CALL_AGR_* GLTEXTURESUBIMAGE3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (_FUNC_CALL_AGR_* GLUNIFORMSUBROUTINESUIV)(GLenum shadertype, GLsizei count, const GLuint* indices);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYATTRIBBINDING)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYATTRIBFORMAT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYATTRIBIFORMAT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYELEMENTBUFFER)(GLuint vaobj, GLuint buffer);
typedef void (_FUNC_CALL_AGR_* GLVERTEXARRAYVERTEXBUFFER)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (_FUNC_CALL_AGR_* GLVIEWPORTINDEXEDF)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (_FUNC_CALL_AGR_* GLVIEWPORTINDEXEDFV)(GLuint index, const GLfloat* v);
typedef void* (_FUNC_CALL_AGR_* GLMAPNAMEDBUFFERRANGE)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (_FUNC_CALL_AGR_* GLGENERATETEXTUREMIPMAP)(GLuint texture);
#endif // _USE_OPENGL_ES_

////////////////////////////////////////////////////////

#define _FUNCTION_EXPORT_ extern

_FUNCTION_EXPORT_ WGLCREATECONTEXT _wglCreateContext;
_FUNCTION_EXPORT_ WGLDELETECONTEXT _wglDeleteContext;
_FUNCTION_EXPORT_ WGLGETPOCADDRESS _wglGetProcAddress;
_FUNCTION_EXPORT_ WGLMAKECURRENT _wglMakeCurrent;
_FUNCTION_EXPORT_ WGLSWAPBUFFERS _wglSwapBuffers;
_FUNCTION_EXPORT_ WGLGETCURRENTCONTEXT _wglGetCurrentContext;

_FUNCTION_EXPORT_ GLCLEAR glClear;
_FUNCTION_EXPORT_ GLCULLFACE glCullFace;
_FUNCTION_EXPORT_ GLDRAWARRAYS glDrawArrays;
_FUNCTION_EXPORT_ GLDRAWELEMENTS glDrawElements;
_FUNCTION_EXPORT_ GLENABLE glEnable;
_FUNCTION_EXPORT_ GLFINISH glFinish;
_FUNCTION_EXPORT_ GLFRONTFACE glFrontFace;
_FUNCTION_EXPORT_ GLGETSTRING glGetString;
_FUNCTION_EXPORT_ GLLINEWIDTH glLineWidth;
_FUNCTION_EXPORT_ GLVIEWPORT glViewport;
_FUNCTION_EXPORT_ GLPOINTSIZE glPointSize;
_FUNCTION_EXPORT_ GLBLENDFUNC glBlendFunc;
_FUNCTION_EXPORT_ GLPOLYGONMODE glPolygonMode;
_FUNCTION_EXPORT_ GLGETINTEGERV glGetIntegerv;
_FUNCTION_EXPORT_ GLGETERROR glGetError;
_FUNCTION_EXPORT_ GLGETFLOATV glGetFloatv;
_FUNCTION_EXPORT_ GLGETBOOLEANV glGetBooleanv;
_FUNCTION_EXPORT_ GLDISABLE glDisable;
_FUNCTION_EXPORT_ GLBINDBUFFER glBindBuffer;
_FUNCTION_EXPORT_ GLGENBUFFERS glGenBuffers;
_FUNCTION_EXPORT_ GLUNMAPBUFFER glUnmapBuffer;
_FUNCTION_EXPORT_ GLBUFFERDATA glBufferData;
_FUNCTION_EXPORT_ GLDEPTHMASK glDepthMask;
_FUNCTION_EXPORT_ GLGENTEXTURES glGenTextures;
_FUNCTION_EXPORT_ GLBINDTEXTURE glBindTexture;
_FUNCTION_EXPORT_ GLTEXIMAGE2D glTexImage2D;
_FUNCTION_EXPORT_ GLTEXSUBIMAGE2D glTexSubImage2D;
_FUNCTION_EXPORT_ GLDEPTHFUNC glDepthFunc;

_FUNCTION_EXPORT_ GLATTACHSHADER glAttachShader;
_FUNCTION_EXPORT_ GLBINDBUFFERBASE glBindBufferBase;
_FUNCTION_EXPORT_ GLBINDVERTEXARRAY glBindVertexArray;
_FUNCTION_EXPORT_ GLCOMPILESHADER glCompileShader;
_FUNCTION_EXPORT_ GLCREATEPROGRAM glCreateProgram;
_FUNCTION_EXPORT_ GLCREATESHADER glCreateShader;
_FUNCTION_EXPORT_ GLDELETEBUFFERS glDeleteBuffers;
_FUNCTION_EXPORT_ GLDELETEPROGRAM glDeleteProgram;
_FUNCTION_EXPORT_ GLDELETESHADER glDeleteShader;
_FUNCTION_EXPORT_ GLDELETEVERTEXARRAYS glDeleteVertexArrays;
_FUNCTION_EXPORT_ GLDISPATCHCOMPUTE glDispatchCompute;
_FUNCTION_EXPORT_ GLDRAWARRAYSINSTANCED glDrawArraysInstanced;
_FUNCTION_EXPORT_ GLDRAWELEMENTSINSTANCED glDrawElementsInstanced;
_FUNCTION_EXPORT_ GLGETACTIVEUNIFORMBLOCKIV glGetActiveUniformBlockiv;
_FUNCTION_EXPORT_ GLGETPROGRAMINFOLOG glGetProgramInfoLog;
_FUNCTION_EXPORT_ GLGETPROGRAMINTERFACEIV glGetProgramInterfaceiv;
_FUNCTION_EXPORT_ GLGETPROGRAMIV glGetProgramiv;
_FUNCTION_EXPORT_ GLGETPROGRAMRESOURCEIV glGetProgramResourceiv;
_FUNCTION_EXPORT_ GLGETPROGRAMRESOURCENAME glGetProgramResourceName;
_FUNCTION_EXPORT_ GLGETSHADERINFOLOG glGetShaderInfoLog;
_FUNCTION_EXPORT_ GLGETSHADERIV glGetShaderiv;
_FUNCTION_EXPORT_ GLLINKPROGRAM glLinkProgram;
_FUNCTION_EXPORT_ GLSHADERSOURCE glShaderSource;
_FUNCTION_EXPORT_ GLSHADERSTORAGEBLOCKBINDING glShaderStorageBlockBinding;
_FUNCTION_EXPORT_ GLUNIFORMBLOCKBINDING glUniformBlockBinding;
_FUNCTION_EXPORT_ GLUSEPROGRAM glUseProgram;
_FUNCTION_EXPORT_ GLPATCHPARAMETERI glPatchParameteri;
_FUNCTION_EXPORT_ GLPRIMITIVERESTARTINDEX glPrimitiveRestartIndex;

_FUNCTION_EXPORT_ GLDELETEFRAMEBUFFERS glDeleteFramebuffers;
_FUNCTION_EXPORT_ GLBINDFRAMEBUFFER glBindFramebuffer;
_FUNCTION_EXPORT_ GLGETFRAMEBUFFERPARAMETERIV glGetFramebufferParameteriv;
_FUNCTION_EXPORT_ GLDELETETEXTURES glDeleteTextures;
_FUNCTION_EXPORT_ GLBINDIMAGETEXTURE glBindImageTexture;
_FUNCTION_EXPORT_ GLDRAWELEMENTSINSTANCEDBASEVERTEX glDrawElementsInstancedBaseVertex;
_FUNCTION_EXPORT_ GLBEGINTRANSFORMFEEDBACK glBeginTransformFeedback;
_FUNCTION_EXPORT_ GLENDTRANSFORMFEEDBACK glEndTransformFeedback;
_FUNCTION_EXPORT_ GLBINDTRANSFORMFEEDBACK glBindTransformFeedback;
_FUNCTION_EXPORT_ GLDELETETRANSFORMFEEDBACKS glDeleteTransformFeedbacks;
_FUNCTION_EXPORT_ GLVERTEXATTRIBDIVISOR glVertexAttribDivisor;
_FUNCTION_EXPORT_ GLTEXSTORAGE2DMULTISAMPLE glTexStorage2DMultisample;
_FUNCTION_EXPORT_ GLTEXIMAGE3DMULTISAMPLE glTexStorage3DMultisample;
_FUNCTION_EXPORT_ GLBLENDFUNCI glBlendFunci;
_FUNCTION_EXPORT_ GLENABLEI glEnablei;
_FUNCTION_EXPORT_ GLDISABLEI glDisablei;
_FUNCTION_EXPORT_ GLGETSTRINGI glGetStringi;
_FUNCTION_EXPORT_ GLMAPBUFFERRANGE glMapBufferRange;
_FUNCTION_EXPORT_ GLBUFFERSTORAGE glBufferStorage;
_FUNCTION_EXPORT_ GLGENVERTEXARRAYS glGenVertexArrays;
_FUNCTION_EXPORT_ GLBUFFERSUBDATA glBufferSubData;
_FUNCTION_EXPORT_ GLGETBUFFERSUBDATA glGetBufferSubData;
_FUNCTION_EXPORT_ GLCHECKFRAMEBUFFERSTATUS glCheckFramebufferStatus;
_FUNCTION_EXPORT_ GLMEMORYBARRIER glMemoryBarrier;
_FUNCTION_EXPORT_ GLMEMORYBARRIERBYREGION glMemoryBarrierByRegion;
_FUNCTION_EXPORT_ GLDELETEQUERIES glDeleteQueries;
_FUNCTION_EXPORT_ GLBEGINQUERY glBeginQuery;
_FUNCTION_EXPORT_ GLENDQUERY glEndQuery;
_FUNCTION_EXPORT_ GLGETQUERYOBJECTI64V glGetQueryObjecti64v;
_FUNCTION_EXPORT_ GLISTEXTURE glIsTexture;
_FUNCTION_EXPORT_ GLGETRENDERBUFFERPARAMETERIV glGetRenderbufferParameteriv;
_FUNCTION_EXPORT_ GLGETINTEGER64V glGetInteger64v;
_FUNCTION_EXPORT_ GLGETINTEGERI_V glGetIntegeri_v;
_FUNCTION_EXPORT_ GLGETFLOATI_V glGetFloati_v;
_FUNCTION_EXPORT_ GLGETINTEGER64I_V glGetInteger64i_v;
_FUNCTION_EXPORT_ GLBINDVERTEXBUFFER glBindVertexBuffer;
_FUNCTION_EXPORT_ GLVERTEXBINDINGDIVISOR glVertexBindingDivisor;
_FUNCTION_EXPORT_ GLENABLEVERTEXATTRIBARRAY glEnableVertexAttribArray;
_FUNCTION_EXPORT_ GLVERTEXATTRIBBINDING glVertexAttribBinding;
_FUNCTION_EXPORT_ GLVERTEXATTRIBFORMAT glVertexAttribFormat;
_FUNCTION_EXPORT_ GLBINDBUFFERRANGE glBindBufferRange;
_FUNCTION_EXPORT_ GLGENFRAMEBUFFERS glGenFramebuffers;
_FUNCTION_EXPORT_ GLFRAMEBUFFERTEXTURE glFramebufferTexture;
_FUNCTION_EXPORT_ GLDRAWBUFFER glDrawBuffer;
_FUNCTION_EXPORT_ GLTEXSTORAGE2D glTexStorage2D;
_FUNCTION_EXPORT_ GLCLEARBUFFERFV glClearBufferfv;
_FUNCTION_EXPORT_ GLBLITFRAMEBUFFER glBlitFramebuffer;
_FUNCTION_EXPORT_ GLREADBUFFER glReadBuffer;
_FUNCTION_EXPORT_ GLREADPIXELS glReadPixels;
_FUNCTION_EXPORT_ GLREADNPIXELS glReadnPixels;
#if !defined(_USE_OPENGL_ES_)
_FUNCTION_EXPORT_ GLCREATEQUERIES glCreateQueries;
_FUNCTION_EXPORT_ GLCREATETRANSFORMFEEDBACKS glCreateTransformFeedbacks;
_FUNCTION_EXPORT_ GLUNIFORMSUBROUTINESUIV glUniformSubroutinesuiv;
_FUNCTION_EXPORT_ GLUNMAPNAMEDBUFFER glUnmapNamedBuffer;
_FUNCTION_EXPORT_ GLVERTEXARRAYATTRIBBINDING glVertexArrayAttribBinding;
_FUNCTION_EXPORT_ GLVERTEXARRAYATTRIBFORMAT glVertexArrayAttribFormat;
_FUNCTION_EXPORT_ GLVERTEXARRAYATTRIBIFORMAT glVertexArrayAttribIFormat;
_FUNCTION_EXPORT_ GLVERTEXARRAYELEMENTBUFFER glVertexArrayElementBuffer;
_FUNCTION_EXPORT_ GLVERTEXARRAYVERTEXBUFFER glVertexArrayVertexBuffer;
_FUNCTION_EXPORT_ GLENABLEVERTEXARRAYATTRIB glEnableVertexArrayAttrib;
_FUNCTION_EXPORT_ GLGETACTIVESUBROUTINENAME glGetActiveSubroutineName;
_FUNCTION_EXPORT_ GLCREATEFRAMEBUFFERS glCreateFramebuffers;
_FUNCTION_EXPORT_ GLMAPNAMEDBUFFERRANGE glMapNamedBufferRange;
_FUNCTION_EXPORT_ GLNAMEDBUFFERSTORAGE glNamedBufferStorage;
_FUNCTION_EXPORT_ GLNAMEDFRAMEBUFFERTEXTURE glNamedFramebufferTexture;
_FUNCTION_EXPORT_ GLNAMEDFRAMEBUFFERDRAWBUFFER glNamedFramebufferDrawBuffer;
_FUNCTION_EXPORT_ GLBLITNAMEDFRAMEBUFFER glBlitNamedFramebuffer;
_FUNCTION_EXPORT_ GLGETNAMEDFRAMEBUFFERPARAMETERIV glGetNamedFramebufferParameteriv;
_FUNCTION_EXPORT_ GLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV glGetNamedFramebufferAttachmentParameteriv;
_FUNCTION_EXPORT_ GLGETNAMEDRENDERBUFFERPARAMETERIV glGetNamedRenderbufferParameteriv;
_FUNCTION_EXPORT_ GLCREATETEXTURES glCreateTextures;
_FUNCTION_EXPORT_ GLTEXTURESTORAGE1D glTextureStorage1D;
_FUNCTION_EXPORT_ GLTEXTURESTORAGE2D glTextureStorage2D;
_FUNCTION_EXPORT_ GLTEXTURESTORAGE3D glTextureStorage3D;
_FUNCTION_EXPORT_ GLTEXTURESUBIMAGE1D glTextureSubImage1D;
_FUNCTION_EXPORT_ GLTEXTURESUBIMAGE2D glTextureSubImage2D;
_FUNCTION_EXPORT_ GLTEXTURESUBIMAGE3D glTextureSubImage3D;
_FUNCTION_EXPORT_ GLTEXTUREPARAMETERI glTextureParameteri;
_FUNCTION_EXPORT_ GLCREATEVERTEXARRAYS glCreateVertexArrays;
_FUNCTION_EXPORT_ GLBINDTEXTUREUNIT glBindTextureUnit;
_FUNCTION_EXPORT_ GLCOPYTEXTURESUBIMAGE1D glCopyTextureSubImage1D;
_FUNCTION_EXPORT_ GLCOPYTEXTURESUBIMAGE2D glCopyTextureSubImage2D;
_FUNCTION_EXPORT_ GLCOPYTEXTURESUBIMAGE3D glCopyTextureSubImage3D;
_FUNCTION_EXPORT_ GLGETTEXTUREIMAGE glGetTextureImage;
_FUNCTION_EXPORT_ GLTEXTURESTORAGE2DMULTISAMPLE glTextureStorage2DMultisample;
_FUNCTION_EXPORT_ GLVERTEXARRAYBINDINGDIVISOR glVertexArrayBindingDivisor;
_FUNCTION_EXPORT_ GLTEXTURESTORAGE3DMULTISAMPLE glTextureStorage3DMultisample;
_FUNCTION_EXPORT_ GLGETNAMEDBUFFERSUBDATA glGetNamedBufferSubData;
_FUNCTION_EXPORT_ GLNAMEDBUFFERSUBDATA glNamedBufferSubData;
_FUNCTION_EXPORT_ GLCREATEBUFFERS glCreateBuffers;
_FUNCTION_EXPORT_ GLCLEARNAMEDFRAMEBUFFERFV glClearNamedFramebufferfv;
_FUNCTION_EXPORT_ GLCLEARNAMEDFRAMEBUFFERUIV glClearNamedFramebufferuiv;
_FUNCTION_EXPORT_ GLCHECKNAMEDFRAMEBUFFERSTATUS glCheckNamedFramebufferStatus;
_FUNCTION_EXPORT_ GLCLEARTEXIMAGE glClearTexImage;
_FUNCTION_EXPORT_ GLNAMEDFRAMEBUFFERDRAWBUFFERS glNamedFramebufferDrawBuffers;
_FUNCTION_EXPORT_ GLGETTEXTURESUBIMAGE glGetTextureSubImage;
_FUNCTION_EXPORT_ GLGENERATETEXTUREMIPMAP glGenerateTextureMipmap;
_FUNCTION_EXPORT_ GLVIEWPORTINDEXEDFV glViewportIndexedfv;
_FUNCTION_EXPORT_ GLGETDOUBLEV glGetDoublev;
_FUNCTION_EXPORT_ GLGETTEXTURELEVELPARAMETERIV glGetTextureLevelParameteriv;
_FUNCTION_EXPORT_ GLVIEWPORTINDEXEDF glViewportIndexedf;
_FUNCTION_EXPORT_ GLGETDOUBLEI_V glGetDoublei_v;
#endif // _USE_OPENGL_ES_

#undef _FUNCTION_EXPORT_

#endif //

#endif // OPENGLFUNCTIONADR_H_