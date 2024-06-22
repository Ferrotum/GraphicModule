#include "RasterizedString.h"

RasterizedString::RasterizedString() noexcept :
	_buffer(nullptr),
	_bufferSize(0),
	_width(0),
	_height(0),
	_innerInterval(1)
{
}

//===========================================================================

RasterizedString::RasterizedString(const std::wstring& str) noexcept :
	_buffer(nullptr),
	_text(str),
	_bufferSize(0),
	_width(0),
	_height(0),
	_innerInterval(1)
{
}

//===========================================================================

void RasterizedString::build()
{
	if (_pFace == nullptr || !static_cast<bool>(*_pFace)) {
		return;
	}
	int bufferWidth = 0;
	for (int i = 0; i < _text.size(); ++i) {
		if (_text[i] == L' ') {
			bufferWidth += _spaceLength;
			continue;
		}
		const GlyphMetrics& gm = _pFace->glyph(_text[i]).metrics();
		bufferWidth += gm.width + _innerInterval;
	}
	int maxHeight = _pFace->maxNeedHeight();
	_width = bufferWidth;
	_height = maxHeight;
	bufferWidth = (bufferWidth + 3) & (~0x3);
	_bufferSize = bufferWidth * maxHeight;
	_buffer = new unsigned char[_bufferSize];
	if (_buffer == nullptr) {
		return;
	}
	memset(_buffer, 0, _bufferSize);
	int currentX = 0;
	int maxUp = _pFace->maxBearingY();
	for (int i = 0; i < _text.size(); ++i) {
		if (_text[i] == L' ') {
			currentX += _spaceLength;
			continue;
		}
		const Glyph& g = _pFace->glyph(_text[i]);
		int width = g.metrics().width;
		int height = g.metrics().height;
		int pitch = g.metrics().pitch;
		int bearing = g.metrics().hBearingY;
		for (int j = 0; j < height; ++j) {
			memcpy(_buffer + (j + maxUp - bearing) * bufferWidth + currentX, g.bitmap() + j * pitch, width);
		}
		currentX += width + _innerInterval;
	}
}
