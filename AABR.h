#pragma once
#ifndef _AABR_H_
#define _AABR_H_

// Axis Aligned Bound Rectangle

#include "Vector2.h"
#include "x86_64_intrin.h"

#include <cstdint>
#include <type_traits>

#if defined (min)
#pragma push_macro("min")
#undef min
#define _AABR_MACRO_MIN_HAS_PUSHED_ 1
#endif // min
#if defined (max)
#pragma push_macro("max")
#undef max
#define _AABR_MACRO_MAX_HAS_PUSHED_ 1
#endif // max

template <typename T>
class AABR
{
	static_assert(std::is_arithmetic<T>::value, "AABR<T>: T must be an arithmetic type");
public:
	using valueType = T;
	using elementType = Vector2<T>;
protected:
	Vector2<T> _data[2];
	Vector2<T> _measures;
	T _square;

	void _copyProc(const AABR<T>& aabr) noexcept;
	void _moveProc(AABR<T>& aabr) noexcept;
public:
	AABR();
	AABR(const AABR<T>& aabr) noexcept;
	AABR(AABR<T>&& aabr) noexcept;
	AABR(const Vector2<T>& v1, const Vector2<T>& v2) noexcept;
	AABR(T left, T bottom, T right, T top) noexcept;
	AABR(const T* point1, const T* point2) noexcept;
	AABR<T>& operator=(const AABR<T>& aabr) noexcept;
	AABR<T>& operator=(AABR<T>&& aabr) noexcept;
	bool operator==(const AABR<T>& aabr) const noexcept;
	const Vector2<T>& operator[](size_t idx) const noexcept;
	bool pointInside(const Vector2<T>& point) const noexcept;
	bool pointInside(const T* p) const noexcept;
	bool pointInside(T x, T y) const noexcept;
	bool intersection(const AABR<T>& aabr) const noexcept;
	static bool intersection(const AABR<T>& aabr1, const AABR<T>& aabr2) noexcept;
	bool intersectionAABR(const AABR<T>& aabr1, AABR<T>& aabr2) const noexcept;
	static bool intersectionAABR(const AABR<T>& aabr1, const AABR<T>& aabr2, AABR<T>& aabr3) noexcept;
	AABR<T> boundAABR(const AABR<T>& aabr) const noexcept;
	static void boundAABR(const AABR<T>& aabr1, const AABR<T>& aabr2, AABR<T>& aabr3) noexcept;
	T square() const noexcept;
	T width() const noexcept;
	T height() const noexcept;
	const Vector2<T>& measures() const noexcept;
	void translate(const Vector2<T>& v) noexcept;
	void translate(T x, T y) noexcept;
	~AABR();
};

template<typename T>
void AABR<T>::_copyProc(const AABR<T>& aabr) noexcept
{
	_data[0] = aabr._data[0];
	_data[1] = aabr._data[1];
	_square = aabr._square;
	_measures = aabr._measures;
}

//==================================================================================

template<typename T>
void AABR<T>::_moveProc(AABR<T>& aabr) noexcept
{
	_data[0] = aabr._data[0];
	_data[1] = aabr._data[1];
	_square = aabr._square;
	_measures = aabr._measures;
}

//==================================================================================

template<typename T>
AABR<T>::AABR() :
	_square((T)0)
{
}

//==================================================================================

template<typename T>
AABR<T>::AABR(const AABR<T>& aabr) noexcept :
	_square((T)0)
{
	_copyProc(aabr);
}

//==================================================================================

template<typename T>
AABR<T>::AABR(AABR<T>&& aabr) noexcept :
	_square((T)0)
{
	_moveProc(aabr);
}

//==================================================================================

template<typename T>
AABR<T>::AABR(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	Vector2<T>::min(v1, v2, _data[0]);
	Vector2<T>::max(v1, v2, _data[1]);
	_measures = _data[1] - _data[0];
	_square = _measures.x() * _measures.y();
}

