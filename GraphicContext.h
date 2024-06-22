#pragma once
#ifndef _GRAPHICCONTEXT_H_
#define _GRAPHICCONTEXT_H_

#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#if defined (_USE_MFC_FRAMEWORK_)
#elif defined (_USE_QT_FRAMEWORK_)
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#endif

#include "Object.h"
#include "OpenGLAux.h"
#include "Rasterizer.h"
#include "Tesselator.h"
#include "Id.h"
#include "GraphicContextParameters.h"
#include "AuxDefinitions.h"

typedef float GraphicContextParametersType;

class DrawingQueueBase;
class Buffer;
class ShaderProgram;
class VertexArrayObject;
class UniformBlock;
class Surface;
class Texture;

class GraphicContext : public Object, protected GraphicContextParameters<GraphicContextParametersType>, public Rasterizer, public Tesselator
{
private:
	friend class Buffer;
	friend class ShaderProgram;
	friend class VertexArrayObject;
	friend class UniformBlock;
	friend class Surface;
	friend class Texture;
public:
	using Ptr =  GraphicContext*;
protected:
//#if defined (_USE_OPENGL_ES_)
	Id<Buffer>::idType _boundArrayBuffer;
	Id<Buffer>::idType _boundAtomicCounterBuffer;
	Id<Buffer>::idType _boundCopyReadBuffer;
	Id<Buffer>::idType _boundCopyWriteBuffer;
	Id<Buffer>::idType _boundDispatchIndirectBuffer;
	Id<Buffer>::idType _boundDrawIndirectBuffer;
	Id<Buffer>::idType _boundElementArrayBuffer;
	Id<Buffer>::idType _boundPixelPackBuffer;
	Id<Buffer>::idType _boundPixelUnpackBuffer;
	Id<Buffer>::idType _boundShaderStorageBuffer;
	Id<Buffer>::idType _boundTextureBuffer;
	Id<Buffer>::idType _boundTransformFeedbackBuffer;
	Id<Buffer>::idType _boundUniformBuffer;

	Id<Texture>::idType _boundTexture2D;
	Id<Texture>::idType _boundTexture2DArray;
	Id<Texture>::idType _boundTexture2DMultisample;
	Id<Texture>::idType _boundTexture2DMultisampleArray;
	Id<Texture>::idType _boundTexture3D;
//#endif // _USE_OPENGL_ES_

	std::vector<Id<Buffer>::idType> _boundShaderStorageBuffers;
	Id<ShaderProgram>::idType _activeProgram;
	Id<VertexArrayObject>::idType _activeVertexArrayObject;
	std::vector<Id<Buffer>::idType> _boundUniformBlocks;
	std::vector<Id<Texture>::idType> _activeTextures;
	struct BoundImage
	{
		Id<Texture>::idType id;
		int level;
		int layer;
		int access;
		BoundImage() noexcept : id(Id<Buffer>::ID_TYPE_MAX), level(-1), layer(-1), access(0) {}
		BoundImage(Id<Texture>::idType _id, int _level, int _layer, int _access) noexcept : id(_id), level(_level), layer(_layer), access(_access) {}
	};
	std::vector<BoundImage> _activeImages;
	std::unordered_map<Id<DrawingQueueBase>::idType, std::shared_ptr<DrawingQueueBase>> _queues;
	std::shared_ptr<DrawingQueueBase> _mainQueue;
	std::string _errorMessage;
	bool _isOK;
#if defined (QOBJECT_H)
	QOpenGLWidget* _widget;
	QOpenGLContext* _context;
	QOpenGLExtraFunctions* _funcs;
#elif defined (__AFX_H__)
	static HMODULE _ogl32dll;
	CWnd* _wnd;
	CDC* _cdc;
	HGLRC _context;
	int _pixelFormat;
	static std::atomic<bool> _libraryIsLoaded;
	static std::atomic<bool> _funcsAddressGet;
	static std::atomic<bool> _initError;
#elif defined (_WIN64) | defined (_WIN32)
	static HMODULE _ogl32dll;
	HWND _hwnd;
	HDC _hdc;
	HGLRC _context;
	int _pixelFormat;
	static std::atomic<bool> _libraryIsLoaded;
	static std::atomic<bool> _funcsAddressGet;
	static std::atomic<bool> _initError;
#endif
	std::thread::id _threadId;
	Id<GraphicContext> _id;
	std::shared_ptr<GraphicContext> _thisInstance;
	static thread_local Id<GraphicContext>::idType _currentContext;
	static std::mutex mt;

	static thread_local std::string _errorInitClass;
	static thread_local std::string _errorGetFunctions;
	static std::weak_ptr<GraphicContext> _defaultGraphicContext;
	static std::unordered_map<Id<GraphicContext>, std::weak_ptr<GraphicContext>> _collection;

private:
#if defined (_WIN64) | defined (_WIN32)
	static void _initClass();
	static void _initFunctionAddresses();
#endif
	void _moveProc(GraphicContext& a) noexcept;
	void _initInstance() noexcept;
	void _setCurrentWithoutCheck() const noexcept;
public:
	struct ContextType
	{
		enum { OPENGL };
	};
protected:
	GraphicContext();
public:
	GraphicContext(const GraphicContext&) = delete;
	GraphicContext(GraphicContext&& a) noexcept :
		Object(std::move(a)),
		GraphicContextParameters(std::move(a)),
		Rasterizer(std::move(a)),
		Tesselator(std::move(a))
	{
		_moveProc(a);
	}
	GraphicContext& operator=(const GraphicContext&) = delete;
	GraphicContext& operator=(GraphicContext&& a) noexcept
	{
		if (this != &a) {
			Object::operator=(std::move(a));
			GraphicContextParameters::operator=(std::move(a));
			Rasterizer::operator=(std::move(a));
			Tesselator::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
#if defined (QOBJECT_H)
	bool init(QOpenGLWidget* widget) noexcept;
#elif defined (__AFX_H__)
	bool init(CWnd* wnd) noexcept;
#elif defined (_WIN64) | defined (_WIN32)
	bool init(HWND hwnd) noexcept;
#endif
	static std::shared_ptr<GraphicContext> sharedNew()
	{
		std::shared_ptr<GraphicContext> temp(new GraphicContext);
		temp->_thisInstance = temp;
		return temp;
	}
	const std::string& getError() const noexcept
	{
		return _errorMessage;
	}
	void setCurrent() const noexcept;
	void releaseCurrent() const noexcept;
	void setDefault() const noexcept;
	const GraphicContextParameters<GraphicContextParametersType>* parameters() const noexcept
	{
		return this;
	}
	virtual operator bool() const noexcept
	{
		return _isOK;
	}
	static std::shared_ptr<GraphicContext> getDefault() noexcept
	{
		return _defaultGraphicContext.lock();
	}
	void reset() noexcept;
	std::shared_ptr<DrawingQueueBase> defaultDrawingQueue() const noexcept
	{
		return _mainQueue;
	}
	bool attachDrawingQueue(const std::shared_ptr<DrawingQueueBase>& queue);
	void setActiveNoProgram() noexcept;
	void setActiveNoVertexArray() noexcept;
	bool isOriginThread() const noexcept
	{
		return _threadId == std::this_thread::get_id();
	}
	static GraphicContext::Ptr getGraphicContextFromCollection(Id<GraphicContext>::idType id);
	void swapBuffers() const noexcept;
	virtual ~GraphicContext() noexcept;
};

#endif // _GRAPHICCONTEXT_H_