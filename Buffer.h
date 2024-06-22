#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <mutex>
#include <unordered_map>
#include <vector>

#include "BufferException.h"
#include "GraphicContext.h"
#include "Id.h"
#include "Object.h"
#include "OpenGLAux.h"

class Buffer : public Object
{
public:
	struct Flags {
		enum { STATIC, DYNAMIC_STORAGE = 1, MAP_READ = 1 << 1, MAP_WRITE = 1 << 2, MAP_PERSISTENT = 1 << 3, MAP_COHERENT = 1 << 4 };
	};
	using idType = Id<Buffer>::idType;
protected:
	static std::mutex _collectionMutex;
	static std::unordered_map<Id<Buffer>::idType, std::weak_ptr<Buffer> > _collection;
	GraphicContext::Ptr _context;

	static std::atomic<bool> _onceInit;
	static unsigned int _counter;
	static std::mutex _mt;

	GLuint _hBuffer;
	unsigned int _size;
	unsigned int _flags;
#if defined (_USE_OPENGL_ES_)
	GLenum _usingMode;
#else
	GLbitfield _glFlags;
#endif // _USE_OPENGL_ES_
	Id<Buffer> _id;

	void _deleteBuffer() noexcept;
	void _moveProc(Buffer& a) noexcept;
	void _create(GraphicContext::Ptr context, unsigned int size, unsigned int flags, const void* data);
	static void _initClass() noexcept;

	static void _eraseFromCollection(Buffer* ptr);
	static bool _emplaceToCollection(const std::shared_ptr<Buffer>& ptr);
public:
	Buffer() noexcept;
	Buffer(unsigned int size, unsigned int flags, const void* data = 0);
	Buffer(GraphicContext::Ptr context, unsigned int size, unsigned int flags, const void* data = 0);
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&& a) noexcept :
		Object(std::move(a)),
		_hBuffer(0),
		_size(0),
		_flags(0)
	{
		_moveProc(a);
	}
	Buffer& operator=(const Buffer&) = delete;
	Buffer& operator=(Buffer&& a) noexcept
	{
		if (this != &a) {
			Object::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	std::shared_ptr<Buffer> sharedNew()
	{
		std::shared_ptr<Buffer> retValue(new Buffer, Buffer::_eraseFromCollection);
		_emplaceToCollection(retValue);
		return retValue;
	}
	std::shared_ptr<Buffer> sharedNew(unsigned int size, unsigned int flags, const void* data = 0)
	{
		std::shared_ptr<Buffer> retValue(new Buffer(size, flags, data), Buffer::_eraseFromCollection);
		_emplaceToCollection(retValue);
		return retValue;
	}
	unsigned int handle() const noexcept
	{
		return _hBuffer;
	}
	unsigned int size() const noexcept
	{
		return _size;
	}
	unsigned int flags() const noexcept
	{
		return _flags;
	}
	bool getSubData(size_t offset, size_t size, void* ptr) const;
	bool storeSubData(size_t offset, size_t size, const void* ptr) const;
	Id<Buffer>::idType id() const noexcept
	{
		return _id;
	}
	virtual operator bool() const noexcept
	{
		return _hBuffer != 0;
	}
	void bindBuffer(int BP) noexcept;
	void sync() const noexcept
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
	void unbindBuffer() noexcept;
	operator GLuint() const noexcept
	{
		return _hBuffer;
	}
#if defined (_USE_OPENGL_ES_)
#else
	GLuint openGLFlags() const noexcept
	{
		return _glFlags;
	}
#endif // _USE_OPENGL_ES_
	std::shared_ptr<Buffer> getFromCollection(Id<Buffer>::idType n);
	GraphicContext::Ptr context() const noexcept
	{
		return _context;
	}
	virtual ~Buffer() noexcept;
};

#endif // _BUFFER_H_