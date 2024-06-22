#include "StencilParameters.h"

void StencilParameters::_moveProc(StencilParameters& a) noexcept
{
	_function = a._function;
	_masked = a._masked;
}

//==================================================================================

StencilParameters::StencilParameters() noexcept :
	_function(Function::ALWAYS),
	_masked(false)
{
}

//==================================================================================

void StencilParameters::setFunction(unsigned int nFunc) noexcept
{
	if (nFunc >= Function::NEVER && nFunc <= Function::ALWAYS) {
		_function = nFunc;
	}
}

//==================================================================================

void StencilParameters::masked(bool b) noexcept
{
	if (_masked != b) {
		_masked = b;
	}
}

//==================================================================================

StencilParameters::~StencilParameters() noexcept
{
}
