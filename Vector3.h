#pragma once
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <algorithm>
#include <cstring>
#include <type_traits>

#include "x86_64_intrin.h"

#if defined (min)
#pragma push_macro("min")
#undef min
#define _VECTOR3_MACRO_MIN_HAS_PUSHED_ 1
#endif // min
#if defined (max)
#pragma push_macro("max")
#undef max
#define _VECTOR3_MACRO_MAX_HAS_PUSHED_ 1
#endif // max

#ifndef _VECTOR3_ALIGNMENT_
#if defined(__AVX512BW__) || defined(__AVX512CD__) || defined(__AVX512DQ__) || defined(__AVX512F__) || defined(__AVX512VL__)
#define _VECTOR3_ALIGNMENT_ 64
#elif defined (__AVX__)
#define _VECTOR3_ALIGNMENT_ 32
#else
#define _VECTOR3_ALIGNMENT_ 16
#endif //
#endif // _VECTOR3_ALIGNMENT_

template <typename T>
class Vector3
{
	static_assert(std::is_arithmetic<T>::value, "Vector3<T>: T must be an arithmetic type");
public:
	using valueType = T;
	enum { ALIGNMENT = sizeof(T) * 4 };
protected:
	enum {ARRAY_SIZE = 4};
	alignas(ALIGNMENT) T _data[ARRAY_SIZE];
	void _moveProc(Vector3<T>& a) noexcept;
	void _copyProc(const Vector3<T>& a) noexcept;
public:
	Vector3() noexcept;
	Vector3(const Vector3<T>& p) noexcept;
	Vector3(Vector3<T>&& p) noexcept;
	Vector3(T x_v, T y_v, T z_v) noexcept;
	Vector3(const T* p) noexcept;
	Vector3<T>& operator=(const Vector3<T>& p) noexcept;
	Vector3<T>& operator=(Vector3<T>&& p) noexcept;
	bool operator==(const Vector3<T>& p) const noexcept;
	T x() const noexcept;
	T y() const noexcept;
	T z() const noexcept;
	const T* data() const noexcept;
	T* data() noexcept;
	const T& operator[](size_t idx) const;
	T& operator[](size_t idx);
	Vector3<T> operator-(const Vector3<T>& v) const noexcept;
	Vector3<T> operator+(const Vector3<T>& v) const noexcept;
	template <typename U>
	friend Vector3<U> operator*(U k, const Vector3<U>& v) noexcept;
	static void min(const Vector3<T>& v1, const Vector3<T>& v2, Vector3<T>& res) noexcept;
	static void max(const Vector3<T>& v1, const Vector3<T>& v2, Vector3<T>& res) noexcept;
	static Vector3<T> min(const Vector3<T>& v1, const Vector3<T>& v2) noexcept;
	static Vector3<T> max(const Vector3<T>& v1, const Vector3<T>& v2) noexcept;
	~Vector3() noexcept;
};

//==================================================================================

template<typename T>
void Vector3<T>::_moveProc(Vector3<T>& a) noexcept
{
	memcpy(_data, a._data, sizeof(_data));
}

//==================================================================================

template<typename T>
void Vector3<T>::_copyProc(const Vector3<T>& a) noexcept
{
	memcpy(_data, a._data, sizeof(_data));
}

//==================================================================================

template<typename T>
Vector3<T>::Vector3() noexcept
{
	memset(_data, 0, sizeof(_data));
}

//==================================================================================

template<typename T>
Vector3<T>::Vector3(const Vector3<T>& p) noexcept
{
	_copyProc(p);
}

//==================================================================================

template<typename T>
Vector3<T>::Vector3(Vector3<T>&& p) noexcept
{
	_moveProc(p);
}

//==================================================================================

template<typename T>
Vector3<T>::Vector3(T x_v, T y_v, T z_v) noexcept
{
	memset(_data, 0, sizeof(_data));
	_data[0] = x_v;
	_data[1] = y_v;
	_data[2] = z_v;
}

//==================================================================================

template<typename T>
Vector3<T>::Vector3(const T* p) noexcept
{
	memset(_data, 0, sizeof(_data));
	memcpy(_data, p, sizeof(T) * 3);
}

//==================================================================================

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& p) noexcept
{
	if (this != &p) {
		_copyProc(p);
	}
	return *this;
}

//==================================================================================

template<typename T>
Vector3<T>& Vector3<T>::operator=(Vector3<T>&& p) noexcept
{
	if (this != &p) {
		_moveProc(p);
	}
	return *this;
}

