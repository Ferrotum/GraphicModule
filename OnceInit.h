#pragma once
#ifndef _ONCEINIT_H_
#define _ONCEINIT_H_

#include <atomic>
#include <mutex>
#include <type_traits>
#include <new>

#include "AuxDefinitions.h"

template <typename T, typename _MutexType = std::mutex>
class OnceInit
{
	static_assert(std::is_class<T>::value, "OnceInit<T>: T must be a class.");
public:
	using ClassType = T;
	using MutexType = _MutexType;
protected:
	alignas(CASH_LINE_SIZE) static std::atomic<bool> _initilized;
	static _MutexType _mt;
public:
	OnceInit();
	template <typename Func, typename... Args>
	bool initialize(Func func, Args... args);
	void reset() noexcept;
	template <typename Func, typename ...Args>
	bool reset(Func func, Args ...args);
	~OnceInit() noexcept;
};

template <typename T, typename _MutexType> alignas(CASH_LINE_SIZE) std::atomic<bool> OnceInit<T, _MutexType>::_initilized = false;
template <typename T, typename _MutexType> _MutexType OnceInit<T, _MutexType>::_mt;

template<typename T, typename _MutexType>
inline OnceInit<T, _MutexType>::OnceInit()
{
}

//==================================================================================

template<typename T, typename _MutexType>
inline void OnceInit<T, _MutexType>::reset() noexcept
{
	std::lock_guard<_MutexType> lk(_mt);
	_initilized.store(false, std::memory_order_release);
}

//==================================================================================

template<typename T, typename _MutexType>
template<typename Func, typename ...Args>
inline bool OnceInit<T, _MutexType>::initialize(Func func, Args ...args)
{
	std::lock_guard<_MutexType> lk(_mt);
	if (_initilized.load(std::memory_order_acquire)) {
		return false;
	}
	func(args...);
	_initilized.store(true, std::memory_order_release);
	return true;
}

//==================================================================================

template<typename T, typename _MutexType>
template<typename Func, typename ...Args>
inline bool OnceInit<T, _MutexType>::reset(Func func, Args ...args)
{
	std::lock_guard<_MutexType> lk(_mt);
	func(args...);
	_initilized.store(false, std::memory_order_release);
}

//==================================================================================

template<typename T, typename _MutexType>
inline OnceInit<T, _MutexType>::~OnceInit() noexcept
{
}

#endif // !_ONCEINIT_H_
