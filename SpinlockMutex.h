#pragma once
#ifndef SPINLOCKMUTEX_H_
#define SPINLOCKMUTEX_H_

#include <atomic>

#include "AuxDefinitions.h"

class SpinlockMutex
{
private:
	alignas(CASH_LINE_SIZE) std::atomic_flag _flag;
	void _moveProc(SpinlockMutex& a) noexcept
	{
		if (a._flag.test_and_set(std::memory_order_acquire)) {
			_flag.test_and_set(std::memory_order_release);
		}
		a._flag.clear(std::memory_order_release);
	}
public:
	SpinlockMutex()
	{
		_flag.clear(std::memory_order_release);
	}
	SpinlockMutex(const SpinlockMutex&) = delete;
	SpinlockMutex(SpinlockMutex&& a) noexcept
	{
		_moveProc(a);
	}
	SpinlockMutex& operator=(const SpinlockMutex&) = delete;
	SpinlockMutex& operator=(SpinlockMutex&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void lock() noexcept
	{
		while (_flag.test_and_set(std::memory_order_acquire)) {

		}
	}
	bool try_lock() noexcept
	{
		return !_flag.test_and_set(std::memory_order_acq_rel);
	}
	void unlock() noexcept
	{
		_flag.clear(std::memory_order_release);
	}
	~SpinlockMutex() noexcept
	{
	}
};

#endif // SPINLOCKMUTEX_H_