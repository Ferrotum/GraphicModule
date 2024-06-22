#include "GraphicContext.h"
#include "DrawingQueue.h"

#if defined (__GLEW_H__)
#elif defined (_USE_QT_FRAMEWORK_)
#elif defined (__AFX_H__) || defined (_WIN64) || defined (_WIN32) || defined (__LP64__)
#ifdef _M_AMD64
typedef unsigned long long* OpenGL_func_ptr;
typedef unsigned long long OpenGL_func_ptr_ret;
#else
typedef unsigned int* OpenGL_func_ptr;
typedef unsigned int OpenGL_func_ptr_ret;
#endif
static const char* wglFuncName[] {
"wglCreateContext",
"wglDeleteContext",
"wglGetProcAddress",
"wglMakeCurrent",
"wglSwapBuffers",
"wglGetCurrentContext"
};

static void* wglFuncAddress[] {
&_wglCreateContext,
&_wglDeleteContext,
&_wglGetProcAddress,
&_wglMakeCurrent,
&_wglSwapBuffers,
&_wglGetCurrentContext
};

static constexpr int WGL_FUNC_SIZE = sizeof(wglFuncName) / sizeof(wglFuncName[0]);

static const char* funcNames[] {
#if !defined(_USE_OPENGL_ES_)
"glGetDoublev",
#endif // _USE_OPENGL_ES_
"glBlendFunc",
"glClear",
"glCullFace",
"glDeleteTextures",
"glDrawArrays",
"glDrawElements",
"glEnable",
"glFinish",
"glFrontFace",
"glGetString",
"glLineWidth",
"glPointSize",
"glViewport",
"glPolygonMode",
"glGetIntegerv",
"glGetError",
"glGetFloatv",
"glGetBooleanv",
"glDisable",
"glDepthMask",
"glGenTextures",
"glBindTexture",
"glTexImage2D",
"glTexSubImage2D",
"glDepthFunc",
#if !defined(_USE_OPENGL_ES_)
"glBindTextureUnit",
"glBlitNamedFramebuffer",
"glClearNamedFramebufferfv",
"glClearNamedFramebufferuiv",
"glClearTexImage",
"glCopyTextureSubImage1D",
"glCopyTextureSubImage2D",
"glCopyTextureSubImage3D",
"glCreateBuffers",
"glCreateFramebuffers",
"glCreateTextures",
"glCreateTransformFeedbacks",
"glCreateVertexArrays",
"glEnableVertexArrayAttrib",
"glGetActiveSubroutineName",
"glGetNamedFramebufferAttachmentParameteriv",
"glGetNamedFramebufferParameteriv",
"glGetNamedRenderbufferParameteriv",
"glGetTextureImage",
"glGetTextureSubImage",
"glMapNamedBufferRange",
"glNamedBufferStorage",
"glNamedFramebufferDrawBuffer",
"glNamedFramebufferDrawBuffers",
"glNamedFramebufferTexture",
"glTextureParameteri",
"glTextureStorage1D",
"glTextureStorage2D",
"glTextureStorage2DMultisample",
"glTextureStorage3D",
"glTextureStorage3DMultisample",
"glTextureSubImage1D",
"glTextureSubImage2D",
"glTextureSubImage3D",
"glUniformSubroutinesuiv",
"glUnmapNamedBuffer",
"glVertexArrayAttribBinding",
"glVertexArrayAttribFormat",
"glVertexArrayAttribIFormat",
"glVertexArrayBindingDivisor",
"glVertexArrayElementBuffer",
"glVertexArrayVertexBuffer",
"glViewportIndexedfv",
"glGetNamedBufferSubData",
"glNamedBufferSubData",
"glCheckNamedFramebufferStatus",
"glGetTextureLevelParameteriv",
"glViewportIndexedf",
"glGetDoublei_v",
"glCreateQueries",
#endif // _USE_OPENGL_ES_
"glAttachShader",
"glBeginTransformFeedback",
"glBindBufferBase",
"glBindFramebuffer",
"glBindImageTexture",
"glBindTransformFeedback",
"glBindVertexArray",
"glBlendFunci",
"glCompileShader",
"glCreateProgram",
"glCreateShader",
"glDeleteBuffers",
"glDeleteFramebuffers",
"glDeleteProgram",
"glDeleteShader",
"glDeleteTransformFeedbacks",
"glDeleteVertexArrays",
"glDisablei",
"glDispatchCompute",
"glDrawArraysInstanced",
"glDrawElementsInstanced",
"glDrawElementsInstancedBaseVertex",
"glEnablei",
"glEndTransformFeedback",
"glGetActiveUniformBlockiv",
"glGetFramebufferParameteriv",
"glGetProgramInfoLog",
"glGetProgramInterfaceiv",
"glGetProgramiv",
"glGetProgramResourceiv",
"glGetProgramResourceName",
"glGetShaderInfoLog",
"glGetShaderiv",
"glGetStringi",
"glLinkProgram",
"glPatchParameteri",
"glPrimitiveRestartIndex",
"glShaderSource",
"glShaderStorageBlockBinding",
"glTexStorage2DMultisample",
"glTexStorage3DMultisample",
"glUniformBlockBinding",
"glUseProgram",
"glVertexAttribDivisor",
"glMapBufferRange",
"glBindBuffer",
"glGenBuffers",
"glUnmapBuffer",
"glBufferStorage",
"glBufferData",
"glGenVertexArrays",
"glBufferSubData",
"glGetBufferSubData",
"glCheckFramebufferStatus",
"glMemoryBarrier",
"glMemoryBarrierByRegion",
"glDeleteQueries",
"glBeginQuery",
"glEndQuery",
"glGetQueryObjecti64v",
"glGetRenderbufferParameteriv",
"glGetInteger64v",
"glGetIntegeri_v",
"glGetFloati_v",
"glGetInteger64i_v",
"glBindVertexBuffer",
"glVertexBindingDivisor",
"glEnableVertexAttribArray",
"glVertexAttribBinding",
"glVertexAttribFormat",
"glBindBufferRange",
"glGenFramebuffers",
"glFramebufferTexture",
"glDrawBuffer",
"glTexStorage2D",
"glClearBufferfv",
"glBlitFramebuffer",
"glReadBuffer",
"glReadPixels",
"glReadnPixels"
};

