#pragma once
#ifndef TESSELATOR_H_
#define TESSELATOR_H_

#include "OpenGLAux.h"

class Tesselator
{
protected:
	bool _isOK;
	int _numVerticesPerPatch;
	int _maxVerticesPerPatch;
	int _patchInnerLevel[4];
	int _patchOuterLevel[4];

	void _moveProc(Tesselator& a) noexcept;
public:
	Tesselator() noexcept;
	Tesselator(const Tesselator&) = delete;
	Tesselator(Tesselator&& a) noexcept;
	Tesselator& operator=(const Tesselator&) = delete;
	Tesselator& operator=(Tesselator&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	bool init() noexcept;
	void setNumVerticesPerPatch(int n) noexcept;
	int numVerticesPerPatch() const noexcept
	{
		return _numVerticesPerPatch;
	}
	int maxVerticesPerPatch() const noexcept
	{
		return _maxVerticesPerPatch;
	}
	void setPatchInnerLevel(const int* v) noexcept;
	const int* patchInnerLevel() const noexcept
	{
		return _patchInnerLevel;
	}
	void setPatchOuterLevel(const int* v) noexcept;
	const int* patchOuterLevel() const noexcept
	{
		return _patchOuterLevel;
	}
	operator bool() const noexcept
	{
		return _isOK;
	}
	virtual ~Tesselator() noexcept
	{

	}
};
#endif // !TESSELATOR_H_
