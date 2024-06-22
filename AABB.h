#pragma once
#ifndef _AABB_H_
#define _AABB_H_

// Axis Aligned Bound Box

#include "Vector3.h"
#include "x86_64_intrin.h"

#include <cstdint>
#include <type_traits>

#if defined (min)
#pragma push_macro("min")
#undef min
#define _AABB_MACRO_MIN_HAS_PUSHED_ 1
#endif // min
#if defined (max)
#pragma push_macro("max")
#undef max
#define _AABB_MACRO_MAX_HAS_PUSHED_ 1
#endif // max

template <typename T>
class AABB
{
	static_assert(std::is_arithmetic<T>::value, "AABB<T>: T must be an arithmetic type");
public:
	using valueType = T;
	using elementType = Vector3<T>;
protected:
	T _volume;
	enum { _DATA_SIZE = 2 };
	Vector3<T> _data[_DATA_SIZE];
	Vector3<T> _measures;

	void _copyProc(const AABB<T>& aabb) noexcept;
	void _moveProc(AABB<T>& aabb) noexcept;
public:
	AABB() noexcept;
	AABB(T left, T bottom, T front, T right, T top, T back) noexcept;
	AABB(const Vector3<T>& point1, const Vector3<T>& point2) noexcept;
	AABB(const T* point1, const T* point2) noexcept;
	AABB(const AABB<T>& aabb);
	AABB(AABB<T>&& aabb) noexcept;
	AABB<T>& operator=(const AABB<T>& aabb);
	AABB<T>& operator=(AABB<T>&& aabb) noexcept;
	bool operator==(const AABB<T>& aabb) const noexcept;
	const Vector3<T>& operator[](size_t idx) const noexcept;
	Vector3<T>& operator[](size_t idx) noexcept;
	bool pointInside(const Vector3<T>& point) const noexcept;
	bool pointInside(const T* p) const noexcept;
	bool pointInside(T x, T y, T z) const noexcept;
	bool intersection(const AABB<T>& aabb) const noexcept;
	static bool intersection(const AABB<T>& aabb1, const AABB<T>& aabb2) noexcept;
	bool intersectionAABB(const AABB<T>& aabb1, AABB<T>& aabb2) const noexcept;
	static bool intersectionAABB(const AABB<T>& aabb1, const AABB<T>& aabb2, AABB<T>& aabb3) noexcept;
	AABB<T> boundAABB(const AABB<T>& aabb) const noexcept;
	static void boundAABB(const AABB<T>& aabb1, const AABB<T>& aabb2, AABB<T>& aabb3) noexcept;
	T volume() const noexcept;
	T width() const noexcept;
	T height() const noexcept;
	T depth() const noexcept;
	const T* measures() const noexcept;
	const Vector3<T>& LBF() const noexcept;
	const Vector3<T>& RTB() const noexcept;

	~AABB() noexcept;
};

//==================================================================================

template<typename T>
void AABB<T>::_copyProc(const AABB<T>& aabb) noexcept
{
	_data[0] = aabb._data[0];
	_data[1] = aabb._data[1];
	_measures = aabb._measures;
	_volume = aabb._volume;
}

//==================================================================================

template<typename T>
void AABB<T>::_moveProc(AABB<T>& aabb) noexcept
{
	_data[0] = std::move(aabb._data[0]);
	_data[1] = std::move(aabb._data[1]);
	_volume = aabb._volume;
	_measures = aabb._measures;
}

//==================================================================================

template<typename T>
AABB<T>::AABB() noexcept :
	_volume((T)0)
{
}

//==================================================================================

template<typename T>
AABB<T>::AABB(T left, T bottom, T front, T right, T top, T back) noexcept :
	_volume((T)0)
{
	Vector3<T> point1(left, bottom, front);
	Vector3<T> point2(right, top, back);
	Vector3<T>::min(point1, point2, _data[0]);
	Vector3<T>::max(point1, point2, _data[1]);
	_measures = _data[1] - _data[0];
	_volume = _measures.x() * _measures.y() * _measures.z();
}

//==================================================================================

template<typename T>
AABB<T>::AABB(const Vector3<T>& point1, const Vector3<T>& point2) noexcept :
	_volume((T)0)
{
	Vector3<T>::min(point1, point2, _data[0]);
	Vector3<T>::max(point1, point2, _data[1]);
	_measures = _data[1] - _data[0];
	_volume = _measures.x() * _measures.y() * _measures.z();
}

//==================================================================================

template<typename T>
inline AABB<T>::AABB(const T* point1, const T* point2) noexcept
{
	Vector3<T> v1(point1);
	Vector3<T> v2(point2);
	Vector3<T>::min(v1, v2, _data[0]);
	Vector3<T>::max(v1, v2, _data[1]);
	_measures = _data[1] - _data[0];
	_volume = _measures.x() * _measures.y() * _measures.z();
}