static void* funcAddress[] {
#if !defined(_USE_OPENGL_ES_)
&glGetDoublev,
#endif // _USE_OPENGL_ES_
& glBlendFunc,
& glClear,
& glCullFace,
& glDeleteTextures,
& glDrawArrays,
& glDrawElements,
& glEnable,
& glFinish,
& glFrontFace,
& glGetString,
& glLineWidth,
& glPointSize,
& glViewport,
& glPolygonMode,
& glGetIntegerv,
& glGetError,
& glGetFloatv,
& glGetBooleanv,
& glDisable,
& glDepthMask,
& glGenTextures,
& glBindTexture,
& glTexImage2D,
& glTexSubImage2D,
&glDepthFunc,
#if !defined(_USE_OPENGL_ES_)
&glBindTextureUnit,
&glBlitNamedFramebuffer,
&glClearNamedFramebufferfv,
&glClearNamedFramebufferuiv,
&glClearTexImage,
&glCopyTextureSubImage1D,
&glCopyTextureSubImage2D,
&glCopyTextureSubImage3D,
&glCreateBuffers,
&glCreateFramebuffers,
&glCreateTextures,
&glCreateTransformFeedbacks,
&glCreateVertexArrays,
&glEnableVertexArrayAttrib,
&glGetActiveSubroutineName,
&glGetNamedFramebufferAttachmentParameteriv,
&glGetNamedFramebufferParameteriv,
&glGetNamedRenderbufferParameteriv,
&glGetTextureImage,
&glGetTextureSubImage,
&glMapNamedBufferRange,
&glNamedBufferStorage,
&glNamedFramebufferDrawBuffer,
&glNamedFramebufferDrawBuffers,
&glNamedFramebufferTexture,
&glTextureParameteri,
&glTextureStorage1D,
&glTextureStorage2D,
&glTextureStorage2DMultisample,
&glTextureStorage3D,
&glTextureStorage3DMultisample,
&glTextureSubImage1D,
&glTextureSubImage2D,
&glTextureSubImage3D,
&glUniformSubroutinesuiv,
&glUnmapNamedBuffer,
&glVertexArrayAttribBinding,
&glVertexArrayAttribFormat,
&glVertexArrayAttribIFormat,
&glVertexArrayBindingDivisor,
&glVertexArrayElementBuffer,
&glVertexArrayVertexBuffer,
&glViewportIndexedfv,
&glGetNamedBufferSubData,
&glNamedBufferSubData,
&glCheckNamedFramebufferStatus,
&glGetTextureLevelParameteriv,
&glViewportIndexedf,
&glGetDoublei_v,
&glCreateQueries,
#endif // _USE_OPENGL_ES_
&glAttachShader,
&glBeginTransformFeedback,
&glBindBufferBase,
&glBindFramebuffer,
&glBindImageTexture,
&glBindTransformFeedback,
&glBindVertexArray,
&glBlendFunci,
&glCompileShader,
&glCreateProgram,
&glCreateShader,
&glDeleteBuffers,
&glDeleteFramebuffers,
&glDeleteProgram,
&glDeleteShader,
&glDeleteTransformFeedbacks,
&glDeleteVertexArrays,
&glDisablei,
&glDispatchCompute,
&glDrawArraysInstanced,
&glDrawElementsInstanced,
&glDrawElementsInstancedBaseVertex,
&glEnablei,
&glEndTransformFeedback,
&glGetActiveUniformBlockiv,
&glGetFramebufferParameteriv,
&glGetProgramInfoLog,
&glGetProgramInterfaceiv,
&glGetProgramiv,
&glGetProgramResourceiv,
&glGetProgramResourceName,
&glGetShaderInfoLog,
&glGetShaderiv,
&glGetStringi,
&glLinkProgram,
&glPatchParameteri,
&glPrimitiveRestartIndex,
&glShaderSource,
&glShaderStorageBlockBinding,
&glTexStorage2DMultisample,
&glTexStorage3DMultisample,
&glUniformBlockBinding,
&glUseProgram,
&glVertexAttribDivisor,
&glMapBufferRange,
&glBindBuffer,
&glGenBuffers,
&glUnmapBuffer,
&glBufferStorage,
&glBufferData,
&glGenVertexArrays,
&glBufferSubData,
&glGetBufferSubData,
&glCheckFramebufferStatus,
&glMemoryBarrier,
&glMemoryBarrierByRegion,
&glDeleteQueries,
&glBeginQuery,
&glEndQuery,
&glGetQueryObjecti64v,
&glGetRenderbufferParameteriv,
&glGetInteger64v,
&glGetIntegeri_v,
&glGetFloati_v,
&glGetInteger64i_v,
&glBindVertexBuffer,
&glVertexBindingDivisor,
&glEnableVertexAttribArray,
&glVertexAttribBinding,
&glVertexAttribFormat,
&glBindBufferRange,
&glGenFramebuffers,
&glFramebufferTexture,
&glDrawBuffer,
&glTexStorage2D,
&glClearBufferfv,
&glBlitFramebuffer,
&glReadBuffer,
&glReadPixels,
&glReadnPixels
};

