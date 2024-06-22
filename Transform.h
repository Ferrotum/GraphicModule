#pragma once
#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <atomic>
#include <mutex>
#include <type_traits>

#include "Object.h"
#include "math_definitions.h"
#include "SyncObject.h"
#include "MatrixLibrary.h"


#define _TRANSFORM_USE_SPINLOCK_
#ifdef _TRANSFORM_USE_SPINLOCK_
#include "SpinlockMutex.h"
typedef SpinlockMutex TransformSyncObj;
#else
typedef std::mutex TransformSyncObj;
#endif // _TRANSFORM_USE_SPINLOCK_


template <typename T>
class Transform : public Object
{
	static_assert(std::is_floating_point<T>::value, "Transform<T>: T must be a floating type.");
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
	using vec3 = __declspec(align(ALIGNMENT)) Eigen::Vector3<T>;
	using vec4 = __declspec(align(ALIGNMENT)) Eigen::Vector4<T>;
#elif defined (__GNUC__)
	using mat4 = Eigen::Matrix4<T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
	using vec3 = Eigen::Vector3<T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
	using vec4 = Eigen::Vector4<T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
#endif // _MSC_VER
#elif defined(_USE_QT_MATRIX_LIBRARY_)
#ifdef _MSC_VER
	using mat4 = __declspec(align(ALIGNMENT)) QGenericMatrix<4, 4, T>;
	using vec3 = __declspec(align(ALIGNMENT)) QGenericMatrix<1, 3, T>;
	using vec4 = __declspec(align(ALIGNMENT)) QGenericMatrix<1, 4, T>;
#elif defined (__GNUC__)
	using mat4 = QGenericMatrix<4, 4, T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
	using vec3 = QGenericMatrix<1, 3, T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
	using vec4 = QGenericMatrix<1, 4, T> __attribute__((aligned(Transform<T>::ALIGNMENT)));
#endif // _MSC_VER
#endif // _USE_EIGEN_MATRIX_LIBRARY_
	using valueType = T;
	enum { BLOCK_SIZE = ((sizeof(mat4) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT };
protected:
	mat4* _aMatrix;
	mat4* _bMatrix;
	mat4* _scaleMatrix;
	mat4* _modelMatrix;
	mat4* _normalMatrix;
	enum { MATRIX_NUM = 5 };
	mat4* _matrixBlock;

	void _moveProc(Transform<T>& a) noexcept;
	void _copyProc(const Transform<T>& a) noexcept;
	void _translate(const vec4& v) noexcept;
	void _hasChangedMatrices() noexcept;
	void _init();

	static void _firstInit();
	static std::atomic<bool> _hasBeenInited;
	static TransformSyncObj _mt;

	alignas(ALIGNMENT) static mat4 _identityMatrix;
public:
	Transform();
	Transform(const Transform<T>& a);
	Transform(Transform<T>&& a) noexcept;
	Transform<T>& operator=(const Transform<T>& a);
	Transform<T>& operator=(Transform<T>&& a) noexcept;
	const T* aMatrixV() const noexcept;
	const T* bMatrixV() const noexcept;
	const T* modelMatrixV() const noexcept;
	const T* normalMatrixV() const noexcept;
	const T* scaleMatrixV() const noexcept;

	const mat4& aMatrixM() const noexcept;
	const mat4& bMatrixM() const noexcept;
	const mat4& modelMatrixM() const noexcept;
	const mat4& normalMatrixM() const noexcept;
	const mat4& scaleMatrixM() const noexcept;

	virtual void setMoveMatrix(const mat4& M) noexcept;
	virtual void setBaseMatrix(const mat4& M) noexcept;
	virtual void setScaleMatrix(const mat4& M) noexcept;

	virtual void clearScale() noexcept;

	virtual void rotate(T angle, T x, T y, T z) noexcept;
	virtual void rotate(T angle, const vec3& v) noexcept;
	virtual void rotateAtOrigins(T angle, T x, T y, T z) noexcept;
	virtual void rotateAtOrigins(T angle, const vec3& v) noexcept;
	virtual void translate(T x, T y, T z) noexcept;
	virtual void translate(const vec3& v) noexcept;
	virtual void scale(T x, T y, T z) noexcept;
	virtual void scale(const vec3& v) noexcept;
	virtual void toOrigin() noexcept;
	virtual void clear() noexcept;
	static mat4 rotateM(T angle, T x, T y, T z) noexcept;
	static mat4 rotateM(T angle, const vec3& v) noexcept;
	static const mat4& mat4Identity() noexcept;
	virtual ~Transform() noexcept;
};

//==================================================================================

template <typename T> std::atomic<bool> Transform<T>::_hasBeenInited = false;

template <typename T> TransformSyncObj Transform<T>::_mt;

template <typename T> alignas(Transform<T>::ALIGNMENT) typename Transform<T>::mat4 Transform<T>::_identityMatrix;

//==================================================================================

template<typename T>
void Transform<T>::_moveProc(Transform<T>& a) noexcept
{
#if defined (_MSC_VER)
	if (_matrixBlock != nullptr) {
		_aligned_free(_matrixBlock);
		_matrixBlock = nullptr;
	}
#elif defined (__GNUC__)
	if (_matrixBlock != nullptr) {
		free(_matrixBlock);
		_matrixBlock = nullptr;
	}
#endif //
	_matrixBlock = a._matrixBlock;
	a._matrixBlock = nullptr;
}

//==================================================================================

template<typename T>
void Transform<T>::_copyProc(const Transform<T>& a) noexcept
{
	memcpy(_matrixBlock, a._matrixBlock, sizeof(mat4) * MATRIX_NUM);
}

//==================================================================================

template<typename T>
inline void Transform<T>::_translate(const vec4& v) noexcept
{
	_bMatrix->col(3) += v;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
Transform<T>::Transform()
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<TransformSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
	_init();
}

//==================================================================================

template<typename T>
inline Transform<T>::Transform(const Transform<T>& a)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<TransformSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
	_init();
	_copyProc(a);
}

//==================================================================================

template<typename T>
inline Transform<T>::Transform(Transform<T>&& a) noexcept :
	Object(std::move(a))
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<TransformSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline Transform<T>& Transform<T>::operator=(const Transform<T>& a)
{
	if (this != &a) {
		_copyProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
void Transform<T>::_hasChangedMatrices() noexcept
{
	*_modelMatrix = *_aMatrix * *_bMatrix * *_scaleMatrix;
	mat4 M = *_modelMatrix;
	*_normalMatrix = M.inverse().transpose();
}

//==================================================================================

template<typename T>
inline void Transform<T>::_init()
{
#if defined (_MSC_VER)
	_matrixBlock = static_cast<mat4*>(_aligned_malloc(BLOCK_SIZE * MATRIX_NUM, ALIGNMENT));
#elif defined (__GNUC__)
	_matrixBlock = static_cast<mat4*>(aligned_alloc(BLOCK_SIZE * MATRIX_NUM, ALIGNMENT));
#endif
	_aMatrix = _matrixBlock;
	_bMatrix = (mat4*)((char*)_matrixBlock + BLOCK_SIZE);
	_scaleMatrix = (mat4*)((char*)_matrixBlock + BLOCK_SIZE * 2);
	_modelMatrix = (mat4*)((char*)_matrixBlock + BLOCK_SIZE * 3);
	_normalMatrix = (mat4*)((char*)_matrixBlock + BLOCK_SIZE * 4);
	clear();
}

//==================================================================================

template<typename T>
inline void Transform<T>::_firstInit()
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
inline Transform<T>& Transform<T>::operator=(Transform<T>&& a) noexcept
{
	if (this != &a) {
		Object::operator=(std::move(a));
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline const T* Transform<T>::aMatrixV() const noexcept
{
	return _aMatrix->data();
}

//==================================================================================

template<typename T>
inline const T* Transform<T>::bMatrixV() const noexcept
{
	return _bMatrix->data();
}

//==================================================================================

template<typename T>
inline const T* Transform<T>::modelMatrixV() const noexcept
{
	return _modelMatrix->data();
}

//==================================================================================

template<typename T>
inline const T* Transform<T>::normalMatrixV() const noexcept
{
	return _normalMatrix->data();
}

//==================================================================================

template<typename T>
inline const T* Transform<T>::scaleMatrixV() const noexcept
{
	return _scaleMatrix->data();
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::aMatrixM() const noexcept
{
	return *_aMatrix;
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::modelMatrixM() const noexcept
{
	return *_modelMatrix;
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::bMatrixM() const noexcept
{
	return *_bMatrix;
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::normalMatrixM() const noexcept
{
	return *_normalMatrix;
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::scaleMatrixM() const noexcept
{
	return *_scaleMatrix;
}

//==================================================================================

template<typename T>
inline void Transform<T>::rotate(T angle, T x, T y, T z) noexcept
{
	vec3 v(x, y, z);
	rotate(angle, v);
}

//==================================================================================

template<typename T>
void Transform<T>::rotate(T angle, const vec3& v) noexcept
{
	T angleR = (T)fmod(angle, (T)360.0);
	if (angleR == (T)0.0) {
		return;
	}
	angleR = angleR / (T)180.0 * (T)PI_VALUE;
	mat4 rot = _identityMatrix;
	T k = (T)sqrt(v.dot(v));
	if (k == (T)0.0) {
		return;
	}
	k = (T)1.0 / k;
	T X = k * v(0);
	T Y = k * v(1);
	T Z = k * v(2);

	const T COS = (T)cos(angleR);
	const T SIN = (T)sin(angleR);
	const T ONE_MINUS_COS = (T)1.0 - COS;
	rot(0, 0) = COS + ONE_MINUS_COS * X * X;
	rot(0, 1) = ONE_MINUS_COS * X * Y - SIN * Z;
	rot(0, 2) = ONE_MINUS_COS * X * Z + SIN * Y;
	rot(1, 0) = ONE_MINUS_COS * X * Y + SIN * Z;
	rot(1, 1) = COS + ONE_MINUS_COS * Y * Y;
	rot(1, 2) = ONE_MINUS_COS * Y * Z - SIN * X;
	rot(2, 0) = ONE_MINUS_COS * X * Z - SIN * Y;
	rot(2, 1) = ONE_MINUS_COS * Y * Z + SIN * X;
	rot(2, 2) = COS + ONE_MINUS_COS * Z * Z;
	mat4 M = *_bMatrix;
	*_bMatrix = rot * M;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::rotateAtOrigins(T angle, T x, T y, T z) noexcept
{
	vec3 v(x, y, z);
	rotateAtOrigins(angle, v);
}

//==================================================================================

template<typename T>
void Transform<T>::rotateAtOrigins(T angle, const vec3& v) noexcept
{
	T angleR = (T)fmod(angle, (T)360.0);
	if (angleR == (T)0.0) {
		return;
	}
	angleR = angleR / (T)180.0 * (T)PI_VALUE;
	mat4 rot = _identityMatrix;
	T k = (T)sqrt(v.dot(v));
	if (k == (T)0.0) {
		return;
	}
	k = (T)1.0 / k;
	T X = k * v(0);
	T Y = k * v(1);
	T Z = k * v(2);

	const T COS = (T)cos(angleR);
	const T SIN = (T)sin(angleR);
	const T ONE_MINUS_COS = (T)1.0 - COS;
	rot(0, 0) = COS + ONE_MINUS_COS * X * X;
	rot(0, 1) = ONE_MINUS_COS * X * Y - SIN * Z;
	rot(0, 2) = ONE_MINUS_COS * X * Z + SIN * Y;
	rot(1, 0) = ONE_MINUS_COS * X * Y + SIN * Z;
	rot(1, 1) = COS + ONE_MINUS_COS * Y * Y;
	rot(1, 2) = ONE_MINUS_COS * Y * Z - SIN * X;
	rot(2, 0) = ONE_MINUS_COS * X * Z - SIN * Y;
	rot(2, 1) = ONE_MINUS_COS * Y * Z + SIN * X;
	rot(2, 2) = COS + ONE_MINUS_COS * Z * Z;
	mat4 M = *_bMatrix;
	*_bMatrix = M * rot;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::translate(T x, T y, T z) noexcept
{
	vec4 v(x, y, z, (T)0.0);
	_translate(v);
}

//==================================================================================

template<typename T>
inline void Transform<T>::translate(const vec3& v) noexcept
{
	vec4 _v(v(0), v(1), v(2), (T)0.0);
	_translate(_v);
}

//==================================================================================

template<typename T>
inline void Transform<T>::scale(const vec3& v) noexcept
{
	mat4 M = _identityMatrix;
	M(0, 0) = v(0);
	M(1, 1) = v(1);
	M(2, 2) = v(2);
	mat4 M2 = M * *_scaleMatrix;
	*_scaleMatrix = M2;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::toOrigin() noexcept
{
	*_bMatrix = _identityMatrix;
	*_scaleMatrix = _identityMatrix;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::clear() noexcept
{
	*_aMatrix = _identityMatrix;
	*_bMatrix = _identityMatrix;
	*_scaleMatrix = _identityMatrix;
	*_modelMatrix = _identityMatrix;
	*_normalMatrix = _identityMatrix;
}

//==================================================================================

template<typename T>
inline typename Transform<T>::mat4 Transform<T>::rotateM(T angle, T x, T y, T z) noexcept
{
	vec3 v(x, y, z);
	return rotateM(angle, v);
}

//==================================================================================

template<typename T>
typename Transform<T>::mat4 Transform<T>::rotateM(T angle, const vec3& v) noexcept
{
	mat4 rot = _identityMatrix;
	T angleR = (T)fmod(angle, (T)360.0);
	if (angleR == (T)0.0) {
		return rot;
	}
	angleR = angleR / (T)180.0 * (T)PI_VALUE;
	T k = (T)sqrt(v.dot(v));
	if (k == (T)0.0) {
		return rot;
	}
	k = (T)1.0 / k;
	T X = k * v(0);
	T Y = k * v(1);
	T Z = k * v(2);

	const T COS = (T)cos(angleR);
	const T SIN = (T)sin(angleR);
	const T ONE_MINUS_COS = (T)1.0 - COS;
	rot(0, 0) = COS + ONE_MINUS_COS * X * X;
	rot(0, 1) = ONE_MINUS_COS * X * Y - SIN * Z;
	rot(0, 2) = ONE_MINUS_COS * X * Z + SIN * Y;
	rot(1, 0) = ONE_MINUS_COS * X * Y + SIN * Z;
	rot(1, 1) = COS + ONE_MINUS_COS * Y * Y;
	rot(1, 2) = ONE_MINUS_COS * Y * Z - SIN * X;
	rot(2, 0) = ONE_MINUS_COS * X * Z - SIN * Y;
	rot(2, 1) = ONE_MINUS_COS * Y * Z + SIN * X;
	rot(2, 2) = COS + ONE_MINUS_COS * Z * Z;
	return rot;
}

//==================================================================================

template<typename T>
inline void Transform<T>::setMoveMatrix(const mat4& M) noexcept
{
	*_aMatrix = M;
	_hasChangedMatrices();
}
//==================================================================================

template<typename T>
inline void Transform<T>::setBaseMatrix(const mat4& M) noexcept
{
	*_bMatrix = M;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::setScaleMatrix(const mat4& M) noexcept
{
	*_scaleMatrix = M;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::clearScale() noexcept
{
	*_scaleMatrix = _identityMatrix;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline void Transform<T>::scale(T x, T y, T z) noexcept
{
	mat4 M = _identityMatrix;
	M(0, 0) = x;
	M(1, 1) = y;
	M(2, 2) = z;
	mat4 M2 = M * *_scaleMatrix;
	*_scaleMatrix = M2;
	_hasChangedMatrices();
}

//==================================================================================

template<typename T>
inline const typename Transform<T>::mat4& Transform<T>::mat4Identity() noexcept
{
	return _identityMatrix;
}

//==================================================================================

template<typename T>
inline Transform<T>::~Transform() noexcept
{
#if defined (_MSC_VER)
	if (_matrixBlock != nullptr) {
		_aligned_free(_matrixBlock);
		_matrixBlock = nullptr;
	}
#elif defined (__GNUC__)
	if (_matrixBlock != nullptr) {
		free(_matrixBlock);
		_matrixBlock = nullptr;
	}
#endif //
	_aMatrix = nullptr;
	_bMatrix = nullptr;
	_scaleMatrix = nullptr;
	_modelMatrix = nullptr;
	_normalMatrix = nullptr;
}

#endif // TRANSFORM_H_