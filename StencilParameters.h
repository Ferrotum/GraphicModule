#pragma once
#ifndef STENCILPARAMETERS_H_
#define STENCILPARAMETERS_H_

class StencilParameters
{
private:
	unsigned int _function;
	bool _masked;
	int _refValue;
	unsigned int _bitMask;

	void _moveProc(StencilParameters& a) noexcept;
public:
	struct Function
	{
		enum {	NEVER,
				LESS,
				LEQUAL,
				GREATER,
				GEQUAL,
				EQUAL,
				NOTEQUAL,
				ALWAYS };
	};
	StencilParameters() noexcept;
	StencilParameters(const StencilParameters&) = delete;
	StencilParameters(StencilParameters&& a) noexcept
	{
		_moveProc(a);
	}
	StencilParameters& operator=(const StencilParameters&) = delete;
	StencilParameters& operator=(StencilParameters&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void setFunction(unsigned int nFunc) noexcept;
	unsigned int function() const noexcept
	{
		return _function;
	}
	void masked(bool b) noexcept;
	bool isMasked() const noexcept
	{
		return _masked;
	}
	~StencilParameters() noexcept;
};
#endif // !STENCILPARAMETERS_H_
