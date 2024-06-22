#pragma once
#ifndef _DEPTHPARAMETERS_H_
#define _DEPTHPARAMETERS_H_

class DepthParameters
{
private:
	bool _masked;
	unsigned int _function;

	void _moveProc(DepthParameters& a) noexcept;
	void _copyProc(const DepthParameters& a) noexcept;
public:
	struct Function
	{
		enum {	NEVER,
				LESS,
				EQUAL,
				LEQUAL,
				GREATER,
				NOTEQUAL,
				GEQUAL,
				ALWAYS };
	};
	DepthParameters() noexcept;
	DepthParameters(const DepthParameters& a) noexcept
	{
		_copyProc(a);
	}
	DepthParameters(DepthParameters&& a) noexcept
	{
		_moveProc(a);
	}
	DepthParameters& operator=(const DepthParameters& a) noexcept
	{
		if (this != &a) {
			_copyProc(a);
		}
		return *this;
	}
	DepthParameters& operator=(DepthParameters&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void setFunction(unsigned int f) noexcept;
	unsigned int function() const noexcept
	{
		return _function;
	}
	void mask(bool b) noexcept;
	bool masked() const noexcept
	{
		return _masked;
	}
	~DepthParameters() noexcept;
};
#endif // !_DEPTHPARAMETERS_H_
