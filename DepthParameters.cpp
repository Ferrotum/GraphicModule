#include "DepthParameters.h"

void DepthParameters::_moveProc(DepthParameters& a) noexcept
{
	_masked = a._masked;
	_function = a._function;
}

//==================================================================================

void DepthParameters::_copyProc(const DepthParameters& a) noexcept
{
	_masked = a._masked;
	_function = a._function;
}

//==================================================================================

DepthParameters::DepthParameters() noexcept :
	_masked(false),
	_function(Function::LESS)
{
}

//==================================================================================

void DepthParameters::setFunction(unsigned int f) noexcept
{
	if (f >= Function::NEVER && f <= Function::ALWAYS) {
		_function = f;
	}
}

//==================================================================================

void DepthParameters::mask(bool b) noexcept
{
	if (b != _masked) {
		_masked = b;
	}
}

//==================================================================================

DepthParameters::~DepthParameters() noexcept
{
}
