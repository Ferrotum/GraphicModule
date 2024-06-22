#pragma once
#ifndef INTERACTOR_H_
#define INTERACTOR_H_

#include <cstdint>
#include <limits.h>
#include <type_traits>

#include "Transform.h"

template <typename T>
struct DeletePointerOrReference
{
	static_assert(!(std::is_fundamental_v<T> || std::is_class_v<T>), "Type must be void* or a reference to a smart pointer");
	using Type = T;
};

template <>
struct DeletePointerOrReference<void*>
{
	using Type = void*;
};

template <typename T>
struct DeletePointerOrReference<T&>
{
	static_assert(std::is_class_v<T> && !std::is_function_v< decltype(&T::get)>, "Type must be a smart pointer.");
	using Type = T;
};

template <typename T>
struct DeletePointerOrReference<T*>
{
	static_assert(!(std::is_fundamental_v<T> || std::is_class_v< T>), "Type must be void* or a reference to a smart pointer. Smart pointer class must have a fuction get()");
	using Type = T;
};

template <typename T, typename P = void*>
class Interactor : public Transform<T>
{
public:
#if defined (_M_AMD64) || defined (_M_ARM64) || defined(__LP64__)
	using idType = uint_fast64_t;
	enum { ID_TYPE_MAX = ULLONG_MAX };
#else
	using idType = unsigned int;
	enum { ID_TYPE_MAX = UINT_MAX };
#endif // _M_AMD64 || _M_ARM64 || __LP64__
	using PredrawFuncType = void (*)(P, P);
	using PostdrawFuncType = void (*)(P, P);
	using DrawFuncArgType = P;
	using DrawFuncArgTypeBase = typename DeletePointerOrReference<P>::Type;
protected:
	using _reference = DrawFuncArgTypeBase&;
	using _pointer = DrawFuncArgTypeBase*;
	void (*_predrawFunc)(const DrawFuncArgType, DrawFuncArgType);
	mutable DrawFuncArgTypeBase _predrawArgIn;
	mutable DrawFuncArgTypeBase _predrawArgOut;
	void (*_postdrawFunc)(const DrawFuncArgType, DrawFuncArgType);
	mutable DrawFuncArgTypeBase _postrawArgIn;
	mutable DrawFuncArgTypeBase _postrawArgOut;
	bool _hiden;
	void _moveProc(Interactor<T, P>& a) noexcept;
	void _predrawFuncRun() const;
	void _postdrawFuncRun() const;
public:
	Interactor();
	Interactor(const Interactor<T, P>&) = delete;
	Interactor(Interactor<T, P>&& a) noexcept;
	Interactor<T, P>& operator=(const Interactor<T, P>&) = delete;
	Interactor<T, P>& operator=(Interactor<T, P>&& a) noexcept;
	virtual void update() const noexcept = 0;
	virtual void draw() const noexcept = 0;
	virtual void clear() noexcept override;
	virtual operator bool() const noexcept = 0;
	virtual bool hiden() const noexcept;
	virtual void hide(bool b) noexcept;
	void setPredrawFunc(void (*newPredrawFunc)(const P, P), const P inData, P outData) noexcept;
	void setPredrawFunc(void (*newPredrawFunc)(const P, P), P data) noexcept;
	void setPredrawFunc(void (*newPredrawFunc)(const P, P)) noexcept;
	void (*predrawFunc() const noexcept)(const P, P);
	void setPostdrawFunc(void (*newPostrawFunc)(const P, P), const P inData, P outData) noexcept;
	void setPostdrawFunc(void (*newPostrawFunc)(const P, P), P data) noexcept;
	void setPostdrawFunc(void (*newPostrawFunc)(const P, P)) noexcept;
	void (*postdrawFunc() const noexcept)(const P, P);
	virtual ~Interactor() noexcept override;
};

//==================================================================================

