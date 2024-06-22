#pragma once
#ifndef SHADERPROGRAMEXCEPTION_H_
#define SHADERPROGRAMEXCEPTION_H_

#include <exception>
#include <string>

#ifndef _EXCEPTION_SPEC_WHAT_a_
#if defined (__GNUC__)
#define _EXCEPTION_SPEC_WHAT_a_ const noexcept
#elif defined (_MSC_VER)
#define _EXCEPTION_SPEC_WHAT_a_ const
#endif
#endif // _EXCEPTION_SPEC_WHAT_a_

class ShaderProgramException : public std::exception
{
private:
	std::string errStr_;
public:
	ShaderProgramException(const char* str)
	{
		if (str != nullptr)
		{
			errStr_ = str;
		}
		else
		{
			errStr_ = "";
		}
	}
	ShaderProgramException(const std::string& str)
	{
		errStr_ = str;
	}
	const char* what() _EXCEPTION_SPEC_WHAT_a_ override
	{
		return errStr_.c_str();
	}
	~ShaderProgramException() noexcept {}
};

#endif // SHADERPROGRAMEXCEPTION_H_