//==================================================================================

template<typename T>
bool Vector3<T>::operator==(const Vector3<T>& p) const noexcept
{
	return (this == &p) || (memcmp(_data, p._data, sizeof(T) * 3) == 0);
}

//==================================================================================

template<typename T>
inline T Vector3<T>::x() const noexcept
{
	return _data[0];
}

//==================================================================================

template<typename T>
inline T Vector3<T>::y() const noexcept
{
	return _data[1];
}

//==================================================================================

template<typename T>
inline T Vector3<T>::z() const noexcept
{
	return _data[2];
}

//==================================================================================

template<typename T>
inline const T* Vector3<T>::data() const noexcept
{
	return _data;
}

//==================================================================================

template<typename T>
inline T* Vector3<T>::data() noexcept
{
	return _data;
}

//==================================================================================

template<typename T>
inline const T& Vector3<T>::operator[](size_t idx) const
{
	return _data[idx];
}

//==================================================================================

template<typename T>
inline T& Vector3<T>::operator[](size_t idx)
{
	return _data[idx];
}

//==================================================================================
#if defined (_USE_SIMD_EXTENTIONS_FOR_CALCULATING_)
#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)
#ifdef __AVX__

#pragma region float_AVX

template<>
inline Vector3<float> Vector3<float>::operator-(const Vector3<float>& v) const noexcept
{
	Vector3<float> temp;
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)this->_data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v._data);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0 };
	_single_8_ a3 = _AVX_subps256_(a1, a2);
	_AVX_maskstoreps256_(temp._data, *(_int_8_*)mask, a3);
	return temp;
}

//==================================================================================

template<>
inline void Vector3<float>::min(const Vector3<float>& v1, const Vector3<float>& v2, Vector3<float>& res) noexcept
{
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)v1._data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v2._data);
	_single_8_ a3 = _AVX_minps256_(a1, a2);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0 };
	_AVX_maskstoreps256_(res._data, *(_int_8_*)mask, a3);
}

//==================================================================================

template<>
inline void Vector3<float>::max(const Vector3<float>& v1, const Vector3<float>& v2, Vector3<float>& res) noexcept
{
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)v1._data);
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)v2._data);
	_single_8_ a3 = _AVX_maxps256_(a1, a2);
	constexpr unsigned int STORE_LABEL = 0xFFFFFFFF;
	static const unsigned int mask[8] = { STORE_LABEL, STORE_LABEL, STORE_LABEL, 0, 0, 0, 0, 0 };
	_AVX_maskstoreps256_(res._data, *(_int_8_*)mask, a3);
}

#pragma endregion

//==================================================================================

#pragma region double_AVX

template<>
inline Vector3<double> Vector3<double>::operator-(const Vector3<double>& v) const noexcept
{
	Vector3<double> temp;
	*(_double_4_*)temp._data = _AVX_subpd256_(*(_double_4_*)this->_data, *(_double_4_*)v._data);
	return temp;
}

//==================================================================================

template<>
inline void Vector3<double>::min(const Vector3<double>& v1, const Vector3<double>& v2, Vector3<double>& res) noexcept
{
	*(_double_4_*)res._data = _AVX_minpd256_(*(_double_4_*)v1._data, *(_double_4_*)v2._data);
}

//==================================================================================

template<>
inline void Vector3<double>::max(const Vector3<double>& v1, const Vector3<double>& v2, Vector3<double>& res) noexcept
{
	*(_double_4_*)res._data = _AVX_maxpd256_(*(_double_4_*)v1._data, *(_double_4_*)v2._data);
}

#pragma endregion

#else

#pragma region float_SSE

template<>
inline Vector3<float> Vector3<float>::operator-(const Vector3<float>& v) const noexcept
{
	Vector3<float> temp;
	*(_single_4_*)temp._data = _SSE_subps_(*(_single_4_*)this->_data, *(_single_4_*)v._data);
	return temp;
}

//==================================================================================

template<>
inline void Vector3<float>::min(const Vector3<float>& v1, const Vector3<float>& v2, Vector3<float>& res) noexcept
{
	*(_single_4_*)res._data = _SSE_minps_(*(_single_4_*)v1._data, *(_single_4_*)v2._data);
}

//==================================================================================

template<>
inline void Vector3<float>::max(const Vector3<float>& v1, const Vector3<float>& v2, Vector3<float>& res) noexcept
{
	*(_single_4_*)res._data = _SSE_maxps_(*(_single_4_*)v1._data, *(_single_4_*)v2._data);
}

