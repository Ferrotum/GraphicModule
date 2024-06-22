#pragma once
#ifndef GFINDEXBUFFER_H_
#define GFINDEXBUFFER_H_

#include "Buffer.h"

class Surface;

class IndexBuffer : public Buffer
{
	friend Surface;
public:
	struct Type
	{
		enum { UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT };
	};
protected:
	int _elementType;
	unsigned int _numElements;
	int _GLelementType;

	void _moveProc(IndexBuffer& a) noexcept;
	void _create(unsigned int size, int type);
public:
	IndexBuffer() noexcept : 
		_elementType(-1),
		_numElements(0),
		_GLelementType(0)
	{}
	IndexBuffer(unsigned int size, int type, unsigned int flags, const void* data = 0);
	IndexBuffer(GraphicContext::Ptr context, unsigned int size, int type, unsigned int flags, const void* data = 0);
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& a) noexcept :
		Buffer(std::move(a))
	{
		_moveProc(a);
	}
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer& operator=(IndexBuffer&& a) noexcept
	{
		if (this != &a) {
			Buffer::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	unsigned int numElements() const noexcept
	{
		return _numElements;
	}
	int type() const noexcept
	{
		return _elementType;
	}
	~IndexBuffer() noexcept override;
};

#endif // GFINDEXBUFFER_H_