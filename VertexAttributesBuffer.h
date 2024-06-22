#pragma once
#ifndef VERTEXATTRIBUTESBUFFER_H_
#define VERTEXATTRIBUTESBUFFER_H_

#include <memory>
#include <string>
#include <map>

#include "VertexAttributesBufferException.h"
#include "Buffer.h"

class Surface;

class VertexAttributesBuffer
{
	friend Surface;
public:
	struct AttribProperties
	{
		int offset;
		int divisor;
		AttribProperties(int o, int d = 0) : offset(o), divisor(d) {}
	};
private:
	int _bufferDivisor;
	std::shared_ptr<Buffer> _buffer;
	int _stride;
	std::map<std::string, AttribProperties> _attributeProperties;
	void _moveProc(VertexAttributesBuffer& a) noexcept;
public:
	VertexAttributesBuffer() noexcept :
		_buffer(nullptr),
		_stride(0),
		_bufferDivisor(0)
	{}
	VertexAttributesBuffer(const std::shared_ptr<Buffer>& buff, int strd, int div = 0) :
		_buffer(buff),
		_stride(strd),
		_bufferDivisor(div)
	{
		if (_buffer == nullptr || !static_cast<bool>(*_buffer) || _stride <= 0 || div < 0) {
			throw VertexAttributesBufferException("VertexAttributesBuffer error.");
		}
	}
	VertexAttributesBuffer(const VertexAttributesBuffer&) = delete;
	VertexAttributesBuffer(VertexAttributesBuffer&& a) noexcept
	{
		_moveProc(a);
	}
	VertexAttributesBuffer& operator=(const VertexAttributesBuffer&) = delete;
	VertexAttributesBuffer& operator=(VertexAttributesBuffer&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	bool appendAttribute(const std::string& name, int dataOffset, int div = 0);
	void clearAttribute() noexcept
	{
		_attributeProperties.clear();
	}
	bool eraseAttribute(const std::string& name) noexcept;
	std::shared_ptr<Buffer> getBuffer() const noexcept
	{
		return _buffer;
	}
	int bufferDivisor() const noexcept
	{
		return _bufferDivisor;
	}
	void setBufferDivisor(int n) noexcept
	{
		if (n >= 0) {
			_bufferDivisor = n;
		}
	}
	operator bool() const noexcept
	{
		return (_buffer != nullptr) && static_cast<bool>(*_buffer);
	}
	~VertexAttributesBuffer() noexcept
	{
	}
};

#endif // VERTEXATTRIBUTESBUFFER_H_