static constexpr int FUNC_NAMES_SIZE = sizeof(funcNames) / sizeof(funcNames[0]);

void GraphicContext::_initClass()
{
	const wchar_t libName[]{ L"opengl32.dll" };
	_ogl32dll = LoadLibraryW(libName);
	if (!_ogl32dll) {
		_initError.store(true, std::memory_order_release);
		return;
	}
	for (int i = 0; i < WGL_FUNC_SIZE; ++i) {
		*(OpenGL_func_ptr)wglFuncAddress[i] = (OpenGL_func_ptr_ret)GetProcAddress(_ogl32dll, wglFuncName[i]);
		if (!*(OpenGL_func_ptr)wglFuncAddress[i]) {
			_errorInitClass = std::string("WGL function loading error. ") + std::string(wglFuncName[i]);
			_initError.store(true, std::memory_order_release);
			return;
		}
	}
	_libraryIsLoaded.store(true, std::memory_order_release);
}

//==================================================================================

void GraphicContext::_initFunctionAddresses()
{
	for (int i = 0; i < FUNC_NAMES_SIZE; ++i) {
		*(OpenGL_func_ptr)funcAddress[i] = (OpenGL_func_ptr_ret)_wglGetProcAddress(funcNames[i]);
		if (*(OpenGL_func_ptr)funcAddress[i] == 0 ||
			*(OpenGL_func_ptr)funcAddress[i] == 1 ||
			*(OpenGL_func_ptr)funcAddress[i] == 2 ||
			*(OpenGL_func_ptr)funcAddress[i] == 3 ||
			*(OpenGL_func_ptr)funcAddress[i] == -1 ) {
			*(OpenGL_func_ptr)funcAddress[i] = (OpenGL_func_ptr_ret)GetProcAddress(_ogl32dll, funcNames[i]);
			if (*(OpenGL_func_ptr)funcAddress[i] == 0 || *(OpenGL_func_ptr)funcAddress[i] == 1) {
				_errorGetFunctions = std::string("OpenGL function loading error. ") + std::string(funcNames[i]);
				_initError.store(true, std::memory_order_release);
				return;
			}
		}
	}
	_funcsAddressGet.store(true, std::memory_order_release);
}

