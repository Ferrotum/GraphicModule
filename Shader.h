#pragma once
#ifndef SHADER_H_
#define SHADER_H_

//------------------------------------------------------
//included headers
//------------------------------------------------------
#include <iostream>
#include <string>
#include <list>
#include <exception>
#include <functional>
//--------------------------------------------------------
#include "Id.h"
#include "GraphicContext.h"

#include "ShaderProgramException.h"

// If need to generate exceptions
// uncomment code below
#define USE_SHADER_EXCEPTION 1

class Shader
{
private:
	size_t _hash;
	int _handle;
	int _type;
	int _openGLType;
	std::string _shaderName;
	std::string _errorMessage;
	Id<Shader> _id;
	GraphicContext::Ptr _context;

	void _createShader(const char* str, int len);

	void _moveProc(Shader& a) noexcept;
	void _deleteShader() noexcept;
public:
	struct ShaderType
	{
		enum { VERTEX, TESS_CONTROL, TESS_EVALUATION, GEOMETRIC, FRAGMENT, COMPUTING };
	};
public:
	Shader(GraphicContext::Ptr context, const char* s, int t, size_t h);
	Shader(GraphicContext::Ptr constext, const std::string& s, int t, size_t h);

	Shader(const Shader&) = delete;
	Shader(Shader&& a) noexcept
	{
		_moveProc(a);
	}

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&& a) noexcept
	{
		if (&a != this) {
			_moveProc(a);
		}
		return *this;
	}

	size_t hash() const noexcept
	{
		return _hash;
	}
	int handle() const noexcept
	{
		return _handle;
	}
	int type() const noexcept
	{
		return _type;
	}
	operator bool() const noexcept
	{
		return _handle != 0;
	}
	const std::string& error() const noexcept
	{
		return _errorMessage;
	}
	~Shader();
};

#endif // SHADER_H_