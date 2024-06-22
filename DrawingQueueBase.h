#pragma once
#ifndef _DRAWINGQUEUEBASE_H_
#define _DRAWINGQUEUEBASE_H_

#include <memory>

#include "Id.h"
#include "GraphicContext.h"
#include "Surface.h"

class DrawingQueueBase
{
public:
	typedef void (*PredrawFuncType)(const std::shared_ptr<void>&, const std::shared_ptr<void>&);
	typedef void (*PostrawFuncType)(const std::shared_ptr<void>&, const std::shared_ptr<void>&);
private:
	void _moveProc(DrawingQueueBase& a) noexcept
	{
		_immediately = a._immediately;
		_id = std::move(a._id);
		_context = a._context;
		_predrawIn = std::move(a._predrawIn);
		_predrawOut = std::move(a._predrawOut);
		_postdrawIn = std::move(a._postdrawIn);
		_postdrawOut = std::move(a._postdrawOut);
		_predrawFunc = a._predrawFunc;
		_postdrawFunc = a._postdrawFunc;
	}
protected:
	bool _immediately;
	Id<DrawingQueueBase> _id;
	GraphicContext::Ptr _context;
	std::shared_ptr<void> _predrawIn;
	std::shared_ptr<void> _predrawOut;
	std::shared_ptr<void> _postdrawIn;
	std::shared_ptr<void> _postdrawOut;
	PredrawFuncType _predrawFunc;
	PostrawFuncType _postdrawFunc;

public:
	DrawingQueueBase() noexcept :
		_immediately(false),
		_predrawFunc(nullptr),
		_postdrawFunc(nullptr),
		_context(nullptr)
	{
		auto context = GraphicContext::getDefault();
		if (context != nullptr) {
			_context = context.get();
		}
	}
	DrawingQueueBase(GraphicContext::Ptr context) noexcept :
		_immediately(false),
		_predrawFunc(nullptr),
		_postdrawFunc(nullptr),
		_context(context)
	{
	}
	DrawingQueueBase(const DrawingQueueBase&) = delete;
	DrawingQueueBase(DrawingQueueBase&& a) noexcept
	{
		_moveProc(a);
	}
	DrawingQueueBase& operator=(const DrawingQueueBase&) = delete;
	DrawingQueueBase& operator=(DrawingQueueBase&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	virtual void push(const std::shared_ptr<Surface>& surface) = 0;
	virtual void flush() = 0;
	virtual void clear() = 0;
	virtual void abort() = 0;
	void setImmediately(bool b) noexcept
	{
		_immediately = b;
	}
	bool immediately() const noexcept
	{
		return _immediately;
	}
	Id<DrawingQueueBase>::idType id() const noexcept
	{
		return _id;
	}
	void setPredrawFunc(PredrawFuncType func) noexcept
	{
		_predrawFunc = func;
	}
	void setPostdrawFunc(PostrawFuncType func) noexcept
	{
		_postdrawFunc = func;
	}
	PredrawFuncType predrawFunc() const noexcept
	{
		return _predrawFunc;
	}
	PostrawFuncType postdrawFunc() const noexcept
	{
		return _postdrawFunc;
	}
	virtual ~DrawingQueueBase() noexcept
	{

	}
};

#endif // !_DRAWINGQUEUEBASE_H_
