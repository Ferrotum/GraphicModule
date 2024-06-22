#pragma once
#ifndef _DRAWINGQUEUE_H_
#define _DRAWINGQUEUE_H_

#include <map>
#include <mutex>
#include <atomic>

#include "DrawingQueueBase.h"

#ifndef DRAWING_QUEUE_IS_DECLARATED_
#define DRAWING_QUEUE_IS_DECLARATED_ 1
template <typename T, typename KeyCompareFunc = std::less<T> >
class DrawingQueue : public DrawingQueueBase
#else
template <typename T, typename KeyCompareFunc>
class DrawingQueue : public DrawingQueueBase
#endif
{
public:
	using KeyType = T;
	using KeyFunc = KeyCompareFunc;
	using KeyGetFuncArgType = const std::shared_ptr<Surface>&;
	using KeyGetFuncType = T (*)(KeyGetFuncArgType);
protected:
	std::atomic<bool> _abort;
	std::mutex _mt;
	std::multimap<T, std::shared_ptr<Surface>, KeyCompareFunc> _queue;
	static thread_local T counter;
	KeyGetFuncType _getKeyValue;
	void _moveProc(DrawingQueue<T, KeyCompareFunc>& a) noexcept;
	static T _defaultKeyValue(DrawingQueue<T, KeyCompareFunc>::KeyGetFuncArgType surface) noexcept;
public:
	DrawingQueue() noexcept;
	DrawingQueue(GraphicContext::Ptr context) noexcept;
	DrawingQueue(const DrawingQueue<T, KeyCompareFunc>&) = delete;
	DrawingQueue(DrawingQueue<T, KeyCompareFunc>&& a) noexcept;
	DrawingQueue<T, KeyCompareFunc>& operator=(const DrawingQueue<T, KeyCompareFunc>&) = delete;
	DrawingQueue<T, KeyCompareFunc>& operator=(DrawingQueue<T, KeyCompareFunc>&& a) noexcept;
	void push(const std::shared_ptr<Surface>& surface);
	void setKeyValueFunc(DrawingQueue<T, KeyCompareFunc>::KeyGetFuncType keyValueFunc) noexcept;
	DrawingQueue<T, KeyCompareFunc>::KeyGetFuncType keyValueFunc() const noexcept;
	void flush();
	void clear();
	void abort();
	~DrawingQueue() noexcept;
};

template<typename T, typename KeyCompareFunc> thread_local T DrawingQueue<T, KeyCompareFunc>::counter = (T)0;

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::_moveProc(DrawingQueue<T, KeyCompareFunc>& a) noexcept
{
	_getKeyValue = a._getKeyValue;
	_queue = std::move(a._queue);
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline T DrawingQueue<T, KeyCompareFunc>::_defaultKeyValue(typename DrawingQueue<T, KeyCompareFunc>::KeyGetFuncArgType surface) noexcept
{
	T temp = counter;
	counter += (T)1;
	return temp;
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline DrawingQueue<T, KeyCompareFunc>::DrawingQueue() noexcept :
	_getKeyValue(_defaultKeyValue),
	_abort(false)
{
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline DrawingQueue<T, KeyCompareFunc>::DrawingQueue(GraphicContext::Ptr context) noexcept :
	DrawingQueueBase(context),
	_getKeyValue(_defaultKeyValue),
	_abort(false)
{
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline DrawingQueue<T, KeyCompareFunc>::DrawingQueue(DrawingQueue<T, KeyCompareFunc>&& a) noexcept :
	DrawingQueueBase(std::move(a))
{
	_moveProc(a);
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline DrawingQueue<T, KeyCompareFunc>& DrawingQueue<T, KeyCompareFunc>::operator=(DrawingQueue<T, KeyCompareFunc>&& a) noexcept
{
	if (this != &a) {
		DrawingQueueBase::operator=(std::move(a));
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::push(const std::shared_ptr<Surface>& surface)
{
	if (_immediately) {
		surface->_pass();
		return;
	}
	std::lock_guard<std::mutex> lk(_mt);
	_queue.emplace(_getKeyValue(surface), surface);
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::setKeyValueFunc(typename DrawingQueue<T, KeyCompareFunc>::KeyGetFuncType keyValueFunc) noexcept
{
	if (keyValueFunc != nullptr) {
		_getKeyValue = keyValueFunc;
	}
	else {
		_getKeyValue = _defaultKeyValue;
	}
	if (!_queue.empty()) {
		decltype(_queue) temp;
		for (auto iter = _queue.begin(); iter != _queue.end(); ++iter) {
			temp.emplace(_getKeyValue(iter->second), iter->second);
		}
		_queue = std::move(temp);
	}
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline typename DrawingQueue<T, KeyCompareFunc>::KeyGetFuncType DrawingQueue<T, KeyCompareFunc>::keyValueFunc() const noexcept
{
	return _getKeyValue;
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::flush()
{
	if (_immediately) {
		return;
	}
	std::lock_guard<std::mutex> lk(_mt);
	auto iter = _queue.begin();
	if (_predrawFunc) {
		_predrawFunc(_predrawIn, _predrawOut);
	}
	for (; iter != _queue.end(); ++iter) {
		iter->second->_pass();
		bool expected = true;
		if (_abort.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {
			++iter;
			_queue.erase(_queue.begin(), iter);
			if (_postdrawFunc) {
				_postdrawFunc(_postdrawIn, _postdrawOut);
			}
			return;
		}
	}
	if (_postdrawFunc) {
		_postdrawFunc(_postdrawIn, _postdrawOut);
	}
	_queue.clear();
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::clear()
{
	std::lock_guard<std::mutex> lk(_mt);
	_queue.clear();
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline void DrawingQueue<T, KeyCompareFunc>::abort()
{
	_abort.store(true, std::memory_order_release);
}

//==================================================================================

template<typename T, typename KeyCompareFunc>
inline DrawingQueue<T, KeyCompareFunc>::~DrawingQueue() noexcept
{
	clear();
}
#endif // !_DRAWINGQUEUE_H_