//==================================================================================

template<typename T>
AABB<T>::AABB(const AABB<T>& aabb) :
	_volume((T)0)
{
	_copyProc(aabb);
}

//==================================================================================

template<typename T>
AABB<T>::AABB(AABB<T>&& aabb) noexcept :
	_volume((T)0)
{
	_moveProc(aabb);
}

//==================================================================================

template<typename T>
AABB<T>& AABB<T>::operator=(const AABB<T>& aabb)
{
	if (this != &aabb) {
		_copyProc(aabb);
	}
	return *this;
}

//==================================================================================

template<typename T>
AABB<T>& AABB<T>::operator=(AABB<T>&& aabb) noexcept
{
	if (this != &aabb) {
		_moveProc(aabb);
	}
	return *this;
}

//==================================================================================

template<typename T>
bool AABB<T>::operator==(const AABB<T>& aabb) const noexcept
{
	return (this == &aabb) || (_data[0] == aabb._data[0]) && (_data[1] == aabb._data[1]);
}

//==================================================================================

template<typename T>
inline const Vector3<T>& AABB<T>::operator[](size_t idx) const noexcept
{
	return _data[idx];
}

//==================================================================================

template<typename T>
inline Vector3<T>& AABB<T>::operator[](size_t idx) noexcept
{
	return _data[idx];
}

//==================================================================================

#if defined (_USE_SIMD_EXTENTIONS_FOR_CALCULATING_)

#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)
#if defined (__AVX__)