#else
void GraphicContext::_initClass()
{

}
#endif // #if defined (_WIN64) || (_WIN32)

thread_local std::string  GraphicContext::_errorInitClass;
thread_local Id<GraphicContext>::idType GraphicContext::_currentContext = Id<GraphicContext>::ID_TYPE_MAX;
thread_local std::string GraphicContext::_errorGetFunctions;
std::weak_ptr<GraphicContext> GraphicContext::_defaultGraphicContext;
std::mutex GraphicContext::mt;

#if defined (_USE_QT_FRAMEWORK_)
#elif defined (_WIN64) | defined (_WIN32)
HMODULE GraphicContext::_ogl32dll;
std::atomic<bool> GraphicContext::_libraryIsLoaded = false;
std::atomic<bool> GraphicContext::_funcsAddressGet = false;
std::atomic<bool> GraphicContext::_initError = false;
#endif

void GraphicContext::_moveProc(GraphicContext& a) noexcept
{
	_errorMessage = std::move(a._errorMessage);
#if defined (_USE_QT_FRAMEWORK_)
	_widget = a._widget;
	_widget = nullptr;
	_funcs = a._funcs;
	_funcs = nullptr;
#elif defined (__AFX_H__)
	_wnd = a._wnd;
	a._wnd = nullptr;
	_cdc = a._cdc;
	a._cdc = nullptr;
	_pixelFormat = a._pixelFormat;
#elif defined (_WIN64) | defined (_WIN32)
	_hwnd = a._hwnd;
	_hdc = a._hdc;
	_pixelFormat = a._pixelFormat;

#endif
	_id = std::move(a._id);
	_isOK = a._isOK;
	_context = a._context;
	a._context = nullptr;
//#if defined (_USE_OPENGL_ES_)
#define _GRAPHIC_CONTEXT_MOVE_(_a) _a = a._a; a._a = Id<Buffer>::ID_TYPE_MAX;
	_GRAPHIC_CONTEXT_MOVE_(_boundArrayBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundAtomicCounterBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundCopyReadBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundCopyWriteBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundDispatchIndirectBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundDrawIndirectBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundElementArrayBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundPixelPackBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundPixelUnpackBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundShaderStorageBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundTextureBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundTransformFeedbackBuffer);
	_GRAPHIC_CONTEXT_MOVE_(_boundUniformBuffer);
//#endif // _USE_OPENGL_ES_
	_boundShaderStorageBuffers = std::move(a._boundShaderStorageBuffers);
	_activeProgram = a._activeProgram;
	a._activeProgram = Id<ShaderProgram>::ID_TYPE_MAX;
	_activeVertexArrayObject = a._activeVertexArrayObject;
	a._activeVertexArrayObject = Id<VertexArrayObject>::ID_TYPE_MAX;
	_boundUniformBlocks = std::move(a._boundUniformBlocks);
	_activeTextures = std::move(a._activeTextures);
	_activeImages = std::move(a._activeImages);
	_queues = std::move(a._queues);
	_mainQueue = std::move(a._mainQueue);
}

