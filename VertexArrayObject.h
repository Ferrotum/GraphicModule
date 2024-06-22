#pragma once
#ifndef VERTEXARRAYOBJECT_H_
#define VERTEXARRAYOBJECT_H_

#include <atomic>

#include "Id.h"
#include "GraphicContext.h"

class VertexArrayObject
{
public:
private:
	GLuint _handle;
	Id<VertexArrayObject> _id;
	GraphicContext::Ptr _context;

	void _deleteVertexArray() noexcept;
	void _moveProc(VertexArrayObject& a) noexcept;
public:
	VertexArrayObject() noexcept;
	VertexArrayObject(GraphicContext::Ptr context) noexcept;
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject(VertexArrayObject&& a) noexcept :
		_handle(0)
	{
		_moveProc(a);
	}
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(VertexArrayObject&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	GLuint handle() const noexcept
	{
		return _handle;
	}
	operator GLuint() const noexcept
	{
		return _handle;
	}
	operator bool() const noexcept
	{
		return _handle != 0;
	}

	void setActive() const noexcept;
	~VertexArrayObject() noexcept
	{
		_deleteVertexArray();
	}
};

#endif // VERTEXARRAYOBJECT_H_