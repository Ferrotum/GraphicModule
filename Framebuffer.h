#pragma once
#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <vector>
#include <atomic>

#include "Id.h"
#include "OpenGLAux.h"
#include "ColorParameters.h"
#include "DepthParameters.h"
#include "StencilParameters.h"
#include "GraphicContext.h"
#include "Texture2D.h"
#include "Texture2DMultisample.h"

class Framebuffer
{
protected:
	struct ColorTexture
	{
		std::shared_ptr<Texture> texture;
		unsigned int _clearingColorui;
		ColorParameters blend;
		bool enable;
		ColorTexture() noexcept : _clearingColorui(0xFFFFFFFF), enable(false) {}
	};
protected:
	bool _isOK;
	GLuint _framebuffer;
	Id<Framebuffer> _id;
	static std::atomic<Id<Framebuffer>::idType> _activeFramebuffer;
	static int _maxColorTextures;
	static int _maxWidth;
	static int _maxHeight;
	int _multisample;
	bool _depthEnable;
	bool _stencilEnable;
	mutable int _width;
	mutable int _height;
	std::vector<ColorTexture> _colorTextures;
	std::shared_ptr<Texture> _depthTexture;

	GraphicContext::Ptr _context;

	void _moveProc(Framebuffer& a) noexcept;
	void _deleteFramebuffer() noexcept;
public:
	struct Texture
	{
		enum { DEPTH32F,
			STENCIL,
			DEPTH_STENCIL,
			COLOR0,
			COLOR1,
			COLOR2,
			COLOR3,
			COLOR4,
			COLOR5,
			COLOR6,
			COLOR7 };
	};
	Framebuffer();
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& a) noexcept
	{
		_moveProc(a);
	}
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer& operator=(Framebuffer&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void resize(int width, int height);
	int width() const noexcept;
	int height() const noexcept
	{
		return _height;
	}
	void setActive() noexcept;
	Id<Framebuffer>::idType id() const noexcept
	{
		return _id;
	}
	operator GLuint() const noexcept
	{
		return _framebuffer;
	}
	GLuint handle() const noexcept
	{
		return _framebuffer;
	}
	operator bool() const noexcept
	{
		return _framebuffer != 0;
	}
	~Framebuffer();
};
#endif // !_FRAMEBUFFER_H_
