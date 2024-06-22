#include "VertexArrayObject.h"


void VertexArrayObject::_deleteVertexArray() noexcept
{
	if (_handle) {
		_context->setCurrent();
		if (_context->_activeVertexArrayObject == _id) {
			_context->setActiveNoVertexArray();
		}
		glDeleteVertexArrays(1, &_handle);
		_handle = 0;
	}
}

//==================================================================================

void VertexArrayObject::_moveProc(VertexArrayObject& a) noexcept
{
	_deleteVertexArray();
	_handle = a._handle;
	a._handle = 0;
	_id = std::move(a._id);
	_context = a._context;
}

//==================================================================================

VertexArrayObject::VertexArrayObject() noexcept :
	_handle(0),
	_context(nullptr)
{
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glGenVertexArrays(1, &_handle);
#else
	glCreateVertexArrays(1, &_handle);
#endif // _USE_OPENGL_ES_
}

//==================================================================================

VertexArrayObject::VertexArrayObject(GraphicContext::Ptr context) noexcept :
	_context(context)
{
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glGenVertexArrays(1, &_handle);
#else
	glCreateVertexArrays(1, &_handle);
#endif // _USE_OPENGL_ES_
}

//==================================================================================

void VertexArrayObject::setActive() const noexcept
{
	if (_context->_activeVertexArrayObject == _id) {
		return;
	}
	_context->_activeVertexArrayObject = _id;
	if (_handle) {
		_context->setCurrent();
		glBindVertexArray(_handle);
	}
}
