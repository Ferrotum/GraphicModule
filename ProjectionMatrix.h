#pragma once
#ifndef PROJECTIONMATRIX_H_
#define PROJECTIONMATRIX_H_

#include <atomic>
#include <cmath>
#include <cstdlib>
#include <mutex>

#include "math_definitions.h"
#include "Object.h"
#include "MatrixLibrary.h"

#define _PROJECTIONMATRIX_USE_SPINLOCK_
#ifdef _PROJECTIONMATRIX_USE_SPINLOCK_
#include "SpinlockMutex.h"
typedef SpinlockMutex ProjectionSyncObj;
#else
typedef std::mutex ProjectionSyncObj;
#endif // _PROJECTIONMATRIX_USE_SPINLOCK_


#if defined (near)
#pragma push_macro("near")
#undef near
#define _PROJECTIONMATRIX_NEAR_MACRO_UNDEFINED_ 1
#endif // near
#if defined (far)
#pragma push_macro("far")
#undef far
#define _PROJECTIONMATRIX_FAR_MACRO_UNDEFINED_ 1
#endif // far

template <typename T>
class ProjectionMatrix
{
	static_assert(std::is_floating_point<T>::value, "ProjectionMatrix<T>: T must be a floating type.");
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
	using mat4 = Eigen::Matrix4<T> __attribute__((aligned(ProjectionMatrix<T>::ALIGNMENT)));
#endif // _MSC_VER
#elif defined(_USE_QT_MATRIX_LIBRARY_)
#ifdef _MSC_VER
	using mat4 = __declspec(align(ALIGNMENT)) QGenericMatrix<4, 4, T>;
#elif defined (__GNUC__)
	using mat4 = QGenericMatrix<4, 4, T> __attribute__((aligned(ProjectionMatrix<T>::ALIGNMENT)));
#endif // _MSC_VER
#endif // _USE_EIGEN_MATRIX_LIBRARY_
	typedef T valueType;
protected:
	mat4* _projM;
	std::atomic<bool> _updated;

	void _moveProc(ProjectionMatrix<T>& a) noexcept;
	void _copyProc(const ProjectionMatrix<T>& a) noexcept;
	static void _firstInit();
	static std::atomic<bool> _hasBeenInited;
	static ProjectionSyncObj _mt;

	alignas(ALIGNMENT) static mat4 _identityMatrix;
public:
	ProjectionMatrix<T>();
	ProjectionMatrix<T>(const ProjectionMatrix<T>& a);
	ProjectionMatrix<T>(ProjectionMatrix<T>&& a) noexcept;
	ProjectionMatrix<T>& operator=(const ProjectionMatrix<T>& a) noexcept;
	ProjectionMatrix<T>& operator=(ProjectionMatrix<T>&& a) noexcept;
	void frustum(T left, T right, T bottom, T top, T near, T far);
	void perspective(T width, T height, T FOV, T zn, T zf, T azimuth = (T)0.0, T zenith = (T)0.0);
	void ortho(T xMin, T xMax, T yMin, T yMax, T zMin, T zMax);
	const mat4& matrix() const noexcept;
	const T* data() const noexcept;
	virtual ~ProjectionMatrix<T>();
};

//==================================================================================

template <typename T>
std::atomic<bool> ProjectionMatrix<T>::_hasBeenInited = false;

template <typename T> ProjectionSyncObj ProjectionMatrix<T>::_mt;

template <typename T>
alignas(ProjectionMatrix<T>::ALIGNMENT) typename ProjectionMatrix<T>::mat4 ProjectionMatrix<T>::_identityMatrix;


//==================================================================================

template <typename T>
ProjectionMatrix<T>::ProjectionMatrix() :
	_projM(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ProjectionSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
	}
}
#if defined (_MSC_VER)
	_projM = static_cast<mat4*>(_aligned_malloc(sizeof(mat4), ALIGNMENT));
#else
	_projM = static_cast<mat4*>(aligned_alloc(sizeof(mat4), ALIGNMENT));
#endif // _MSC_VER
	*_projM = _identityMatrix;
}

//==================================================================================

template<typename T>
inline ProjectionMatrix<T>::ProjectionMatrix(const ProjectionMatrix<T>& a) :
	_projM(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ProjectionSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
#if defined (_MSC_VER)
	_projM = static_cast<mat4*>(_aligned_malloc(sizeof(mat4), ALIGNMENT));
#else
	_projM = static_cast<mat4*>(aligned_alloc(sizeof(mat4), ALIGNMENT));
#endif // _MSC_VER
	_copyProc(a);
}

