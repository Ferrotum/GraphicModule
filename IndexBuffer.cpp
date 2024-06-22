#include "IndexBuffer.h"

void IndexBuffer::_moveProc(IndexBuffer& a) noexcept
{
	_elementType = a._elementType;
	_numElements = a._numElements;
	_GLelementType = a._GLelementType;
}

void IndexBuffer::_create(unsigned int size, int Type)
{
	switch (Type) {
	case static_cast<int>(IndexBuffer::Type::UNSIGNED_BYTE): {
		_elementType = static_cast<int>(IndexBuffer::Type::UNSIGNED_BYTE);
		_GLelementType = GL_UNSIGNED_BYTE;
		_numElements = size;
		break;
	}
	case static_cast<int>(IndexBuffer::Type::UNSIGNED_SHORT): {
		_elementType = static_cast<int>(IndexBuffer::Type::UNSIGNED_SHORT);
		_GLelementType = GL_UNSIGNED_SHORT;
		_numElements = size >> 1;
		break;
	}
	case static_cast<int>(IndexBuffer::Type::UNSIGNED_INT): {
		_elementType = static_cast<int>(IndexBuffer::Type::UNSIGNED_INT);
		_GLelementType = GL_UNSIGNED_INT;
		_numElements = size >> 2;
		break;
	}
	default: {
		throw BufferException("Type unknown.");
		break;
	}
	}
}

//==================================================================================

IndexBuffer::IndexBuffer(unsigned int size, int Type, unsigned int flags, const void* data) :
	Buffer(size, flags, data)
{
	_create(size, Type);
}

//==================================================================================

IndexBuffer::IndexBuffer(GraphicContext::Ptr context, unsigned int size, int Type, unsigned int flags, const void* data) :
	Buffer(context, size, flags, data)
{
	_create(size, Type);
}

//==================================================================================

IndexBuffer::~IndexBuffer() noexcept
{
}
