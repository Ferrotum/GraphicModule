#pragma once
#ifndef UNIFORMBLOCKEXCEPTION_H_
#define UNIFORMBLOCKEXCEPTION_H_

#include <exception>
#include <string>

#ifndef _EXCEPTION_SPEC_WHAT_a_
#if defined (__GNUC__)
#define _EXCEPTION_SPEC_WHAT_a_ const noexcept
#elif defined (_MSC_VER)
#define _EXCEPTION_SPEC_WHAT_a_ const
#endif
#endif // _EXCEPTION_SPEC_WHAT_a_

class UniformBlockException final : public std::exception
{
private:
	std::string errStr_;
public:
	UniformBlockException(const char* str)
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
	UniformBlockException(const std::string& str)
	{
		errStr_ = str;
	}
	const char* what() _EXCEPTION_SPEC_WHAT_a_ override
	{
		return errStr_.c_str();
	}
	~UniformBlockException() noexcept {}
};

#endif // UNIFORMBLOCKEXCEPTION_H_
