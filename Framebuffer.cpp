#include "Framebuffer.h"

std::atomic<Id<Framebuffer>::idType> Framebuffer::_activeFramebuffer = 0;
int Framebuffer::_maxColorTextures = 0;
int Framebuffer::_maxWidth = 0;
int Framebuffer::_maxHeight = 0;

void Framebuffer::_moveProc(Framebuffer& a) noexcept
{
	_context = a._context;
	a._context = nullptr;
	_deleteFramebuffer();
	_framebuffer = a._framebuffer;
	a._framebuffer = 0;
	_id = std::move(a._id);
	_multisample = a._multisample;
	_depthEnable = a._depthEnable;
	_stencilEnable = a._stencilEnable;
	_width = a._width;
	_height = a._height;
	_colorTextures = std::move(a._colorTextures);
}

//==================================================================================

void Framebuffer::_deleteFramebuffer() noexcept
{
	if (_framebuffer != 0) {
		_context->setCurrent();
		glDeleteFramebuffers(1, &_framebuffer);
	}
}

//==================================================================================

Framebuffer::Framebuffer() :
	_framebuffer(0),
	_width(0),
	_height(0)
{
	_colorTextures.resize(_maxColorTextures);
	_context->setCurrent();
#if defined(_USE_OPENGL_ES_)
	glGenFramebuffers(1, &_framebuffer);
#else
	glCreateFramebuffers(1, &_framebuffer);
#endif // _USE_OPENGL_ES_
}

//==================================================================================

void Framebuffer::resize(int width, int height)
{
	if (!static_cast<bool>(*this)) {
		return;
	}
	if (width < 1 || width > _maxWidth || height < 1 || height > _maxHeight) {
		return;
	}
	_width = width;
	_height = height;
}

//==================================================================================

int Framebuffer::width() const noexcept
{
	if (static_cast<bool>(*this)) {
#if defined(_USE_OPENGL_ES_)
#else
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_WIDTH, &_width);
#endif // _USE_OPENGL_ES_
	}
	return _width;
}

//==================================================================================

void Framebuffer::setActive() noexcept
{
	if (_activeFramebuffer.load(std::memory_order_acquire) == _id) {
		return;
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
}

//==================================================================================

Framebuffer::~Framebuffer()
{
	_deleteFramebuffer();
}