template<typename T, typename P>
void Interactor<T, P>::_moveProc(Interactor<T, P>& a) noexcept
{
	_hiden = a._hiden;
	_predrawFunc = a._predrawFunc;
	_postdrawFunc = a._postdrawFunc;
	_predrawArgIn = std::move(a._predrawArgIn);
	_predrawArgOut = std::move(a._predrawArgOut);
	_postrawArgIn = std::move(a._postrawArgIn);
	_postrawArgOut = std::move(a._postrawArgOut);
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::_predrawFuncRun() const
{
	if (_predrawFunc != nullptr) {
		_predrawFunc(_predrawArgIn, _predrawArgOut);
	}
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::_postdrawFuncRun() const
{
	if (_postdrawFunc != nullptr) {
		_postdrawFunc(_postrawArgIn, _postrawArgOut);
	}
}

//==================================================================================

template<typename T, typename P>
Interactor<T, P>::Interactor() :
	_hiden(false),
	_predrawFunc(nullptr),
	_postdrawFunc(nullptr),
	_predrawArgIn(nullptr),
	_predrawArgOut(nullptr),
	_postrawArgIn(nullptr),
	_postrawArgOut(nullptr)
{
}

//==================================================================================

template<typename T, typename P>
Interactor<T, P>::Interactor(Interactor<T, P>&& a) noexcept :
	Transform<T>(std::move(a)),
	_hiden(false)
{
	_moveProc(a);
}

//==================================================================================

template<typename T, typename P>
Interactor<T, P>& Interactor<T, P>::operator=(Interactor<T, P>&& a) noexcept
{
	if (this != &a) {
		Transform<T>::operator=(std::move(a));
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T, typename P>
void Interactor<T, P>::clear() noexcept
{
	Transform<T>::clear();
	_hiden = false;
	_predrawFunc = nullptr;
	_postdrawFunc = nullptr;
	_predrawArgIn = nullptr;
	_predrawArgOut = nullptr;
	_postrawArgIn = nullptr;
	_postrawArgOut = nullptr;
}

//==================================================================================

template<typename T, typename P>
bool Interactor<T, P>::hiden() const noexcept
{
	return _hiden;
}

//==================================================================================

template<typename T, typename P>
void Interactor<T, P>::hide(bool b) noexcept
{
	_hiden = b;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPredrawFunc(void (*newPredrawFunc)(const P, P), const P inData, P outData) noexcept
{
	_predrawFunc = newPredrawFunc;
	_predrawArgIn = inData;
	_predrawArgOut = outData;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPredrawFunc(void(*newPredrawFunc)(const P, P), P data) noexcept
{
	_predrawFunc = newPredrawFunc;
	_predrawArgIn = data;
	_predrawArgOut = nullptr;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPredrawFunc(void(*newPredrawFunc)(const P, P)) noexcept
{
	_predrawFunc = newPredrawFunc;
	_predrawArgIn = nullptr;
	_predrawArgOut = nullptr;
}

//==================================================================================

template<typename T, typename P>
inline void (*Interactor<T, P>::predrawFunc() const noexcept)(const P, P)
{
	return _predrawFunc;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPostdrawFunc(void (*newPostrawFunc)(const P, P), const P inData, P outData) noexcept
{
	_postdrawFunc = newPostrawFunc;
	_postrawArgIn = inData;
	_postrawArgOut = outData;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPostdrawFunc(void(*newPostrawFunc)(const P, P), P data) noexcept
{
	_postdrawFunc = newPostrawFunc;
	_postrawArgIn = data;
	_postrawArgOut = nullptr;
}

//==================================================================================

template<typename T, typename P>
inline void Interactor<T, P>::setPostdrawFunc(void(*newPostrawFunc)(const P, P)) noexcept
{
	_postdrawFunc = newPostrawFunc;
	_postrawArgIn = nullptr;
	_postrawArgOut = nullptr;
}

//==================================================================================

template<typename T, typename P>
inline void (*Interactor<T, P>::postdrawFunc() const noexcept)(const P, P)
{
	return _postdrawFunc;
}

//==================================================================================

template<typename T, typename P>
Interactor<T, P>::~Interactor() noexcept
{
}

#endif // INTERACTOR_H_