#pragma once
#ifndef VIEWDATA_H_
#define VIEWDATA_H_

#include <atomic>
#include <cmath>
#include <cstdlib>
#include <mutex>

#include "math_definitions.h"
#include "MatrixLibrary.h"
#include "necessaryMacro.h"

#define _VIEWDATA_USE_SPINLOCK_
#ifdef _VIEWDATA_USE_SPINLOCK_
#include "SpinlockMutex.h"
typedef SpinlockMutex ViewdataSyncObj;
#else
typedef std::mutex ViewdataSyncObj;
#endif // _VIEWDATA_USE_SPINLOCK_



template <typename T>
class ViewData
{
	static_assert(std::is_floating_point<T>::value, "ViewData<T>: T must be a floating type.");
public:
#if defined(__AVX512BW__) || defined(__AVX512CD__) || defined(__AVX512DQ__) || defined(__AVX512F__) || defined(__AVX512VL__)
	enum { ALIGNMENT = 64 };
#elif defined (__AVX__)
	enum { ALIGNMENT = 32 };
#else
	enum { ALIGNMENT = 16 };
#endif //

#if defined (_USE_EIGEN_MATRIX_LIBRARY_)
#ifdef _MSC_VER
	using mat4 = __declspec(align(ALIGNMENT)) Eigen::Matrix4<T>;
#elif defined (__GNUC__)
	using mat4 = Eigen::Matrix4<T> __attribute__((aligned(ViewData<T>::ALIGNMENT)));
#endif // _MSC_VER
#elif defined(_USE_QT_MATRIX_LIBRARY_)
#ifdef _MSC_VER
	using mat4 = __declspec(align(ALIGNMENT)) QGenericMatrix<4, 4, T>;
#elif defined (__GNUC__)
	using mat4 = QGenericMatrix<4, 4, T> __attribute__((aligned(ViewData<T>::ALIGNMENT)));
#endif // _MSC_VER
#endif // _USE_EIGEN_MATRIX_LIBRARY_
	typedef T valueType;
protected:
	mat4* _viewMatrix;
	std::atomic<bool> _updated;

	static void _firstInit();
	static std::atomic<bool> _hasBeenInited;
	static ViewdataSyncObj _mt;
	alignas(ALIGNMENT) static mat4 _identityMatrix;

	void _moveProc(ViewData<T>& a) noexcept;
	void _copyProc(const ViewData<T>& a) noexcept;
public:
	ViewData();
	ViewData(const ViewData<T>& a);
	ViewData(ViewData<T>&& a) noexcept;
	ViewData<T>& operator=(const ViewData<T>& a) noexcept;
	ViewData<T>& operator=(ViewData<T>&& a) noexcept;
	const mat4& matrix() const noexcept;
	const T* data() const noexcept;
	const T* viewPosition() const noexcept;
	std::atomic<bool>* updatePtr() noexcept;
	virtual ~ViewData() noexcept;
};

//==================================================================================

template <typename T> std::atomic<bool> ViewData<T>::_hasBeenInited = false;

template <typename T> ViewdataSyncObj ViewData<T>::_mt;

template <typename T>
alignas(ViewData<T>::ALIGNMENT) typename ViewData<T>::mat4 ViewData<T>::_identityMatrix;


//==================================================================================

template<typename T>
inline void ViewData<T>::_firstInit()
{
#if defined (_USE_EIGEN_MATRIX_LIBRARY_)
	_identityMatrix = Eigen::Matrix4<T>::Identity();
#elif defined (_USE_QT_MATRIX_LIBRARY_)
	_identityMatrix.setToIdentity();
#endif //
	_hasBeenInited.store(true, std::memory_order_release);
}

//==================================================================================


template<typename T>
void ViewData<T>::_moveProc(ViewData<T>& a) noexcept
{
	if (_viewMatrix != nullptr) {
#if defined (_MSC_VER)
		_aligned_free(_viewMatrix);
#else
		free(_viewMatrix);
#endif // _MSC_VER
	}
	_viewMatrix = a._viewMatrix;
	a._viewMatrix = nullptr;
	_updated.store(a._updated.load(std::memory_order_acquire), std::memory_order_release);
}

//==================================================================================

template<typename T>
inline void ViewData<T>::_copyProc(const ViewData<T>& a) noexcept
{
	*_viewMatrix = *a._viewMatrix;
	_updated.store(true, std::memory_order_release);
}

//==================================================================================

template<typename T>
inline ViewData<T>::ViewData() :
	_updated(true),
	_viewMatrix(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ViewdataSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
#if defined (_MSC_VER)
	_viewMatrix = static_cast<mat4*>(_aligned_malloc(sizeof(mat4), ALIGNMENT));
#else
	_viewMatrix = static_cast<mat4*>(aligned_alloc(sizeof(mat4), ALIGNMENT));
#endif // _MSC_VER
	*_viewMatrix = _identityMatrix;
}

//==================================================================================

template<typename T>
inline ViewData<T>::ViewData(const ViewData<T>& a) :
	_updated(true),
	_viewMatrix(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ViewdataSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
#if defined (_MSC_VER)
	_viewMatrix = static_cast<mat4*>(_aligned_malloc(sizeof(mat4), ALIGNMENT));
#else
	_viewMatrix = static_cast<mat4*>(aligned_alloc(sizeof(mat4), ALIGNMENT));
#endif // _MSC_VER
	_copyProc(a);
}

//==================================================================================

template<typename T>
inline ViewData<T>::ViewData(ViewData<T>&& a) noexcept :
	_updated(true),
	_viewMatrix(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ViewdataSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline ViewData<T>& ViewData<T>::operator=(const ViewData<T>& a) noexcept
{
	if (this != &a) {
		_copyProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline ViewData<T>& ViewData<T>::operator=(ViewData<T>&& a) noexcept
{
	if (this != &a) {
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline const typename ViewData<T>::mat4& ViewData<T>::matrix() const noexcept
{
	return *_viewMatrix;
}

//==================================================================================

template<typename T>
inline const T* ViewData<T>::data() const noexcept
{
	return _viewMatrix->data();
}

//==================================================================================

template<typename T>
inline const T* ViewData<T>::viewPosition() const noexcept
{
	return _viewMatrix->data() + sizeof(T) * 12;
}

//==================================================================================

template<typename T>
inline std::atomic<bool>* ViewData<T>::updatePtr() noexcept
{
	return _updated;
}

//==================================================================================

template<typename T>
inline ViewData<T>::~ViewData() noexcept
{
	if (_viewMatrix != nullptr) {
#if defined (_MSC_VER)
		_aligned_free(_viewMatrix);
#else
		free(_viewMatrix);
#endif // _MSC_VER
	}
}

#endif // VIEWDATA_H_