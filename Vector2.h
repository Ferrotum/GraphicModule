#pragma once
#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "x86_64_intrin.h"

#if defined (min)
#pragma push_macro("min")
#undef min
#define _VECTOR2_MACRO_MIN_HAS_PUSHED_ 1
#endif // min
#if defined (max)
#pragma push_macro("max")
#undef max
#define _VECTOR2_MACRO_MAX_HAS_PUSHED_ 1
#endif // max

template<typename T>
class Vector2
{
	static_assert(std::is_arithmetic<T>::value, "Vector2<T>: T must be an arithmetic type");
public:
	using valueType = T;
	//enum { ARRAY_SIZE = 16 / sizeof(T) > 2 ? 16 / sizeof(T) : 2 };
	enum { ALIGNMENT = (sizeof(T) > 8 ? 32 : 16 ) };
protected:
	enum { ARRAY_SIZE = 2 };
	alignas(ALIGNMENT) T _data[ARRAY_SIZE];

	void _copyProc(const Vector2<T>& v) noexcept;
	void _moveProc(Vector2<T>& v) noexcept;
public:
	Vector2();
	Vector2(const Vector2<T>& v) noexcept;
	Vector2(Vector2<T>&& v) noexcept;
	Vector2(const T* p) noexcept;
	Vector2(T x, T y) noexcept;
	Vector2<T>& operator=(const Vector2<T>& v) noexcept;
	Vector2<T>& operator=(Vector2<T>&& v) noexcept;
	bool operator==(Vector2<T>&& v) const noexcept;
	T x() const noexcept;
	T y() const noexcept;
	const T* data() const noexcept;
	T* data() noexcept;
	const T& operator[](size_t idx) const noexcept;
	T& operator[](size_t idx) noexcept;
	Vector2<T> operator-(const Vector2<T>& v) const noexcept;
	Vector2<T> operator+(const Vector2<T>& v) const noexcept;
	Vector2<T>& operator+=(const Vector2<T>& v) noexcept;
	template <typename U>
	friend Vector2<U> operator*(U k, const Vector2<U>& v) noexcept;
	static Vector2<T> min(const Vector2<T>& v1, const Vector2<T>& v2) noexcept;
	static Vector2<T> max(const Vector2<T>& v1, const Vector2<T>& v2) noexcept;
	static void min(const Vector2<T>& v1, const Vector2<T>& v2, Vector2<T>& res) noexcept;
	static void max(const Vector2<T>& v1, const Vector2<T>& v2, Vector2<T>& res) noexcept;
	~Vector2() noexcept;
};

template<typename T>
inline void Vector2<T>::_copyProc(const Vector2<T>& v) noexcept
{
	memcpy(_data, v._data, sizeof(_data));
}

//==================================================================================

template<typename T>
inline void Vector2<T>::_moveProc(Vector2<T>& v) noexcept
{
	memcpy(_data, v._data, sizeof(_data));
}

//==================================================================================

template<typename T>
inline Vector2<T>::Vector2()
{
	memset(_data, 0, sizeof(_data));
}

//==================================================================================

template<typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v) noexcept
{
	_copyProc(v);
}

//==================================================================================

template<typename T>
inline Vector2<T>::Vector2(Vector2<T>&& v) noexcept
{
	_moveProc(v);
}

//==================================================================================

template<typename T>
inline Vector2<T>::Vector2(const T* p) noexcept
{
	memset(_data, 0, sizeof(_data));
	memcpy(_data, p, sizeof(_data));
}

//==================================================================================

template<typename T>
inline Vector2<T>::Vector2(T x, T y) noexcept
{
	memset(_data, 0, sizeof(_data));
	_data[0] = x;
	_data[1] = y;
}

//==================================================================================