#pragma endregion

//==================================================================================

#pragma region double_SSE

template<>
inline void Vector3<double>::min(const Vector3<double>& v1, const Vector3<double>& v2, Vector3<double>& res) noexcept
{
	*(_double_2_*)res._data = _SSE2_minpd_(*(_double_2_*)v1._data, *(_double_2_*)v2._data);
	*(_double_2_*)(res._data + 2) = _SSE2_minpd_(*(_double_2_*)(v1._data + 2), *(_double_2_*)(v2._data + 2));
}

//==================================================================================

template<>
inline void Vector3<double>::max(const Vector3<double>& v1, const Vector3<double>& v2, Vector3<double>& res) noexcept
{
	*(_double_2_*)res._data = _SSE2_maxpd_(*(_double_2_*)v1._data, *(_double_2_*)v2._data);
	*(_double_2_*)(res._data + 2) = _SSE2_maxpd_(*(_double_2_*)(v1._data + 2), *(_double_2_*)(v2._data + 2));
}

//==================================================================================

template<>
inline Vector3<double> Vector3<double>::operator-(const Vector3<double>& v) const noexcept
{
	Vector3<double> temp;
	*(_double_2_*)temp._data = _SSE2_subpd_(*(_double_2_*)this->_data, *(_double_2_*)v._data);
	*(_double_2_*)(temp._data + 2) = _SSE2_subpd_(*(_double_2_*)(this->_data + 2), *(_double_2_*)(v._data + 2));
	return temp;
}

#pragma endregion

#endif // __AVX__
#elif defined(_M_ARM64) || defined(_M_ARM) || defined (__aarch64__)
#endif // defined (_M_IX86) || (_M_AMD64) || (__x86_64__) || defined(__i386__)
#endif // _USE_SIMD_EXTENTIONS_FOR_CALCULATING_
//==================================================================================

template<typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const noexcept
{
	Vector3<T> temp(*this);
	temp._data[0] -= v._data[0];
	temp._data[1] -= v._data[1];
	temp._data[2] -= v._data[2];
	return temp;
}

//==================================================================================

template<typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const noexcept
{
	Vector3<T> temp(*this);
	temp._data[0] += v._data[0];
	temp._data[1] += v._data[1];
	temp._data[2] += v._data[2];
	return temp;
}

//==================================================================================

template<typename U>
inline Vector3<U> operator*(U k, const Vector3<U>& v) noexcept
{
	Vector3<U> temp;
	temp._data[0] = k * v._data[0];
	temp._data[1] = k * v._data[1];
	temp._data[2] = k * v._data[2];
	return temp;
}

//==================================================================================

template<typename T>
inline void Vector3<T>::min(const Vector3<T>& v1, const Vector3<T>& v2, Vector3<T>& res) noexcept
{
	res._data[0] = std::min<T>(v1._data[0], v2._data[0]);
	res._data[1] = std::min<T>(v1._data[1], v2._data[1]);
	res._data[2] = std::min<T>(v1._data[2], v2._data[2]);
}

//==================================================================================

template<typename T>
inline void Vector3<T>::max(const Vector3<T>& v1, const Vector3<T>& v2, Vector3<T>& res) noexcept
{
	res._data[0] = std::max<T>(v1._data[0], v2._data[0]);
	res._data[1] = std::max<T>(v1._data[1], v2._data[1]);
	res._data[2] = std::max<T>(v1._data[2], v2._data[2]);
}

//==================================================================================

template<typename T>
inline Vector3<T> Vector3<T>::min(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	Vector3<T> temp;
	Vector3<T>::min(v1, v2, temp);
	return temp;
}

//==================================================================================

template<typename T>
inline Vector3<T> Vector3<T>::max(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	Vector3<T> temp;
	Vector3<T>::max(v1, v2, temp);
	return temp;
}

//==================================================================================

template<typename T>
Vector3<T>::~Vector3() noexcept
{
}

#if defined (_VECTOR3_MACRO_MAX_HAS_PUSHED_)
#pragma pop_macro("max")
#undef _VECTOR3_MACRO_MAX_HAS_PUSHED_
#endif // _VECTOR3_MACRO_MAX_HAS_PUSHED_
#if defined (_VECTOR3_MACRO_MIN_HAS_PUSHED_)
#pragma pop_macro("min")
#undef _VECTOR3_MACRO_MIN_HAS_PUSHED_
#endif // _VECTOR3_MACRO_MIN_HAS_PUSHED_

#endif // VECTOR3_H_