#include "ColorOutput.h"

void ColorOutput::_moveProc(ColorOutput& a) noexcept
{
	_index = a._index;
	_isOK = a._isOK;
	_depthTest = a._depthTest;
	_blend = a._blend;
	_stencilTest = a._stencilTest;
	_scissorTest = a._scissorTest;
}

//==================================================================================

ColorOutput::ColorOutput() :
	_index(0),
	_isOK(false),
	_depthTest(false),
	_blend(false),
	_stencilTest(false),
	_scissorTest(false)
{
}

//==================================================================================

ColorOutput::ColorOutput(ColorOutput&& a) noexcept :
	_index(0),
	_isOK(false),
	_depthTest(false),
	_blend(false),
	_stencilTest(false),
	_scissorTest(false)
{
	_moveProc(a);
}

//==================================================================================

bool ColorOutput::init() noexcept
{
	if (_isOK) {
		return false;
	}
	float value = 0.0f;
	glGetFloatv(GL_DEPTH_TEST, &value);
	_depthTest = value == 1.0f ? true : false;
	value = 0.0f;
	glGetFloatv(GL_STENCIL_TEST, &value);
	_stencilTest = value == 1.0f ? true : false;
	value = 0.0f;
	glGetFloatv(GL_BLEND, &value);
	_blend = value == 1.0f ? true : false;
	value = 0.0f;
	glGetFloatv(GL_SCISSOR_TEST, &value);
	_scissorTest = value == 1.0f ? true : false;
	_isOK = true;
	return true;
}

//==================================================================================

ColorOutput::~ColorOutput()
{
}