template<typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v) noexcept
{
	if (this != &v) {
		_copyProc(v);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline Vector2<T>& Vector2<T>::operator=(Vector2<T>&& v) noexcept
{
	if (this != &v) {
		_moveProc(v);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline T Vector2<T>::x() const noexcept
{
	return _data[0];
}

//==================================================================================

template<typename T>
inline T Vector2<T>::y() const noexcept
{
	return _data[1];
}

//==================================================================================

template<typename T>
inline const T* Vector2<T>::data() const noexcept
{
	return _data;
}

//==================================================================================

template<typename T>
inline T* Vector2<T>::data() noexcept
{
	return _data;
}

//==================================================================================

template<typename T>
inline const T& Vector2<T>::operator[](size_t idx) const noexcept
{
	return _data[idx];
}

//==================================================================================

template<typename T>
inline T& Vector2<T>::operator[](size_t idx) noexcept
{
	return _data[idx];
}

//==================================================================================

#if defined (_USE_SIMD_EXTENTIONS_FOR_CALCULATING_)
#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)

#if defined(__AVX__)

template<>
inline Vector2<float> Vector2<float>::operator-(const Vector2<float>& v) const noexcept
{
	Vector2<float> temp;
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)_data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v._data);
	_single_8_ a3 = _AVX_subps256_(a1, a2);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0, 0 };
	_AVX_maskstoreps256_(temp._data, *(_int_8_*)mask, a3);
	return temp;
}

//==================================================================================

template<>
inline void Vector2<float>::min(const Vector2<float>& v1, const Vector2<float>& v2, Vector2<float>& res) noexcept
{
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)v1._data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v2._data);
	_single_8_ a3 = _AVX_minps256_(a1, a2);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0, 0 };
	_AVX_maskstoreps256_(res._data, *(_int_8_*)mask, a3);
}

//==================================================================================

template<>
inline void Vector2<float>::max(const Vector2<float>& v1, const Vector2<float>& v2, Vector2<float>& res) noexcept
{
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)v1._data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v2._data);
	_single_8_ a3 = _AVX_maxps256_(a1, a2);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0, 0 };
	_AVX_maskstoreps256_(res._data, *(_int_8_*)mask, a3);
}

//==================================================================================

template<>
inline Vector2<double> Vector2<double>::operator-(const Vector2<double>& v) const noexcept
{
	Vector2<double> temp;
	_double_4_ a1 = _AVX_broadcastpd256_((_double_2_*)this->_data);
	_double_4_ a2 = _AVX_broadcastpd256_((_double_2_*)v._data);
	_double_4_ a3 = _AVX_subpd256_(a1, a2);
	constexpr uint_fast64_t STORE_LABEL = 0xFFFFFFFFFFFFFFFF;
	static const uint_fast64_t mask[4] = { STORE_LABEL, STORE_LABEL, 0, 0 };
	_AVX_maskstorepd256_(temp._data, *(_int_8_*)mask, a3);
	return temp;
}

//==================================================================================

template<>
inline void Vector2<double>::min(const Vector2<double>& v1, const Vector2<double>& v2, Vector2<double>& res) noexcept
{
	_double_4_ a1 = _AVX_broadcastpd256_((_double_2_*)v1._data);
	_double_4_ a2 = _AVX_broadcastpd256_((_double_2_*)v2._data);
	_double_4_ a3 = _AVX_minpd256_(a1, a2);
	constexpr uint_fast64_t STORE_LABEL = 0xFFFFFFFFFFFFFFFF;
	static const uint_fast64_t mask[4] = { STORE_LABEL, STORE_LABEL, 0, 0 };
	_AVX_maskstorepd256_(res._data, *(_int_8_*)mask, a3);
}

//==================================================================================

template<>
inline void Vector2<double>::max(const Vector2<double>& v1, const Vector2<double>& v2, Vector2<double>& res) noexcept
{
	_double_4_ a1 = _AVX_broadcastpd256_((_double_2_*)v1._data);
	_double_4_ a2 = _AVX_broadcastpd256_((_double_2_*)v2._data);
	_double_4_ a3 = _AVX_maxpd256_(a1, a2);
	constexpr uint_fast64_t STORE_LABEL = 0xFFFFFFFFFFFFFFFF;
	static const uint_fast64_t mask[4] = { STORE_LABEL, STORE_LABEL, 0, 0 };
	_AVX_maskstorepd256_(res._data, *(_int_8_*)mask, a3);
}

#else

template<>
inline Vector2<float> Vector2<float>::operator-(const Vector2<float>& v) const noexcept
{
	Vector2<float> temp;
	*(_single_4_*)temp._data = _SSE_subps_(*(_single_4_*)this->_data, *(_single_4_*)v._data);
	return temp;
}

//==================================================================================

template<>
inline void Vector2<float>::min(const Vector2<float>& v1, const Vector2<float>& v2, Vector2<float>& res) noexcept
{
	*(_single_4_*)res._data = _SSE_minps_(*(_single_4_*)v1._data, *(_single_4_*)v2._data);
}

