#pragma once
#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "AABR.h"
#include "Object.h"
#include "SyncObject.h"
#include "GraphicContext.h"


#include <type_traits>

template <typename T>
class Viewport : public Object
{
	static_assert(std::is_arithmetic<T>::value, "Viewport<T>: T must be an arithmetic type");
protected:
	int _index;
	AABR<T>* _viewportBox;
	GraphicContext::Ptr _context;
	void _moveProc(Viewport<T>& a) noexcept;
	void _hasModified() noexcept
	{
		Object::_hasModified();
	}
	void _update() noexcept;
public:
	Viewport();
	Viewport(T x, T y, T width, T height);
	Viewport(const T* v);
	Viewport(GraphicContext::Ptr context);
	Viewport(GraphicContext::Ptr context, T x, T y, T width, T height);
	Viewport(GraphicContext::Ptr context, const T* v);
	Viewport(const Viewport<T>&) = delete;
	Viewport(Viewport<T>&& a) noexcept :
		Object(std::move(a))
	{
		_moveProc(a);
		Object::_hasModified();
	}
	Viewport<T>& operator=(const Viewport<T>&) = delete;
	Viewport<T>& operator=(Viewport<T>&& a) noexcept
	{
		if (this != &a) {
			Object::operator=(std::move(a));
			_moveProc(a);
			Object::_hasModified();
		}
		return *this;
	}
	T x() const noexcept
	{
		return (*_viewportBox)[0][0];
	}
	T y() const noexcept
	{
		return (*_viewportBox)[0][1];
	}
	T width() const noexcept
	{
		return *_viewportBox->width();
	}
	T height() const noexcept
	{
		return *_viewportBox->height();
	}
	const AABR<T>* box() const noexcept
	{
		return _viewportBox;
	}
	void setX(T x) noexcept;
	void setY(T y) noexcept;
	void setWidth(T width) noexcept;
	void setHeight(T height) noexcept;
	void setOriginPos(T x, T y) noexcept;
	void setOriginPos(const T* v) noexcept;
	void setRect(T x, T y, T width, T height) noexcept;
	void setRect(const T* v) noexcept;
	~Viewport() noexcept override;
};

//==================================================================================

template<typename T>
inline void Viewport<T>::_moveProc(Viewport<T>& a) noexcept
{
	_viewportBox = a._viewportBox;
	a._viewportBox = nullptr;
	_index = a._index;
	_context = a._context;
}

//==================================================================================

template<typename T>
inline void Viewport<T>::_update() noexcept
{
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glViewport((GLint)(*_viewportBox)[0][0], (GLint)(*_viewportBox)[0][1], (GLsizei)_viewportBox->width(), (GLsizei)_viewportBox->height());
#else
	glViewportIndexedf(_index, (GLfloat)(*_viewportBox)[0][0], (GLfloat)(*_viewportBox)[0][1], (GLfloat)_viewportBox->width(), (GLfloat)_viewportBox->height());
#endif // _USE_OPENGL_ES_
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport() :
	_viewportBox(new AABR<T>),
	_index(0),
	_context(nullptr)
{
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport(T x, T y, T width, T height) :
	_viewportBox(new AABR<T>(x, y, x + width, y + height)),
	_index(0),
	_context(nullptr)
{
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport(const T* v) :
	_viewportBox(new AABR<T>(v[0], v[1], v[0] + v[2], v[1] + v[3])),
	_index(0),
	_context(nullptr)
{
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport(GraphicContext::Ptr context) :
	_viewportBox(new AABR<T>),
	_index(0),
	_context(context)
{
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport(GraphicContext::Ptr context, T x, T y, T width, T height) :
	_viewportBox(new AABR<T>(x, y, x + width, y + height)),
	_index(0),
	_context(context)
{
}

//==================================================================================

template<typename T>
inline Viewport<T>::Viewport(GraphicContext::Ptr context, const T* v) :
	_viewportBox(new AABR<T>(v[0], v[1], v[0] + v[2], v[1] + v[3])),
	_index(0),
	_context(context)
{
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setX(T x) noexcept
{
	*_viewportBox = AABR<T>(x, (*_viewportBox)[0][1], x + _viewportBox->width(), (*_viewportBox)[1][1]);
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setY(T y) noexcept
{
	*_viewportBox = AABR<T>((*_viewportBox)[0][0], y, (*_viewportBox)[1][0], y + _viewportBox->height());
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setWidth(T width) noexcept
{
	*_viewportBox = AABR<T>((*_viewportBox)[0][0], (*_viewportBox)[0][1], (*_viewportBox)[0][0] + width, (*_viewportBox)[1][1]);
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setHeight(T height) noexcept
{
	*_viewportBox = AABR<T>((*_viewportBox)[0][0], (*_viewportBox)[0][1], (*_viewportBox)[0][1], (*_viewportBox)[0][1] + height);
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setOriginPos(T x, T y) noexcept
{
	*_viewportBox = AABR<T>(x, y, x + _viewportBox->width(), y + _viewportBox->height());
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setOriginPos(const T* v) noexcept
{
	*_viewportBox = AABR<T>(v[0], v[1], v[0] + _viewportBox->width(), v[1] + _viewportBox->height());
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setRect(T x, T y, T width, T height) noexcept
{
	*_viewportBox = AABR<T>(x, y, x + x + width, y + height);
	_update();
}

//==================================================================================

template<typename T>
inline void Viewport<T>::setRect(const T* v) noexcept
{
	*_viewportBox = AABR<T>(v[0], v[1], v[0] + v[2], v[1] + v[3]);
	_update();
}

//==================================================================================

template<typename T>
inline Viewport<T>::~Viewport() noexcept
{
	if (_viewportBox != nullptr) {
		delete _viewportBox;
	}
}

#endif // VIEWPORT_H_
