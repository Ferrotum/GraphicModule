#pragma once
#ifndef SYNCOBJECT_H_
#define SYNCOBJECT_H_

#include <type_traits>
#include <atomic>
#include <new>

#include "AuxDefinitions.h"


template <typename T>
class SyncObject
{
	//static_assert(std::is_function<decltype(T::lock)>::value && std::is_function<decltype(T::unlock)>::value, "SyncObject<T>: T must has lock and unlock functions.");

public:
#if defined (_M_AMD64) || defined (_M_ARM64) || defined(__LP64__)
	using counterType = uint_fast64_t;
	enum { COUNTER_INIT = 0 };
#else
	using counterType = unsigned int;
	enum { COUNTER_INIT = 0 };
#endif // _M_AMD64 || _M_ARM64

	using MutexType = T;
protected:
	T _sync;
	alignas(CASH_LINE_SIZE) std::atomic<counterType> _counter;
private:
	void _moveProc(SyncObject<T>& a) noexcept;
public:
	SyncObject();
	SyncObject(const SyncObject<T>&) = delete;
	SyncObject(SyncObject<T>&& a) noexcept;
	SyncObject<T>& operator=(const SyncObject<T>&) = delete;
	SyncObject<T>& operator=(SyncObject<T>&& a) noexcept;
	SyncObject<T>::counterType count() const noexcept;
	void lock();
	bool try_lock();
	void unlock();
	void lock_shared();
	bool try_lock_shared();
	void unlock_shared();
	void update() noexcept;
	~SyncObject();
};

//==================================================================================

template<typename T>
inline void SyncObject<T>::_moveProc(SyncObject<T>& a) noexcept
{
	_sync = std::move(a._sync);
	_counter.store(a._counter.load(std::memory_order_acquire), std::memory_order_release);
}

//==================================================================================

template<typename T>
inline SyncObject<T>::SyncObject()
{
	_counter.store(COUNTER_INIT, std::memory_order_release);
}

//==================================================================================

template<typename T>
inline SyncObject<T>::SyncObject(SyncObject<T>&& a) noexcept
{
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline SyncObject<T>& SyncObject<T>::operator=(SyncObject<T>&& a) noexcept
{
	if (this != &a) {
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline typename SyncObject<T>::counterType SyncObject<T>::count() const noexcept
{
	return _counter.load(std::memory_order_acquire);
}

//==================================================================================

template<typename T>
inline void SyncObject<T>::lock()
{
	_sync.lock();
}

//==================================================================================

template<typename T>
inline bool SyncObject<T>::try_lock()
{
	return _sync.try_lock();
}

//==================================================================================

template<typename T>
inline void SyncObject<T>::unlock()
{
	_sync.unlock();
}

//==================================================================================

template<typename T>
inline void SyncObject<T>::lock_shared()
{
	_sync.lock_shared();
}

//==================================================================================

template<typename T>
inline bool SyncObject<T>::try_lock_shared()
{
	return _sync.try_lock_shared();
}

//==================================================================================

template<typename T>
inline void SyncObject<T>::unlock_shared()
{
	_sync.unlock_shared();
}

//==================================================================================

template<typename T>
inline void SyncObject<T>::update() noexcept
{
	_counter.fetch_add(1, std::memory_order_acq_rel);
}

//==================================================================================

template<typename T>
inline SyncObject<T>::~SyncObject()
{
}

#endif // SYNCOBJECT_H_