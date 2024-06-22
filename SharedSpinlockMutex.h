#pragma once
#ifndef SHAREDSPINLOCKMUTEX_H_
#define SHAREDSPINLOCKMUTEX_H_

#include <atomic>
#include <new>

#include "AuxDefinitions.h"

class SharedSpinlockMutex
{
private:
	using CounterType = size_t;
	enum {
		EXCEPTIONAL_FREE = 0,
		SET_EXCEPTIONAL = 1,
		RESET_EXCEPTIONAL = ~SET_EXCEPTIONAL,
		SET_SHARED = 2,
		RESET_SHARED = ~SET_SHARED,
		SHARED_INCREMENT = 0x100
	};
	// bit 0 - exceptional
	// bit 1 - shared
	alignas(CASH_LINE_SIZE) std::atomic<CounterType> _flagCounter;
public:
	SharedSpinlockMutex() noexcept
	{
		_flagCounter.store(0, std::memory_order_release);
	}
	SharedSpinlockMutex(const SharedSpinlockMutex&) = delete;
	SharedSpinlockMutex(SharedSpinlockMutex&&) = delete;
	SharedSpinlockMutex& operator=(const SharedSpinlockMutex&) = delete;
	SharedSpinlockMutex& operator=(SharedSpinlockMutex&&) = delete;
	void lock() noexcept
	{
		while (_flagCounter.fetch_or(SET_EXCEPTIONAL, std::memory_order_acq_rel)) {
			_flagCounter.fetch_and(RESET_EXCEPTIONAL, std::memory_order_acq_rel);
		}
	}
	bool try_lock() noexcept
	{
		auto cond = _flagCounter.fetch_or(SET_EXCEPTIONAL, std::memory_order_acq_rel);
		bool retVal = cond == EXCEPTIONAL_FREE;
		if (!retVal) {
			_flagCounter.fetch_and(RESET_EXCEPTIONAL, std::memory_order_acq_rel);
		}
		return retVal;
	}
	void unlock() noexcept
	{
		_flagCounter.fetch_and(RESET_EXCEPTIONAL, std::memory_order_acq_rel);
	}
	void lock_shared() noexcept
	{
		while (_flagCounter.fetch_or(SET_SHARED, std::memory_order_acq_rel)) {
			if (_flagCounter.load(std::memory_order_relaxed) & SET_EXCEPTIONAL) {
				_flagCounter.fetch_and(RESET_SHARED, std::memory_order_acq_rel);
			}
			else {
				break;
			}
		}
		_flagCounter.fetch_add(SHARED_INCREMENT, std::memory_order_relaxed);
		_flagCounter.fetch_and(RESET_SHARED, std::memory_order_acq_rel);
	}
	bool try_lock_shared() noexcept
	{
		auto cond = _flagCounter.fetch_or(SET_SHARED, std::memory_order_acq_rel);
		bool retVal = (cond & SET_EXCEPTIONAL) == EXCEPTIONAL_FREE;
		if (retVal) {
			_flagCounter.fetch_add(SHARED_INCREMENT, std::memory_order_relaxed);
		}
		_flagCounter.fetch_and(RESET_SHARED, std::memory_order_acq_rel);
		return retVal;
	}
	void unlock_shared() noexcept
	{
		_flagCounter.fetch_sub(SHARED_INCREMENT, std::memory_order_acq_rel);
	}
	~SharedSpinlockMutex() noexcept
	{

	}
};

#endif // !SHAREDSPINLOCKMUTEX_H_