//==================================================================================

void GraphicContext::_initInstance() noexcept
{
	int num = 0;
	glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &num);
	_boundShaderStorageBuffers.clear();
	_boundShaderStorageBuffers.resize(num, Id<Buffer>::ID_TYPE_MAX);
	num = 0;
	glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &num);
	_boundUniformBlocks.clear();
	_boundUniformBlocks.resize(num, Id<Buffer>::ID_TYPE_MAX);
	_activeTextures.clear();
	_activeTextures.resize(max_combined_texture_image_units, Id<Buffer>::ID_TYPE_MAX);
	_activeImages.clear();
	_activeImages.resize(max_combined_texture_image_units, BoundImage());
}

//==================================================================================

void GraphicContext::_setCurrentWithoutCheck() const noexcept
{
	if (_context != nullptr) {
		if (_currentContext != _id.id()) {
			_currentContext = _id.id();
#if defined (_USE_QT_FRAMEWORK_)
			_widget->makeCurrent();
#elif defined (__AFX_H__)
			_wglMakeCurrent(_cdc->m_hDC, _context);
#elif defined (_WIN64) | defined (_WIN32)
			_wglMakeCurrent(_hdc, _context);
#endif
		}
	}
}

//==================================================================================

GraphicContext::GraphicContext() :
#if defined (QOBJECT_H)
	_context(nullptr),
	_funcs(nullptr),
#elif defined (__AFX_H__)
	_context(nullptr),
#elif defined (_WIN64) | defined (_WIN32)
	_hwnd(nullptr),
	_hdc(nullptr),
	_context(nullptr),
#else
#endif
//#if defined (_USE_OPENGL_ES_)
	_boundArrayBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundAtomicCounterBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundCopyReadBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundCopyWriteBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundDispatchIndirectBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundDrawIndirectBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundElementArrayBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundPixelPackBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundPixelUnpackBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundShaderStorageBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundTextureBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundTransformFeedbackBuffer(Id<Buffer>::ID_TYPE_MAX),
	_boundUniformBuffer(Id<Buffer>::ID_TYPE_MAX),
//#endif // _USE_OPENGL_ES_
	_isOK(false),
	_activeProgram(Id<ShaderProgram>::ID_TYPE_MAX),
	_activeVertexArrayObject(Id<VertexArrayObject>::ID_TYPE_MAX)
{
	if (!_libraryIsLoaded.load(std::memory_order_acquire)) {
		std::lock_guard<std::mutex> lk(mt);
		if (!_libraryIsLoaded.load(std::memory_order_acquire)) {
			_initClass();
		}
	}
	_threadId = std::this_thread::get_id();
}

//==================================================================================