//==================================================================================

template<>
inline void Vector2<float>::max(const Vector2<float>& v1, const Vector2<float>& v2, Vector2<float>& res) noexcept
{
	*(_single_4_*)res._data = _SSE_maxps_(*(_single_4_*)v1._data, *(_single_4_*)v2._data);
}

//==================================================================================

template<>
inline Vector2<double> Vector2<double>::operator-(const Vector2<double>& v) const noexcept
{
	Vector2<double> temp;
	*(_double_2_*)temp._data = _SSE2_subpd_(*(_double_2_*)this->_data, *(_double_2_*)v._data);
	return temp;
}

//==================================================================================

template<>
inline void Vector2<double>::min(const Vector2<double>& v1, const Vector2<double>& v2, Vector2<double>& res) noexcept
{
	*(_double_2_*)res._data = _SSE2_minpd_(*(_double_2_*)v1._data, *(_double_2_*)v2._data);
}

//==================================================================================

template<>
inline void Vector2<double>::max(const Vector2<double>& v1, const Vector2<double>& v2, Vector2<double>& res) noexcept
{
	*(_double_2_*)res._data = _SSE2_maxpd_(*(_double_2_*)v1._data, *(_double_2_*)v2._data);
}

#endif // __AVX__

#elif defined(_M_ARM64) || defined(_M_ARM) || defined (__aarch64__)
#endif // _M_IX86 || _M_AMD64 || __x86_64__ || __i386__

#endif // defined (_USE_SIMD_EXTENTIONS_FOR_CALCULATING_)

//==================================================================================

template<typename T>
inline bool Vector2<T>::operator==(Vector2<T>&& v) const noexcept
{
	return (this == &v) || (memcmp(_data, v._data, sizeof(T) * 2) == 0);
}

//==================================================================================

template<typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& v) const noexcept
{
	Vector2<T> temp;
	temp._data[0] = _data[0] - v._data[0];
	temp._data[1] = _data[1] - v._data[1];
	return temp;
}

//==================================================================================

template<typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const noexcept
{
	Vector2<T> temp;
	temp._data[0] = _data[0] + v._data[0];
	temp._data[1] = _data[1] + v._data[1];
	return temp;
}

//==================================================================================

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v) noexcept
{
	_data[0] += v[0];
	_data[1] += v[1];
	return *this;
}

//==================================================================================

template<typename U>
inline Vector2<U> operator*(U k, const Vector2<U>& v) noexcept
{
	Vector2<U> temp;
	temp._data[0] = k * v._data[0];
	temp._data[1] = k * v._data[1];
	return temp;
}

//==================================================================================

template<typename T>
inline void Vector2<T>::min(const Vector2<T>& v1, const Vector2<T>& v2, Vector2<T>& res) noexcept
{
	res._data[0] = std::min<T>(v1._data[0], v2._data[0]);
	res._data[1] = std::min<T>(v1._data[1], v2._data[1]);
}

//==================================================================================

template<typename T>
inline void Vector2<T>::max(const Vector2<T>& v1, const Vector2<T>& v2, Vector2<T>& res) noexcept
{
	res._data[0] = std::max<T>(v1._data[0], v2._data[0]);
	res._data[1] = std::max<T>(v1._data[1], v2._data[1]);
}


//==================================================================================

template<typename T>
inline Vector2<T> Vector2<T>::min(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	Vector2<T> temp;
	Vector2<T>::min(v1, v2, temp);
	return temp;
}

//==================================================================================

template<typename T>
inline Vector2<T> Vector2<T>::max(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	Vector2<T> temp;
	Vector2<T>::max(v1, v2, temp);
	return temp;
}

//==================================================================================

template<typename T>
inline Vector2<T>::~Vector2() noexcept
{
}

#if defined (_VECTOR2_MACRO_MAX_HAS_PUSHED_)
#pragma pop_macro("max")
#undef _VECTOR2_MACRO_MAX_HAS_PUSHED_
#endif // _VECTOR2_MACRO_MAX_HAS_PUSHED_
#if defined (_VECTOR2_MACRO_MIN_HAS_PUSHED_)
#pragma pop_macro("min")
#undef _VECTOR2_MACRO_MIN_HAS_PUSHED_
#endif // _VECTOR2_MACRO_MIN_HAS_PUSHED_

#endif // VECTOR2_H_