//==================================================================================

template<typename T>
inline AABR<T>::AABR(T left, T bottom, T right, T top) noexcept
{
	Vector2<T> v1(left, bottom);
	Vector2<T> v2(right, top);
	Vector2<T>::min(v1, v2, _data[0]);
	Vector2<T>::max(v1, v2, _data[1]);
	_measures = _data[1] - _data[0];
	_square = _measures.x() * _measures.y();
}

//==================================================================================

template<typename T>
inline AABR<T>::AABR(const T* point1, const T* point2) noexcept
{
	Vector2<T> v1(point1);
	Vector2<T> v2(point2);
	Vector2<T>::min(v1, v2, _data[0]);
	Vector2<T>::max(v1, v2, _data[1]);
	_measures = _data[1] - _data[0];
	_square = _measures.x() * _measures.y();
}

//==================================================================================

template<typename T>
inline AABR<T>& AABR<T>::operator=(const AABR<T>& aabr) noexcept
{
	if (this != &aabr) {
		_copyProc(aabr);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline AABR<T>& AABR<T>::operator=(AABR<T>&& aabr) noexcept
{
	if (this != &aabr) {
		_moveProc(aabr);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline bool AABR<T>::operator==(const AABR<T>& aabr) const noexcept
{
	return (this == &aabr) || (_data[0] == aabr._data[0]) && (_data[1] == aabr._data[1]);
}

//==================================================================================

template<typename T>
inline const Vector2<T>& AABR<T>::operator[](size_t idx) const noexcept
{
	return _data[idx];
}

#if defined (_USE_SIMD_EXTENTIONS_FOR_CALCULATING_)
#if defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64__) || defined(__i386__)
#if defined (__AVX__)

#else

#endif // AVX or SSE
#endif // x86 of x64
#endif // _USE_SIMD_EXTENTIONS_FOR_CALCULATING_


//==================================================================================

template<typename T>
inline bool AABR<T>::pointInside(const Vector2<T>& point) const noexcept
{
	return	(point.x() >= _data[0].x() && point.x() <= _data[1].x()) &&
			(point.y() >= _data[0].y() && point.y() <= _data[1].y());
}

//==================================================================================

template<typename T>
inline bool AABR<T>::pointInside(const T* p) const noexcept
{
	return	(p[0] >= _data[0].x() && p[0] <= _data[1].x()) &&
			(p[1] >= _data[0].y() && p[1] <= _data[1].y());
}

//==================================================================================

template<typename T>
inline bool AABR<T>::pointInside(T x, T y) const noexcept
{
	return	(x >= _data[0].x() && x <= _data[1].x()) &&
			(y >= _data[0].y() && y <= _data[1].y());
}

//==================================================================================

template<typename T>
inline bool AABR<T>::intersection(const AABR<T>& aabr) const noexcept
{
	if ((this == &aabr) ||
		((_data[0].x() >= aabr._data[0].x() && _data[0].x() <= aabr._data[1].x() ||
		aabr._data[0].x() >= _data[0].x() && aabr._data[0].x() <= _data[1].x()) &&
		(_data[0].y() >= aabr._data[0].y() && _data[0].y() <= aabr._data[1].y() ||
		aabr._data[0].y() >= _data[0].y() && aabr._data[0].y() <= _data[1].y()))) {
		return true;
	}
	return false;
}

//==================================================================================

template<typename T>
inline bool AABR<T>::intersection(const AABR<T>& aabr1, const AABR<T>& aabr2) noexcept
{
	if ((&aabr1 == &aabr2) ||
		((aabr1._data[0].x() >= aabr2._data[0].x() && aabr1._data[0].x() <= aabr2._data[1].x() ||
			aabr2._data[0].x() >= aabr1._data[0].x() && aabr2._data[0].x() <= aabr1._data[1].x()) &&
			(aabr1._data[0].y() >= aabr2._data[0].y() && aabr1._data[0].y() <= aabr2._data[1].y() ||
			aabr2._data[0].y() >= aabr1._data[0].y() && aabr2._data[0].y() <= aabr1._data[1].y()))) {
		return true;
	}
	return false;
}

//==================================================================================

template<typename T>
inline bool AABR<T>::intersectionAABR(const AABR<T>& aabr1, AABR<T>& aabr2) const noexcept
{
	if (!intersection(aabr1)) {
		return false;
	}
	Vector2<T>::max(_data[0], aabr1._data[0], aabr2._data[0]);
	Vector2<T>::min(_data[1], aabr1._data[1], aabr2._data[1]);
	aabr2._measures = aabr2._data[1] - aabr2._data[0];
	aabr2._square = aabr2._measures.x() * aabr2._measures.y();
	return true;
}

//==================================================================================

template<typename T>
inline bool AABR<T>::intersectionAABR(const AABR<T>& aabr1, const AABR<T>& aabr2, AABR<T>& aabr3) noexcept
{
	if (!intersection(aabr1, aabr2)) {
		return false;
	}
	Vector2<T>::max(aabr1._data[0], aabr2._data[0], aabr3._data[0]);
	Vector2<T>::min(aabr1._data[1], aabr2._data[1], aabr3._data[1]);
	aabr3._measures = aabr3._data[1] - aabr3._data[0];
	aabr3._square = aabr3._measures.x() * aabr3._measures.y();
	return true;
}

//==================================================================================

template<typename T>
inline AABR<T> AABR<T>::boundAABR(const AABR<T>& aabr) const noexcept
{
	AABR<T> temp;
	Vector2<T>::min(_data[0], aabr._data[0], temp._data[0]);
	Vector2<T>::max(_data[1], aabr._data[1], temp._data[1]);
	temp._measures = temp._data[1] - temp._data[0];
	temp._square = temp._measures.x() * temp._measures.y();
	return temp;
}

//==================================================================================

template<typename T>
inline void AABR<T>::boundAABR(const AABR<T>& aabr1, const AABR<T>& aabr2, AABR<T>& aabr3) noexcept
{
	Vector2<T>::min(aabr1._data[0], aabr2._data[0], aabr3._data[0]);
	Vector2<T>::max(aabr1._data[1], aabr2._data[1], aabr3._data[1]);
	aabr3._measures = aabr3._data[1] - aabr3._data[0];
	aabr3._square = aabr3._measures.x() * aabr3._measures.y();
}

//==================================================================================

template<typename T>
inline T AABR<T>::square() const noexcept
{
	return _square;
}

//==================================================================================

template<typename T>
inline T AABR<T>::width() const noexcept
{
	return _measures[0];
}

//==================================================================================

template<typename T>
inline T AABR<T>::height() const noexcept
{
	return _measures[1];
}

//==================================================================================

template<typename T>
inline const Vector2<T>& AABR<T>::measures() const noexcept
{
	return _measures;
}

//==================================================================================

template<typename T>
inline void AABR<T>::translate(const Vector2<T>& v) noexcept
{
	_data[0] += v;
	_data[1] += v;
}

//==================================================================================

template<typename T>
inline void AABR<T>::translate(T x, T y) noexcept
{
	Vector2<T> v(x, y);
	_data[0] += v;
	_data[1] += v;
}

//==================================================================================

template<typename T>
inline AABR<T>::~AABR()
{
}

#if defined (_AABR_MACRO_MAX_HAS_PUSHED_)
#pragma pop_macro("max")
#undef _AABR_MACRO_MAX_HAS_PUSHED_
#endif // _AABR_MACRO_MAX_HAS_PUSHED_
#if defined (_AABR_MACRO_MIN_HAS_PUSHED_)
#pragma pop_macro("min")
#undef _AABR_MACRO_MIN_HAS_PUSHED_
#endif // _AABR_MACRO_MIN_HAS_PUSHED_

#endif // _AABR_H_
