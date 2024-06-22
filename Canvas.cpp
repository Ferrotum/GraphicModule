#include "Canvas.h"

void Canvas::_moveProc(Canvas& a) noexcept
{
	_context = a._context;
	a._context = nullptr;
	_isOk = a._isOk;
	a._isOk = false;
}

//==================================================================================

Canvas::Canvas():
	_isOk(false),
	_bckColorf{ 1.0f, 1.0f, 1.0f, 1.0f },
	_bckColori(0xFFFFFFFF),
	_viewportSize{0, 0, 0, 0}
{
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
	_viewport = std::shared_ptr<Viewport<int>>(new Viewport<int>(_context));
	_isOk = true;
}

//==================================================================================

Canvas::Canvas(GraphicContext::Ptr context) :
	_isOk(false),
	_context(context),
	_bckColorf{ 1.0f, 1.0f, 1.0f, 1.0f },
	_bckColori(0xFFFFFFFF),
	_viewportSize{ 0, 0, 0, 0 }
{
	_viewport = std::shared_ptr<Viewport<int>>(new Viewport<int>(_context));
	_isOk = true;
}

//==================================================================================

void Canvas::resizeWindow(int width, int height)
{
	if (!_isOk) {
		return;
	}
	_viewportWidth = width;
	_viewportHeight = height;
	_viewport->setRect(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
}

//==================================================================================

void Canvas::setBasePoint(int x, int y)
{
	if (!_isOk) {
		return;
	}
	_viewportX = x;
	_viewportY = y;
	_viewport->setRect(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
}

//==================================================================================

Canvas::operator bool() const noexcept
{
	return _isOk;
}
