#pragma once
#ifndef _BUFFEREXCEPTION_H_
#define _BUFFEREXCEPTION_H_

#include <exception>
#include <string>

#ifndef _EXCEPTION_SPEC_WHAT_a_
#if defined (__GNUC__)
#define _EXCEPTION_SPEC_WHAT_a_ const noexcept
#elif defined (_MSC_VER)
#define _EXCEPTION_SPEC_WHAT_a_ const
#endif
#endif // _EXCEPTION_SPEC_WHAT_a_

class BufferException : public std::exception
{
private:
	std::string errStr_;
public:
	BufferException(const char* str)
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
	BufferException(const std::string& str)
	{
		errStr_ = str;
	}
	const char* what() _EXCEPTION_SPEC_WHAT_a_ override
	{
		return errStr_.c_str();
	}
	~BufferException() noexcept {}
};

#endif // _BUFFEREXCEPTION_H_