#if defined (QOBJECT_H)
bool GraphicContext::init(QOpenGLWidget* widget) noexcept
{
	if (_isOK) {
		return false;
	}
	if (widget == nullptr) {
		return false;
	}
	_widget = widget;
	_context = _widget->context();
	_funcs = _context->extraFunctions();
	GraphicContextParameters::init();
	Tesselator::init();
	Rasterizer::init();
	_isOK = true;
	{
		std::lock_guard<std::mutex> lk(mt);
		if (_defaultGraphicContext.expired()) {
			setDefault();
		}
	}
	_initInstance();
	_mainQueue = std::shared_ptr<DrawingQueue<int> >(new DrawingQueue<int>(this));
	_mainQueue->setImmediately(true);
	_queues.emplace(_mainQueue->id(), _mainQueue);
	return true;
}
#elif defined (__AFX_H__)
bool GraphicContext::init(CWnd* wnd) noexcept
{
	if (_isOK) {
		return false;
	}
	if (wnd == nullptr) {
		return false;
	}
	_wnd = wnd;
	_cdc = _wnd->GetDC();
	PIXELFORMATDESCRIPTOR pfd{ 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	_pixelFormat = ChoosePixelFormat(_hdc, &pfd);
	if (!_pixelFormat || !SetPixelFormat(_hdc, _pixelFormat, &pfd)) {
		_errorMessage = std::string("Pixel format cannot be chosen.");
		return false;
	}
	_context = _wglCreateContext(_hdc);
	if (_context == nullptr) {
		_errorMessage = std::string("Graphic context hasn't been created.");
		return false;
	}
	_setCurrentWithoutCheck();
	if (!_funcsAddressGet.load(std::memory_order_acquire)) {
		std::lock_guard<std::mutex> lk(mt);
		if (!_funcsAddressGet.load(std::memory_order_acquire)) {
			_initFunctionAddresses();
		}
	}
	if (_initError.load(std::memory_order_acquire)) {
		_errorMessage = _errorGetFunctions;
		return false;
	}
	GraphicContextParameters::init();
	Tesselator::init();
	Rasterizer::init();
	_isOK = true;
	{
		std::lock_guard<std::mutex> lk(mt);
		if (_defaultGraphicContext.expired()) {
			setDefault();
		}
	}
	_initInstance();
	_mainQueue = std::shared_ptr<DrawingQueue<int> >(new DrawingQueue<int>(this));
	_mainQueue->setImmediately(true);
	_queues.emplace(_mainQueue->id(), _mainQueue);
	return true;
}
#elif defined (_WIN64) | defined (_WIN32)
bool GraphicContext::init(HWND hwnd) noexcept
{
	if (_isOK || _initError.load(std::memory_order_acquire)) {
		return false;
	}
	//if (!_errorInitClass.empty()) {
	//	_errorMessage = _errorInitClass;
	//	return false;
	//}
	if (hwnd == NULL) {
		return false;
	}
	_hwnd = hwnd;
	_hdc = GetDC(_hwnd);
	if (_hdc == NULL) {
		_errorMessage = "Device context error.";
		return false;
	}
	PIXELFORMATDESCRIPTOR pfd{ 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	_pixelFormat = ChoosePixelFormat(_hdc, &pfd);
	if (!_pixelFormat || !SetPixelFormat(_hdc, _pixelFormat, &pfd)) {
		_errorMessage = std::string("Pixel format cannot be chosen.");
		return false;
	}
	_context = _wglCreateContext(_hdc);
	if (_context == nullptr) {
		_errorMessage = std::string("Graphic context hasn't been created.");
		return false;
	}
	_setCurrentWithoutCheck();
	if (!_funcsAddressGet.load(std::memory_order_acquire)) {
		std::lock_guard<std::mutex> lk(mt);
		if (!_funcsAddressGet.load(std::memory_order_acquire)) {
			_initFunctionAddresses();
		}
	}
	if (_initError.load(std::memory_order_acquire)) {
		_errorMessage = _errorGetFunctions;
		return false;
	}
	GraphicContextParameters::init();
	Tesselator::init();
	Rasterizer::init();
	_isOK = true;
	{
		std::lock_guard<std::mutex> lk(mt);
		if (_defaultGraphicContext.expired()) {
			setDefault();
		}
	}
	_initInstance();
	_mainQueue = std::shared_ptr<DrawingQueue<int> >(new DrawingQueue<int>(this));
	_mainQueue->setImmediately(true);
	_queues.emplace(_mainQueue->id(), _mainQueue);
	return true;
}
#endif

//==================================================================================

inline void GraphicContext::setCurrent() const noexcept
{
	if (!_isOK) {
		return;
	}
#if defined (_DEBUG)
	if (_threadId != std::this_thread::get_id()) {
	}
#endif
	if (_context != nullptr) {
		if (_currentContext != _id.id()) {
			_currentContext = _id.id();
#if defined (_USE_QT_FRAMEWORK_)
			_widget->makeCurrent();
#elif defined (__AFX_H__)
			_wglMakeCurrent(_cdc->m_hDC, _context);
#elif defined (_WIN64) | defined (_WIN32)
			_wglMakeCurrent(_hdc, _context);
#endif
		}
	}
}

//==================================================================================

void GraphicContext::releaseCurrent() const noexcept
{
	if (!_isOK) {
		return;
	}
	if (_context != nullptr) {
		if (_currentContext == _id.id()) {
			_currentContext = Id<GraphicContext>::ID_TYPE_MAX;
#if defined (_USE_QT_FRAMEWORK_)
			_widget->doneCurrent();
#elif defined (__AFX_H__)
			_wglMakeCurrent(_cdc->m_hDC, 0);
#elif defined (_WIN64) | defined (_WIN32)
			_wglMakeCurrent(_hdc, 0);
#endif
		}
	}
}

//==================================================================================

void GraphicContext::setDefault() const noexcept
{
	if (_isOK) {
		_defaultGraphicContext = _thisInstance;
	}
}

//==================================================================================

void GraphicContext::reset() noexcept
{
	if (!_isOK) {
		return;
	}
#if defined (QOBJECT_H)
	_widget->doneCurrent();
	_widget = nullptr;
	_context = nullptr;
	_funcs = nullptr;
#elif defined (__AFX_H__)
	if (_context != nullptr) {
		if (_currentContext == _id) {
			_wglMakeCurrent(_cdc->m_hDC, nullptr);
			_wglMakeCurrent(nullptr, nullptr);
			_currentContext = Id<GraphicContext>::ID_TYPE_MAX;
		}
		_wglDeleteContext(_context);
		_context = nullptr;
		_wnd->ReleaseDC(_cdc);
		_cdc = nullptr;
		_wnd = nullptr;
		_thisInstance.reset();
	}
#elif defined (_WIN64) | defined (_WIN32)
	if (_context != nullptr) {
		if (_currentContext == _id) {
			_wglMakeCurrent(_hdc, nullptr);
			_wglMakeCurrent(nullptr, nullptr);
			_currentContext = Id<GraphicContext>::ID_TYPE_MAX;
		}
		_wglDeleteContext(_context);
		_context = nullptr;
		ReleaseDC(_hwnd, _hdc);
		_hdc = NULL;
		_hwnd = NULL;
		_thisInstance.reset();
	}
#else

#endif
	_isOK = false;
}

//==================================================================================

bool GraphicContext::attachDrawingQueue(const std::shared_ptr<DrawingQueueBase>& queue)
{
	if (queue == std::shared_ptr<DrawingQueueBase>()) {
		return false;
	}
	return true;
}

//==================================================================================

void GraphicContext::setActiveNoProgram() noexcept
{
	if (!_isOK) {
		return;
	}
	_activeProgram = Id<ShaderProgram>::ID_TYPE_MAX;
	setCurrent();
	glUseProgram(0);
}

//==================================================================================

void GraphicContext::setActiveNoVertexArray() noexcept
{
	if (!_isOK) {
		return;
	}
	_activeVertexArrayObject = Id<VertexArrayObject>::ID_TYPE_MAX;
	setCurrent();
	glBindVertexArray(0);
}

//==================================================================================

GraphicContext::Ptr GraphicContext::getGraphicContextFromCollection(Id<GraphicContext>::idType id)
{
	return GraphicContext::Ptr();
}

//==================================================================================

void GraphicContext::swapBuffers() const noexcept
{
	if (!_isOK) {
		return;
	}
#if defined (QOBJECT_H)
	_widget->swapBuffers();
#elif defined (__AFX_H__)
	SwapBuffers(_cdc->m_hDC);
#elif defined (_WIN64) | defined (_WIN32)
	SwapBuffers(_hdc);
#endif
}

//==================================================================================

GraphicContext::~GraphicContext() noexcept
{
	reset();
	std::lock_guard<std::mutex> lk(mt);
	if (Id<GraphicContext>::numInstances() == 1) {
#if defined (QOBJECT_H)
#elif defined (__AFX_H__)
#elif defined (_WIN64) | defined (_WIN32)
#endif
	}
}
