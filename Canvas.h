#pragma once
#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <atomic>
#include <string>
#include <memory>
#include "OpenGLAux.h"
#include "Viewport.h"
#include "GraphicContext.h"

class Canvas
{
protected:
	bool _isOk;
	float _bckColorf[4];
	unsigned int _bckColori;
	std::shared_ptr<Viewport<int>> _viewport;

	GraphicContext::Ptr _context;

	int _viewportSize[4];

	int& _viewportX = _viewportSize[0];
	int& _viewportY = _viewportSize[1];
	int& _viewportWidth = _viewportSize[2];
	int& _viewportHeight = _viewportSize[3];
private:
	void _moveProc(Canvas& a) noexcept;
public:
	Canvas();
	Canvas(GraphicContext::Ptr context);
	Canvas(const Canvas&) = delete;
	Canvas(Canvas&& a) noexcept
	{
		_moveProc(a);
	}
	Canvas& operator=(const Canvas&) = delete;
	Canvas& operator=(Canvas&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	virtual void resizeWindow(int width, int height);
	virtual void setBasePoint(int x, int y);
	virtual void update() noexcept = 0;
	virtual void draw() const noexcept = 0;
	unsigned int setBackgroundColor(unsigned int newColor) noexcept
	{
		unsigned int oldColor = _bckColori;
		_bckColori = newColor;
		_bckColorf[0] = (float)(_bckColori & 0xFF) / 255.0f;
		_bckColorf[1] = (float)((_bckColori >> 8) & 0xFF) / 255.0f;
		_bckColorf[2] = (float)((_bckColori >> 16) & 0xFF) / 255.0f;
		_bckColorf[3] = (float)((_bckColori >> 24) & 0xFF) / 255.0f;
		return oldColor;
	}
	unsigned int backgroundColor() const noexcept
	{
		return _bckColori;
	}
	virtual operator bool() const noexcept;
	virtual ~Canvas() noexcept
	{
	}
};

#endif // _CANVAS_H_