//==================================================================================

template<typename T>
inline ProjectionMatrix<T>::ProjectionMatrix(ProjectionMatrix<T>&& a) noexcept :
	_projM(nullptr)
{
	if (!_hasBeenInited.load(std::memory_order_acquire)) {
		std::lock_guard<ProjectionSyncObj> lk(_mt);
		if (!_hasBeenInited.load(std::memory_order_acquire)) {
			_firstInit();
		}
	}
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::_moveProc(ProjectionMatrix<T>& a) noexcept
{
	if (_projM != nullptr) {
#if defined (_MSC_VER)
		_aligned_free(_projM);
#else
		free(_projM);
#endif // _MSC_VER
	}
	_projM = a._projM;
	a._projM = nullptr;
	_updated.store(a._updated.load(std::memory_order_acquire), std::memory_order_release);
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::_copyProc(const ProjectionMatrix<T>& a) noexcept
{
	*_projM = *a._projM;
	_updated.store(true, std::memory_order_release);
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::_firstInit()
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
inline ProjectionMatrix<T>& ProjectionMatrix<T>::operator=(const ProjectionMatrix<T>& a) noexcept
{
	if (this != &a) {
		_copyProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline ProjectionMatrix<T>& ProjectionMatrix<T>::operator=(ProjectionMatrix<T>&& a) noexcept
{
	if (this != &a) {
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::frustum(T left, T right, T bottom, T top, T near, T far)
{
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::perspective(T width, T height, T FOV, T zn, T zf, T azimuth, T zenith)
{
	const T aspect = height > (T)0.0 ? width /height : (T)1.0;
	mat4 proj = _identityMatrix;
	mat4 scale = _identityMatrix;
	const T alpha = (zn + zf) / (zn - zf);
	const T beta = (T)(-2.0) * zn * zf / (zn - zf);
	const T Ymax = (T)tan(FOV * (T)PI_VALUE / (T)360.0);
	const T Ymin = -Ymax;
	scale(1, 1) = (T)1.0 / (T)tan(FOV * (T)PI_VALUE / (T)360.0);
	scale(0, 0) = scale(1, 1) / aspect;
	proj(2, 2) = alpha;
	proj(2, 3) = beta;
	proj(3, 2) = (T)(-1.0);
	*_projM = scale * proj;
	_updated.store(true, std::memory_order_release);
}

//==================================================================================

template<typename T>
inline void ProjectionMatrix<T>::ortho(T xMin, T xMax, T yMin, T yMax, T zMin, T zMax)
{
	mat4 M = _identityMatrix;
	T Xminus = xMax - xMin;
	T Xplus = xMax + xMin;
	T Yminus = yMax - yMin;
	T Yplus = yMax + yMin;
	T Zminus = zMax - zMin;
	T Zplus = zMax + zMin;
	
	M(0, 0) = (T)2.0 / Xminus;
	M(1, 1) = (T)2.0 / Yminus;
	M(2, 2) = (T)(-2.0) / Zminus;
	M(0, 3) = -Xplus / Xminus;
	M(1, 3) = -Yplus / Yminus;
	M(2, 3) = -Zplus / Zminus;
	*_projM = M;
	_updated.store(true, std::memory_order_release);
}

//==================================================================================

template<typename T>
inline const typename ProjectionMatrix<T>::mat4& ProjectionMatrix<T>::matrix() const noexcept
{
	return *_projM;
}

//==================================================================================

template<typename T>
inline const T* ProjectionMatrix<T>::data() const noexcept
{
	return _projM->data();
}

//==================================================================================

template<typename T>
inline ProjectionMatrix<T>::~ProjectionMatrix()
{
	if (_projM != nullptr) {
#if defined (_MSC_VER)
		_aligned_free(_projM);
#else
		free(_projM);
#endif // _MSC_VER
	}
}

#if defined (_PROJECTIONMATRIX_NEAR_MACRO_UNDEFINED_)
#pragma pop_macro("far")
#undef _PROJECTIONMATRIX_NEAR_MACRO_UNDEFINED_
#endif // _PROJECTIONMATRIX_NEAR_MACRO_UNDEFINED_
#if defined (_PROJECTIONMATRIX_FAR_MACRO_UNDEFINED_)
#pragma pop_macro("near")
#undef _PROJECTIONMATRIX_FAR_MACRO_UNDEFINED_
#endif // _PROJECTIONMATRIX_FAR_MACRO_UNDEFINED_

#endif // PROJECTIONMATRIX_H_