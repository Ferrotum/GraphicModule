#pragma once
#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include "OpenGLAux.h"

class Rasterizer
{
private:
	void _moveProc(Rasterizer& a) noexcept;
protected:
	bool _isOK;
	float _lineWidth;
	float _lineWidthRange[2];
	float _pointSize;
	float _pointSizeRange[2];
	bool _culling;
	unsigned int _frontFace;
	unsigned int _cullFaceMode;

public:
	struct Direction
	{
		enum { COUNTERCLOCKWISE, CLOCKWISE };
	};
	struct CullingFace
	{
		enum { FRONT, BACK, FRONT_AND_BACK };
	};
	Rasterizer();
	Rasterizer(const Rasterizer&) = delete;
	Rasterizer(Rasterizer&& a) noexcept;
	Rasterizer& operator=(const Rasterizer&) = delete;
	Rasterizer& operator=(Rasterizer&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	bool init() noexcept;
	void setLineWidth(float w) noexcept;
	float lineWidth() const noexcept
	{
		return _lineWidth;
	}
	void setPointSize(float s) noexcept;
	float pointSize() const noexcept
	{
		return _pointSize;
	}
	void setCulling(bool b) noexcept;
	bool culling() const noexcept
	{
		return _culling;
	}
	void setFrontFace(unsigned int ff) noexcept;
	unsigned int frontFace() const noexcept
	{
		return _frontFace;
	}
	void setCullFace(unsigned int cf) noexcept;
	unsigned int cullFace() const noexcept
	{
		return _cullFaceMode;
	}
	operator bool() const noexcept
	{
		return _isOK;
	}
	virtual ~Rasterizer() noexcept;
};

#endif // RASTERIZER_H_