#pragma once
#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "ViewData.h"
#include "ProjectionMatrix.h"
#include "Object.h"

template <typename T>
class Observer : public Object, protected ViewData<T>, protected ProjectionMatrix<T>
{
private:
	void _moveProc(Observer<T>& a) noexcept;
	void _copyProc(const Observer<T>& a) noexcept;
public:
	Observer();
	Observer(const Observer<T>& a);
	Observer(Observer<T>&& a) noexcept;
	Observer<T>& operator=(const Observer<T>& a) noexcept;
	Observer<T>& operator=(Observer<T>&& a) noexcept;
	virtual ~Observer() noexcept;
};

//==================================================================================

template<typename T>
void Observer<T>::_moveProc(Observer<T>& a) noexcept
{
}

//==================================================================================

template<typename T>
inline void Observer<T>::_copyProc(const Observer<T>& a) noexcept
{
}

//==================================================================================

template<typename T>
Observer<T>::Observer()
{
}

//==================================================================================

template<typename T>
Observer<T>::Observer(const Observer<T>& a) :
	ViewData<T>(a),
	ProjectionMatrix<T>(a)
{
	_copyProc(a);
}

//==================================================================================

template<typename T>
Observer<T>::Observer(Observer<T>&& a) noexcept :
	Object(std::move(a)),
	ViewData<T>(std::move(a)),
	ProjectionMatrix<T>(std::move(a))
{
	_moveProc(a);
}

//==================================================================================

template<typename T>
Observer<T>& Observer<T>::operator=(const Observer<T>& a) noexcept
{
	if (this != &a) {
		ViewData<T>::operator=(a);
		ProjectionMatrix<T>::operator=(a);
		_copyProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
Observer<T>& Observer<T>::operator=(Observer<T>&& a) noexcept
{
	if (this != &a) {
		Object::operator=(std::move(a));
		ViewData<T>::operator=(std::move(a));
		ProjectionMatrix<T>::operator=(std::move(a));
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
Observer<T>::~Observer() noexcept
{
}

#endif // OBSERVER_H_