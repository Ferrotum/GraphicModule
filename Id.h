#pragma once
#ifndef ID_H_
#define ID_H_

#include <atomic>
#include <type_traits>
#include <cstdint>

#include "AuxDefinitions.h"

template <typename T>
class Id
{
	static_assert(std::is_class<T>::value, "Id<T>: T must be a class.");
public:
	using ClassType = T;
#if defined (_M_AMD64) || defined (_M_ARM64) || defined (__LP64__)
	using idType = uint64_t;
	enum { ID_TYPE_MAX = UINT64_MAX };
#else
	using idType = unsigned int;
	enum { ID_TYPE_MAX = UINT_MAX };
#endif // _M_AMD64 || _M_ARM64 || __LP64__
protected:
	static std::atomic<Id<T>::idType> _freeId;
	static std::atomic<Id<T>::idType> _numInstances;
	Id<T>::idType _id;
	Id<T>::idType _getFreeId() noexcept;
	void _releaseId(Id<T>::idType n) noexcept;
	void _moveProc(Id<T>& a) noexcept;
public:
	Id() noexcept;
	Id(const Id<T>&) = delete;
	Id(Id<T>&& a) noexcept;
	Id<T>& operator=(const Id<T>&) = delete;
	Id<T>& operator=(Id<T>&& a) noexcept;
	Id<T>::idType id() const noexcept;
	static Id<T>::idType numInstances() noexcept;
	operator idType() const noexcept;
	~Id() noexcept;
};

template <typename T> alignas(CASH_LINE_SIZE) std::atomic<typename Id<T>::idType> Id<T>::_freeId = 0;
template <typename T> alignas(CASH_LINE_SIZE) std::atomic<typename Id<T>::idType> Id<T>::_numInstances = 0;

template<typename T>
inline typename Id<T>::idType Id<T>::_getFreeId() noexcept
{
	return _freeId.fetch_add(1, std::memory_order_acq_rel);
}

//==================================================================================

template<typename T>
inline void Id<T>::_releaseId(Id<T>::idType n) noexcept
{
}

//==================================================================================

template<typename T>
inline void Id<T>::_moveProc(Id<T>& a) noexcept
{
	_id = a._id;
}

//==================================================================================

template<typename T>
inline Id<T>::Id() noexcept
{
	_id = _getFreeId();
	_numInstances.fetch_add(1, std::memory_order_acq_rel);
}

//==================================================================================

template<typename T>
inline Id<T>::Id(Id<T>&& a) noexcept
{
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline Id<T>& Id<T>::operator=(Id<T>&& a) noexcept
{
	if (this != &a) {
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline typename Id<T>::idType Id<T>::id() const noexcept
{
	return _id;
}

//==================================================================================

template<typename T>
inline typename Id<T>::idType Id<T>::numInstances() noexcept
{
	return _numInstances.load(std::memory_order_acquire);
}

//==================================================================================

template<typename T>
inline Id<T>::operator idType() const noexcept
{
	return _id;
}

//==================================================================================

template<typename T>
inline Id<T>::~Id() noexcept
{
	_releaseId(_id);
	_numInstances.fetch_sub(1, std::memory_order_acq_rel);
}

#endif // !ID_H_