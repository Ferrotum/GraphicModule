#pragma once
#ifndef RASTERIZEDSTRING_H_
#define RASTERIZEDSTRING_H_

#include "MyFace.h"

#include <string>

class RasterizedString
{
private:
	std::wstring _text;
	std::shared_ptr<MyFace> _pFace;
	int _spaceLength;
	int _innerInterval;
	unsigned char* _buffer;
	int _bufferSize;
	int _width;
	int _height;
	void _moveProc(RasterizedString& a) noexcept
	{
		_text = std::move(a._text);
		_pFace = a._pFace;
		_spaceLength = a._spaceLength;
		_buffer = a._buffer;
		a._buffer = nullptr;
		_bufferSize = a._bufferSize;
		a._bufferSize = 0;
		_width = a._width;
		a._width = 0;
		_height = a._height;
		a._height = 0;
		_innerInterval = a._innerInterval;
	}
public:
	RasterizedString() noexcept;
	RasterizedString(const std::wstring& str) noexcept;
	RasterizedString(const RasterizedString&) = delete;
	RasterizedString(RasterizedString&& a) noexcept
	{
		_moveProc(a);
	}
	RasterizedString& operator=(const RasterizedString&) = delete;
	RasterizedString& operator=(RasterizedString&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void setString(const std::wstring& str)
	{
		_text = str;
	}
	void setFace(std::shared_ptr<MyFace> p)
	{
		if (p != nullptr && static_cast<bool>(*p)) {
			_pFace = p;
			constexpr float SPACE_K = 0.278f; // default space koefficient depends on a font size
			_spaceLength = (int)((float)_pFace->size() * SPACE_K);
			if (_spaceLength == 0) {
				_spaceLength = 1;
			}
		}
	}
	int setSpaceSize(int newValue) noexcept
	{
		int temp = _spaceLength;
		_spaceLength = newValue;
		return temp;
	}
	int setInnerInterval(int newValue) noexcept
	{
		int temp = _innerInterval;
		_innerInterval = newValue;
		return temp;
	}
	void build();
	int width() const noexcept
	{
		return _width;
	}
	int height() const noexcept
	{
		return _height;
	}
	const unsigned char* bitmap() const noexcept
	{
		return _buffer;
	}
	operator bool() const noexcept
	{
		return _buffer != nullptr;
	}
	~RasterizedString() noexcept
	{
		if (_buffer != nullptr) {
			delete[] _buffer;
		}
	}
};

#endif // RASTERIZEDSTRING_H_