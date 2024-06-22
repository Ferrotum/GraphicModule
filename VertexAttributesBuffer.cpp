#include "VertexAttributesBuffer.h"

void VertexAttributesBuffer::_moveProc(VertexAttributesBuffer& a) noexcept
{
	_buffer = std::move(a._buffer);
	_stride = a._stride;
	_attributeProperties = std::move(a._attributeProperties);
	_bufferDivisor = a._bufferDivisor;
}

//==================================================================================

bool VertexAttributesBuffer::appendAttribute(const std::string& name, int dataOffset, int div)
{
	bool retValue = false;
	if (_buffer != nullptr && static_cast<bool>(*_buffer)) {
		int d = div < 0 ? 0 : div;
		_attributeProperties.emplace(name, AttribProperties(dataOffset, d));
		retValue = true;
	}
	return retValue;
}

//==================================================================================

bool VertexAttributesBuffer::eraseAttribute(const std::string& name) noexcept
{
	auto p = _attributeProperties.find(name);
	if (p != _attributeProperties.end()) {
		_attributeProperties.erase(p);
		return true;
	}
	return false;
}