#pragma region float_AVX
template<>
inline bool AABB<float>::pointInside(const Vector3<float>& point) const noexcept
{
	_single_8_ pt = _AVX_broadcastps256_((_single_4_*)point.data());
	_single_8_ b1 = _AVX_broadcastps256_((_single_4_*)_data[0].data());
	_single_8_ b2 = _AVX_broadcastps256_((_single_4_*)_data[1].data());
	_single_8_ r1 = _AVX_cmpps256_(pt, b1, 13);
	_single_8_ r2 = _AVX_cmpps256_(pt, b2, 2);
	_single_8_ r3 = _AVX_andps256_(r1, r2);
	int m = _AVX_movemaskps256_(r3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::pointInside(const float* p) const noexcept
{
	_single_8_ pt = _AVX_broadcastps256_((_single_4_*)p);
	_single_8_ b1 = _AVX_broadcastps256_((_single_4_*)_data[0].data());
	_single_8_ b2 = _AVX_broadcastps256_((_single_4_*)_data[1].data());
	_single_8_ r1 = _AVX_cmpps256_(pt, b1, 13);
	_single_8_ r2 = _AVX_cmpps256_(pt, b2, 2);
	_single_8_ r3 = _AVX_andps256_(r1, r2);
	int m = _AVX_movemaskps256_(r3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::pointInside(float x, float y, float z) const noexcept
{
	Vector3<float> v(x, y, z);
	_single_8_ pt = _AVX_broadcastps256_((_single_4_*)v.data());
	_single_8_ b1 = _AVX_broadcastps256_((_single_4_*)_data[0].data());
	_single_8_ b2 = _AVX_broadcastps256_((_single_4_*)_data[1].data());
	_single_8_ r1 = _AVX_cmpps256_(pt, b1, 13);
	_single_8_ r2 = _AVX_cmpps256_(pt, b2, 2);
	_single_8_ r3 = _AVX_andps256_(r1, r2);
	int m = _AVX_movemaskps256_(r3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersection(const AABB<float>& aabb) const noexcept
{
	if (this == &aabb) {
		return true;
	}
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)_data[0].data());
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)_data[1].data());
	_single_8_ b1 = _AVX_broadcastps256_((_single_4_*)aabb._data[0].data());
	_single_8_ b2 = _AVX_broadcastps256_((_single_4_*)aabb._data[1].data());
	_single_8_ r11 = _AVX_cmpps256_(a1, b1, 13);
	_single_8_ r12 = _AVX_cmpps256_(a1, b2, 2);
	r11 = _AVX_andps256_(r11, r12);
	_single_8_ r21 = _AVX_cmpps256_(b1, a1, 13);
	_single_8_ r22 = _AVX_cmpps256_(b1, a2, 2);
	r21 = _AVX_andps256_(r21, r22);
	r12 = _AVX_orps256_(r11, r21);
	int m = _AVX_movemaskps256_(r12);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersectionAABB(const AABB<float>& aabb1, AABB<float>& aabb2) const noexcept
{
	if (this == &aabb1) {
		aabb2 = aabb1;
		return true;
	}
	if (!intersection(aabb1)) {
		return false;
	}
	Vector3<float>::max(_data[0], aabb1._data[0], aabb2._data[0]);
	Vector3<float>::min(_data[1], aabb1._data[1], aabb2._data[1]);
	aabb2._measures = aabb2._data[1] - aabb2._data[0];
	aabb2._volume = aabb2._measures.x() * aabb2._measures.y() * aabb2._measures.z();
	return true;
}

//==================================================================================

template<>
inline bool AABB<float>::intersection(const AABB<float>& aabb1, const AABB<float>& aabb2) noexcept
{
	if (&aabb1 == &aabb2) {
		return true;
	}
	_single_8_ a1 = _AVX_broadcastps256_((_single_4_*)aabb1._data[0].data());
	_single_8_ a2 = _AVX_broadcastps256_((_single_4_*)aabb1._data[1].data());
	_single_8_ b1 = _AVX_broadcastps256_((_single_4_*)aabb2._data[0].data());
	_single_8_ b2 = _AVX_broadcastps256_((_single_4_*)aabb2._data[1].data());
	_single_8_ r11 = _AVX_cmpps256_(a1, b1, 13);
	_single_8_ r12 = _AVX_cmpps256_(a1, b2, 2);
	r11 = _AVX_andps256_(r11, r12);
	_single_8_ r21 = _AVX_cmpps256_(b1, a1, 13);
	_single_8_ r22 = _AVX_cmpps256_(b1, a2, 2);
	r21 = _AVX_andps256_(r21, r22);
	r12 = _AVX_orps256_(r11, r21);
	int m = _AVX_movemaskps256_(r12);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersectionAABB(const AABB<float>& aabb1, const AABB<float>& aabb2, AABB<float>& aabb3) noexcept
{
	if (&aabb1 == &aabb2) {
		aabb3 = aabb1;
		return true;
	}
	if (!intersection(aabb1, aabb2)) {
		return false;
	}
	Vector3<float>::max(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<float>::min(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
	return true;
}

#pragma endregion float_AVX

//==================================================================================
#pragma region double_AVX

template<>
inline bool AABB<double>::pointInside(const Vector3<double>& point) const noexcept
{
	_double_4_ f1 = _AVX_cmppd256_(*(_double_4_*)point.data(), *(_double_4_*)_data[0].data(), 13);
	_double_4_ f2 = _AVX_cmppd256_(*(_double_4_*)point.data(), *(_double_4_*)_data[1].data(), 2);
	_double_4_ f3 = _AVX_andpd256_(f1, f2);
	int m = _AVX_movemaskpd256_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<double>::pointInside(const double* p) const noexcept
{
	_double_4_ f1 = _AVX_cmppd256_(*(_double_4_*)p, *(_double_4_*)_data[0].data(), 13);
	_double_4_ f2 = _AVX_cmppd256_(*(_double_4_*)p, *(_double_4_*)_data[1].data(), 2);
	_double_4_ f3 = _AVX_andpd256_(f1, f2);
	int m = _AVX_movemaskpd256_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<double>::pointInside(double x, double y, double z) const noexcept
{
	Vector3<double> v(x, y, z);
	_double_4_ f1 = _AVX_cmppd256_(*(_double_4_*)v.data(), *(_double_4_*)_data[0].data(), 13);
	_double_4_ f2 = _AVX_cmppd256_(*(_double_4_*)v.data(), *(_double_4_*)_data[1].data(), 2);
	_double_4_ f3 = _AVX_andpd256_(f1, f2);
	int m = _AVX_movemaskpd256_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<double>::intersection(const AABB<double>& aabb) const noexcept
{
	if (this == &aabb) {
		return true;
	}
	_double_4_ f11 = _AVX_cmppd256_(*(_double_4_*)_data[0].data(), *(_double_4_*)aabb._data[0].data(), 13);
	_double_4_ f12 = _AVX_cmppd256_(*(_double_4_*)_data[0].data(), *(_double_4_*)aabb._data[1].data(), 2);
	_double_4_ f13 = _AVX_andpd256_(f11, f12);
	_double_4_ f21 = _AVX_cmppd256_(*(_double_4_*)aabb._data[0].data(), *(_double_4_*)_data[0].data(), 13);
	_double_4_ f22 = _AVX_cmppd256_(*(_double_4_*)aabb._data[0].data(), *(_double_4_*)_data[1].data(), 2);
	_double_4_ f23 = _AVX_andpd256_(f21, f22);
	_double_4_ res = _AVX_orpd256_(f13, f23);
	int m = _AVX_movemaskpd256_(res);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<double>::intersectionAABB(const AABB<double>& aabb1, AABB<double>& aabb2) const noexcept
{
	if (this == &aabb1) {
		aabb2 = aabb1;
		return true;
	}
	if (!intersection(aabb1)) {
		return false;
	}
	Vector3<double>::max(_data[0], aabb1._data[0], aabb2._data[0]);
	Vector3<double>::min(_data[1], aabb1._data[1], aabb2._data[1]);
	aabb2._measures = aabb2._data[1] - aabb2._data[0];
	aabb2._volume = aabb2._measures.x() * aabb2._measures.y() * aabb2._measures.z();
	return true;
}

//==================================================================================

template<>
inline bool AABB<double>::intersection(const AABB<double>& aabb1, const AABB<double>& aabb2) noexcept
{
	if (&aabb1 == &aabb2) {
		return true;
	}
	_double_4_ f11 = _AVX_cmppd256_(*(_double_4_*)aabb1._data[0].data(), *(_double_4_*)aabb2._data[0].data(), 13);
	_double_4_ f12 = _AVX_cmppd256_(*(_double_4_*)aabb1._data[0].data(), *(_double_4_*)aabb2._data[1].data(), 2);
	_double_4_ f13 = _AVX_andpd256_(f11, f12);
	_double_4_ f21 = _AVX_cmppd256_(*(_double_4_*)aabb2._data[0].data(), *(_double_4_*)aabb1._data[0].data(), 13);
	_double_4_ f22 = _AVX_cmppd256_(*(_double_4_*)aabb2._data[0].data(), *(_double_4_*)aabb1._data[1].data(), 2);
	_double_4_ f23 = _AVX_andpd256_(f21, f22);
	_double_4_ res = _AVX_orpd256_(f13, f23);
	int m = _AVX_movemaskpd256_(res);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<double>::intersectionAABB(const AABB<double>& aabb1, const AABB<double>& aabb2, AABB<double>& aabb3) noexcept
{
	if (&aabb1 == &aabb2) {
		aabb3 = aabb1;
		return true;
	}
	if (!intersection(aabb1, aabb2)) {
		return false;
	}
	Vector3<double>::max(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<double>::min(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
	return true;
}
#pragma endregion double_AVX

#else

#pragma region float_SSE
template<>
inline bool AABB<float>::pointInside(const Vector3<float>& point) const noexcept
{
	_single_4_ f1 = _AVX_cmpps_(*(_single_4_*)point.data(), *(_single_4_*)_data[0].data(), 13);
	_single_4_ f2 = _AVX_cmpps_(*(_single_4_*)point.data(), *(_single_4_*)_data[1].data(), 2);
	_single_4_ f3 = _SSE_andps_(f1, f2);
	int m = _SSE_movemaskps_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::pointInside(const float* p) const noexcept
{
	_single_4_ f1 = _AVX_cmpps_(*(_single_4_*)p, *(_single_4_*)_data[0].data(), 13);
	_single_4_ f2 = _AVX_cmpps_(*(_single_4_*)p, *(_single_4_*)_data[1].data(), 2);
	_single_4_ f3 = _SSE_andps_(f1, f2);
	int m = _SSE_movemaskps_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::pointInside(float x, float y, float z) const noexcept
{
	Vector3<float> v(x, y, z);
	_single_4_ f1 = _AVX_cmpps_(*(_single_4_*)v.data(), *(_single_4_*)_data[0].data(), 13);
	_single_4_ f2 = _AVX_cmpps_(*(_single_4_*)v.data(), *(_single_4_*)_data[1].data(), 2);
	_single_4_ f3 = _SSE_andps_(f1, f2);
	int m = _SSE_movemaskps_(f3);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersection(const AABB<float>& aabb) const noexcept
{
	if (this == &aabb) {
		return true;
	}
	_single_4_ f11 = _AVX_cmpps_(*(_single_4_*)_data[0].data(), *(_single_4_*)aabb._data[0].data(), 13);
	_single_4_ f12 = _AVX_cmpps_(*(_single_4_*)_data[0].data(), *(_single_4_*)aabb._data[1].data(), 2);
	_single_4_ f13 = _SSE_andps_(f11, f12);
	_single_4_ f21 = _AVX_cmpps_(*(_single_4_*)aabb._data[0].data(), *(_single_4_*)_data[0].data(), 13);
	_single_4_ f22 = _AVX_cmpps_(*(_single_4_*)aabb._data[0].data(), *(_single_4_*)_data[1].data(), 2);
	_single_4_ f23 = _SSE_andps_(f21, f22);
	_single_4_ res = _SSE_orps_(f13, f23);
	int m = _SSE_movemaskps_(res);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersectionAABB(const AABB<float>& aabb1, AABB<float>& aabb2) const noexcept
{
	if (this == &aabb1) {
		aabb2 = aabb1;
		return true;
	}
	if (!intersection(aabb1)) {
		return false;
	}
	Vector3<float>::max(_data[0], aabb1._data[0], aabb2._data[0]);
	Vector3<float>::min(_data[1], aabb1._data[1], aabb2._data[1]);
	aabb2._measures = aabb2._data[1] - aabb2._data[0];
	aabb2._volume = aabb2._measures.x() * aabb2._measures.y() * aabb2._measures.z();
	return true;
}

//==================================================================================

template<>
inline bool AABB<float>::intersection(const AABB<float>& aabb1, const AABB<float>& aabb2) noexcept
{
	if (&aabb1 == &aabb2) {
		return true;
	}
	_single_4_ f11 = _AVX_cmpps_(*(_single_4_*)aabb1._data[0].data(), *(_single_4_*)aabb2._data[0].data(), 13);
	_single_4_ f12 = _AVX_cmpps_(*(_single_4_*)aabb1._data[0].data(), *(_single_4_*)aabb2._data[1].data(), 2);
	_single_4_ f13 = _SSE_andps_(f11, f12);
	_single_4_ f21 = _AVX_cmpps_(*(_single_4_*)aabb2._data[0].data(), *(_single_4_*)aabb1._data[0].data(), 13);
	_single_4_ f22 = _AVX_cmpps_(*(_single_4_*)aabb2._data[0].data(), *(_single_4_*)aabb1._data[1].data(), 2);
	_single_4_ f23 = _SSE_andps_(f21, f22);
	_single_4_ res = _SSE_orps_(f13, f23);
	int m = _SSE_movemaskps_(res);
	return (m & 0x7) == 0x7;
}

//==================================================================================

template<>
inline bool AABB<float>::intersectionAABB(const AABB<float>& aabb1, const AABB<float>& aabb2, AABB<float>& aabb3) noexcept
{
	if (&aabb1 == &aabb2) {
		aabb3 = aabb1;
		return true;
	}
	if (!intersection(aabb1, aabb2)) {
		return false;
	}
	Vector3<float>::max(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<float>::min(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
	return true;
}

#pragma endregion float_SSE

//==================================================================================

#pragma region double_SSE
template<>
inline bool AABB<double>::pointInside(const Vector3<double>& point) const noexcept
{
	_double_2_ f1[2];
	f1[0] = _AVX_cmppd_(*(_double_2_*)point.data(), *(_double_2_*)_data[0].data(), 13);
	f1[1] = _AVX_cmppd_(*(_double_2_*)(point.data() + 2), *(_double_2_*)(_data[0].data() + 2), 13);
	_double_2_ f2[2];
	f2[0] = _AVX_cmppd_(*(_double_2_*)point.data(), *(_double_2_*)_data[1].data(), 2);
	f2[1] = _AVX_cmppd_(*(_double_2_*)(point.data() + 2), *(_double_2_*)(_data[1].data() + 2), 2);
	_double_2_ f3[2];
	f3[0] = _SSE2_andpd_(f1[0], f2[0]);
	f3[1] = _SSE2_andpd_(f1[1], f2[1]);
	int m1 = _SSE2_movemaskpd_(f3[0]);
	int m2 = _SSE2_movemaskpd_(f3[1]);
	return (m1 == 3) && ((m2 & 1) == 1);
}

//==================================================================================

template<>
inline bool AABB<double>::pointInside(const double* p) const noexcept
{
	_double_2_ f1[2];
	f1[0] = _AVX_cmppd_(*(_double_2_*)p, *(_double_2_*)_data[0].data(), 13);
	f1[1] = _AVX_cmppd_(*(_double_2_*)(p + 2), *(_double_2_*)(_data[0].data() + 2), 13);
	_double_2_ f2[2];
	f2[0] = _AVX_cmppd_(*(_double_2_*)p, *(_double_2_*)_data[1].data(), 2);
	f2[1] = _AVX_cmppd_(*(_double_2_*)(p + 2), *(_double_2_*)(_data[1].data() + 2), 2);
	_double_2_ f3[2];
	f3[0] = _SSE2_andpd_(f1[0], f2[0]);
	f3[1] = _SSE2_andpd_(f1[1], f2[1]);
	int m1 = _SSE2_movemaskpd_(f3[0]);
	int m2 = _SSE2_movemaskpd_(f3[1]);
	return (m1 == 3) && ((m2 & 1) == 1);
}

//==================================================================================

template<>
inline bool AABB<double>::pointInside(double x, double y, double z) const noexcept
{
	Vector3<double> v(x, y, z);
	_double_2_ f1[2];
	f1[0] = _AVX_cmppd_(*(_double_2_*)v.data(), *(_double_2_*)_data[0].data(), 13);
	f1[1] = _AVX_cmppd_(*(_double_2_*)(v.data() + 2), *(_double_2_*)(_data[0].data() + 2), 13);
	_double_2_ f2[2];
	f2[0] = _AVX_cmppd_(*(_double_2_*)v.data(), *(_double_2_*)_data[1].data(), 2);
	f2[1] = _AVX_cmppd_(*(_double_2_*)(v.data() + 2), *(_double_2_*)(_data[1].data() + 2), 2);
	_double_2_ f3[2];
	f3[0] = _SSE2_andpd_(f1[0], f2[0]);
	f3[1] = _SSE2_andpd_(f1[1], f2[1]);
	int m1 = _SSE2_movemaskpd_(f3[0]);
	int m2 = _SSE2_movemaskpd_(f3[1]);
	return (m1 == 3) && ((m2 & 1) == 1);
}

//==================================================================================

template<>
inline bool AABB<double>::intersection(const AABB<double>& aabb) const noexcept
{
	if (this == &aabb) {
		return true;
	}
	_double_2_ f11[2];
	f11[0] = _AVX_cmppd_(*(_double_2_*)_data[0].data(), *(_double_2_*)aabb._data[0].data(), 13);
	f11[1] = _AVX_cmppd_(*(_double_2_*)(_data[0].data() + 2), *(_double_2_*)(aabb._data[0].data() + 2), 13);
	_double_2_ f12[2];
	f12[0] = _AVX_cmppd_(*(_double_2_*)_data[0].data(), *(_double_2_*)aabb._data[1].data(), 2);
	f12[1] = _AVX_cmppd_(*(_double_2_*)(_data[0].data() + 2), *(_double_2_*)(aabb._data[1].data() + 2), 2);
	_double_2_ f13[2];
	f13[0] = _SSE2_andpd_(f11[0], f12[0]);
	f13[1] = _SSE2_andpd_(f11[1], f12[1]);
	_double_2_ f21[2];
	f21[0] = _AVX_cmppd_(*(_double_2_*)aabb._data[0].data(), *(_double_2_*)_data[0].data(), 13);
	f21[1] = _AVX_cmppd_(*(_double_2_*)(aabb._data[0].data() + 2), *(_double_2_*)(_data[0].data() + 2), 13);
	_double_2_ f22[2];
	f22[0] = _AVX_cmppd_(*(_double_2_*)aabb._data[0].data(), *(_double_2_*)_data[1].data(), 2);
	f22[1] = _AVX_cmppd_(*(_double_2_*)(aabb._data[0].data() + 2), *(_double_2_*)(_data[1].data() + 2), 2);
	_double_2_ f23[2];
	f23[0] = _SSE2_andpd_(f21[0], f22[0]);
	f23[1] = _SSE2_andpd_(f21[1], f22[1]);
	_double_2_ res[2];
	res[0] = _SSE2_orpd_(f13[0], f23[0]);
	res[1] = _SSE2_orpd_(f13[1], f23[1]);
	int m1 = _SSE2_movemaskpd_(res[0]);
	int m2 = _SSE2_movemaskpd_(res[1]);
	return (m1 == 3) && ((m2 & 1) == 1);
}

//==================================================================================

template<>
inline bool AABB<double>::intersectionAABB(const AABB<double>& aabb1, AABB<double>& aabb2) const noexcept
{
	if (this == &aabb1) {
		aabb2 = aabb1;
		return true;
	}
	if (!intersection(aabb1)) {
		return false;
	}
	Vector3<double>::max(_data[0], aabb1._data[0], aabb2._data[0]);
	Vector3<double>::min(_data[1], aabb1._data[1], aabb2._data[1]);
	aabb2._measures = aabb2._data[1] - aabb2._data[0];
	aabb2._volume = aabb2._measures.x() * aabb2._measures.y() * aabb2._measures.z();
	return true;
}

//==================================================================================

template<>
inline bool AABB<double>::intersection(const AABB<double>& aabb1, const AABB<double>& aabb2) noexcept
{
	if (&aabb1 == &aabb2) {
		return true;
	}
	_double_2_ f11[2];
	f11[0] = _AVX_cmppd_(*(_double_2_*)aabb1._data[0].data(), *(_double_2_*)aabb2._data[0].data(), 13);
	f11[1] = _AVX_cmppd_(*(_double_2_*)(aabb1._data[0].data() + 2), *(_double_2_*)(aabb2._data[0].data() + 2), 13);
	_double_2_ f12[2];
	f12[0] = _AVX_cmppd_(*(_double_2_*)aabb1._data[0].data(), *(_double_2_*)aabb2._data[1].data(), 2);
	f12[1] = _AVX_cmppd_(*(_double_2_*)(aabb1._data[0].data() + 2), *(_double_2_*)(aabb2._data[1].data() + 2), 2);
	_double_2_ f13[2];
	f13[0] = _SSE2_andpd_(f11[0], f12[0]);
	f13[1] = _SSE2_andpd_(f11[1], f12[1]);
	_double_2_ f21[2];
	f21[0] = _AVX_cmppd_(*(_double_2_*)aabb2._data[0].data(), *(_double_2_*)aabb1._data[0].data(), 13);
	f21[1] = _AVX_cmppd_(*(_double_2_*)(aabb2._data[0].data() + 2), *(_double_2_*)(aabb1._data[0].data() + 2), 13);
	_double_2_ f22[2];
	f22[0] = _AVX_cmppd_(*(_double_2_*)aabb2._data[0].data(), *(_double_2_*)aabb1._data[1].data(), 2);
	f22[1] = _AVX_cmppd_(*(_double_2_*)(aabb2._data[0].data() + 2), *(_double_2_*)(aabb1._data[1].data() + 2), 2);
	_double_2_ f23[2];
	f23[0] = _SSE2_andpd_(f21[0], f22[0]);
	f23[1] = _SSE2_andpd_(f21[1], f22[1]);
	_double_2_ res[2];
	res[0] = _SSE2_orpd_(f13[0], f23[0]);
	res[1] = _SSE2_orpd_(f13[1], f23[1]);
	int m1 = _SSE2_movemaskpd_(res[0]);
	int m2 = _SSE2_movemaskpd_(res[1]);
	return (m1 == 3) && ((m2 & 1) == 1);
}

//==================================================================================

template<>
inline bool AABB<double>::intersectionAABB(const AABB<double>& aabb1, const AABB<double>& aabb2, AABB<double>& aabb3) noexcept
{
	if (&aabb1 == &aabb2) {
		aabb3 = aabb1;
		return true;
	}
	if (!intersection(aabb1, aabb2)) {
		return false;
	}
	Vector3<double>::max(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<double>::min(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
	return true;
}
#pragma endregion double_SSE

#endif // __AVX__
#elif defined(_M_ARM64) || defined(_M_ARM) || defined (__aarch64__)
#endif // defined (_M_IX86) || (_M_AMD64) || (__x86_64__) || defined(__i386__)

#endif // _USE_SIMD_EXTENTIONS_FOR_CALCULATING_

//==================================================================================

template<typename T>
inline bool AABB<T>::pointInside(const Vector3<T>& point) const noexcept
{
	return	(point.x() >= _data[0].x()) && (point.x() <= _data[1].x()) &&
		(point.y() >= _data[0].y()) && (point.y() <= _data[1].y()) &&
		(point.z() >= _data[0].z()) && (point.z() <= _data[1].z());
}

//==================================================================================

template<typename T>
inline bool AABB<T>::pointInside(const T* p) const noexcept
{
	return	(p[0] >= _data[0].x()) && (p[0] <= _data[1].x()) &&
		(p[1] >= _data[0].y()) && (p[1] <= _data[1].y()) &&
		(p[2] >= _data[0].z()) && (p[2] <= _data[1].z());
}

//==================================================================================

template<typename T>
inline bool AABB<T>::pointInside(T x, T y, T z) const noexcept
{
	return	(x >= _data[0].x()) && (x <= _data[1].x()) &&
		(y >= _data[0].y()) && (y <= _data[1].y()) &&
		(z >= _data[0].z()) && (z <= _data[1].z());
}

//==================================================================================

template<typename T>
inline bool AABB<T>::intersection(const AABB<T>& aabb) const noexcept
{
	if (this == &aabb) {
		return true;
	}
	if ((_data[0].x() < aabb._data[0].x() || _data[0].x() > aabb._data[1].x()) &&
		(aabb._data[0].x() < _data[0].x() || aabb._data[0].x() > _data[1].x())) {
		return false;
	}
	if ((_data[0].y() < aabb._data[0].y() || _data[0].y() > aabb._data[1].y()) &&
		(aabb._data[0].y() < _data[0].y() || aabb._data[0].y() > _data[1].y())) {
		return false;
	}
	if ((_data[0].z() < aabb._data[0].z() || _data[0].z() > aabb._data[1].z()) &&
		(aabb._data[0].z() < _data[0].z() || aabb._data[0].z() > _data[1].z())) {
		return false;
	}
	return true;
}

//==================================================================================

template<typename T>
inline bool AABB<T>::intersectionAABB(const AABB<T>& aabb1, AABB<T>& aabb2) const noexcept
{
	if (this == &aabb1) {
		aabb2 = aabb1;
		return true;
	}
	if ((_data[0].x() < aabb1._data[0].x() || _data[0].x() > aabb1._data[1].x()) &&
		(aabb1._data[0].x() < _data[0].x() || aabb1._data[0].x() > _data[1].x())) {
		return false;
	}
	if ((_data[0].y() < aabb1._data[0].y() || _data[0].y() > aabb1._data[1].y()) &&
		(aabb1._data[0].y() < _data[0].y() || aabb1._data[0].y() > _data[1].y())) {
		return false;
	}
	if ((_data[0].z() < aabb1._data[0].z() || _data[0].z() > aabb1._data[1].z()) &&
		(aabb1._data[0].z() < _data[0].z() || aabb1._data[0].z() > _data[1].z())) {
		return false;
	}
	Vector3<T>::max(_data[0], aabb1._data[0], aabb2._data[0]);
	Vector3<T>::min(_data[1], aabb1._data[1], aabb2._data[1]);
	aabb2._measures = aabb2._data[1] - aabb2._data[0];
	aabb2._volume = aabb2._measures.x() * aabb2._measures.y() * aabb2._measures.z();
	return true;
}


//==================================================================================

template<typename T>
inline bool AABB<T>::intersection(const AABB<T>& aabb1, const AABB<T>& aabb2) noexcept
{
	if (&aabb1 == &aabb2) {
		return true;
	}
	if ((aabb1._data[0].x() < aabb2._data[0].x() || aabb1._data[0].x() > aabb2._data[1].x()) &&
		(aabb2._data[0].x() < aabb1._data[0].x() || aabb2._data[0].x() > aabb1._data[1].x())) {
		return false;
	}
	if ((aabb1._data[0].y() < aabb2._data[0].y() || aabb1._data[0].y() > aabb2._data[1].y()) &&
		(aabb2._data[0].y() < aabb1._data[0].y() || aabb2._data[0].y() > aabb1._data[1].y())) {
		return false;
	}
	if ((aabb1._data[0].z() < aabb2._data[0].z() || aabb1._data[0].z() > aabb2._data[1].z()) &&
		(aabb2._data[0].z() < aabb1._data[0].z() || aabb2._data[0].z() > aabb1._data[1].z())) {
		return false;
	}
	return true;
}

//==================================================================================

template<typename T>
inline bool AABB<T>::intersectionAABB(const AABB<T>& aabb1, const AABB<T>& aabb2, AABB<T>& aabb3) noexcept
{
	if (&aabb1 == &aabb2) {
		aabb3 = aabb1;
		return true;
	}
	if ((aabb1._data[0].x() < aabb2._data[0].x() || aabb1._data[0].x() > aabb2._data[1].x()) &&
		(aabb2._data[0].x() < aabb1._data[0].x() || aabb2._data[0].x() > aabb1._data[1].x())) {
		return false;
	}
	if ((aabb1._data[0].y() < aabb2._data[0].y() || aabb1._data[0].y() > aabb2._data[1].y()) &&
		(aabb2._data[0].y() < aabb1._data[0].y() || aabb2._data[0].y() > aabb1._data[1].y())) {
		return false;
	}
	if ((aabb1._data[0].z() < aabb2._data[0].z() || aabb1._data[0].z() > aabb2._data[1].z()) &&
		(aabb2._data[0].z() < aabb1._data[0].z() || aabb2._data[0].z() > aabb1._data[1].z())) {
		return false;
	}
	Vector3<T>::max(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<T>::min(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
	return true;
}

//==================================================================================

template<typename T>
AABB<T> AABB<T>::boundAABB(const AABB<T>& aabb) const noexcept
{
	AABB<T> temp;
	Vector3<T>::min(_data[0], aabb._data[0], temp._data[0]);
	Vector3<T>::max(_data[1], aabb._data[1], temp._data[1]);
	temp._measures = temp._data[1] - temp._data[0];
	temp._volume = temp._measures.x() * temp._measures.y() * temp._measures.z();
	return temp;
}

//==================================================================================

template<typename T>
void AABB<T>::boundAABB(const AABB<T>& aabb1, const AABB<T>& aabb2, AABB<T>& aabb3) noexcept
{
	Vector3<T>::min(aabb1._data[0], aabb2._data[0], aabb3._data[0]);
	Vector3<T>::max(aabb1._data[1], aabb2._data[1], aabb3._data[1]);
	aabb3._measures = aabb3._data[1] - aabb3._data[0];
	aabb3._volume = aabb3._measures.x() * aabb3._measures.y() * aabb3._measures.z();
}

//==================================================================================

template<typename T>
T AABB<T>::volume() const noexcept
{
	return _volume;
}

//==================================================================================

template<typename T>
inline T AABB<T>::width() const noexcept
{
	return _measures[0];
}

//==================================================================================

template<typename T>
inline T AABB<T>::height() const noexcept
{
	return _measures[1];
}

//==================================================================================

template<typename T>
inline T AABB<T>::depth() const noexcept
{
	return _measures[2];
}

//==================================================================================

template<typename T>
inline const T* AABB<T>::measures() const noexcept
{
	return _measures.data();
}

//==================================================================================
// left bottom front
template<typename T>
const Vector3<T>& AABB<T>::LBF() const noexcept
{
	return _data[0];
}

//==================================================================================
// right top back
template<typename T>
const Vector3<T>& AABB<T>::RTB() const noexcept
{
	return _data[1];
}

//==================================================================================

template<typename T>
AABB<T>::~AABB() noexcept
{
}

#if defined (_AABB_MACRO_MAX_HAS_PUSHED_)
#pragma pop_macro("max")
#undef _AABB_MACRO_MAX_HAS_PUSHED_
#endif // _AABB_MACRO_MAX_HAS_PUSHED_
#if defined (_AABB_MACRO_MIN_HAS_PUSHED_)
#pragma pop_macro("min")
#undef _AABB_MACRO_MIN_HAS_PUSHED_
#endif // _AABB_MACRO_MIN_HAS_PUSHED_

#endif // _AABB_H_
