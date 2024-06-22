#include "ColorParameters.h"

void ColorParameters::_moveProc(ColorParameters& a) noexcept
{
	_srcFactor = a._srcFactor;
	_destFactor = a._destFactor;
	_equation = a._equation;
	_useBlending = a._useBlending;
	memcpy(_masked, a._masked, sizeof(_masked));
	//memcpy(_constColor, a._constColor, sizeof(_constColor));
}

//==================================================================================

void ColorParameters::_copyProc(const ColorParameters& a) noexcept
{
	_srcFactor = a._srcFactor;
	_destFactor = a._destFactor;
	_equation = a._equation;
	_useBlending = a._useBlending;
	memcpy(_masked, a._masked, sizeof(_masked));
	//memcpy(_constColor, a._constColor, sizeof(_constColor));
}

//==================================================================================

ColorParameters::ColorParameters() noexcept :
	_srcFactor(Factor::SRC_ALPHA),
	_destFactor(Factor::ONE_MINUS_SRC_ALPHA),
	_equation(Equation::ADD),
	_useBlending(false),
	_masked{ false, false, false, false }/*,
	_constColor{ 1.0f, 1.0f, 1.0f, 1.0f }*/
{
}

//==================================================================================

ColorParameters::ColorParameters(const ColorParameters& a) noexcept
{
	_copyProc(a);
}

//==================================================================================

ColorParameters::ColorParameters(ColorParameters&& a) noexcept
{
	_moveProc(a);
}

//==================================================================================

void ColorParameters::setSourceFactor(unsigned int newVal) noexcept
{
	if (newVal >= Factor::ZERO && newVal <= Factor::ONE_MINUS_CONSTANT_ALPHA) {
		_srcFactor = newVal;
	}
}

//==================================================================================

void ColorParameters::setDestinationFactor(unsigned int newVal) noexcept
{
	if (newVal >= Factor::ZERO && newVal <= Factor::ONE_MINUS_CONSTANT_ALPHA) {
		_destFactor = newVal;
	}
}

//==================================================================================

void ColorParameters::setEquation(unsigned int newVal) noexcept
{
	if (newVal >= Equation::ADD && newVal <= Equation::MAX) {
		_equation = newVal;
	}
}

//==================================================================================

void ColorParameters::useBlending(bool b) noexcept
{

}

//==================================================================================

void ColorParameters::mask(unsigned int component, bool b) noexcept
{
	if (component >= Component::RED && component <= Component::ALPHA) {
		if (_masked[component] != b) {
			_masked[component] = b;
		}
	}
}

//==================================================================================

bool ColorParameters::masked(unsigned int component) noexcept
{
	if (component >= Component::RED && component <= Component::ALPHA) {
		return _masked[component];
	}
	return false;
}

/*
//==================================================================================

void ColorParameters::setConstantColor(float red, float green, float blue, float alpha) noexcept
{
	_constColor[0] = red;
	_constColor[1] = green;
	_constColor[2] = blue;
	_constColor[3] = alpha;
}

//==================================================================================

void ColorParameters::setConstantColor(double red, double green, double blue, double alpha) noexcept
{
	_constColor[0] = (float)red;
	_constColor[1] = (float)green;
	_constColor[2] = (float)blue;
	_constColor[3] = (float)alpha;
}

//==================================================================================

void ColorParameters::setConstantColor(char red, char green, char blue, char alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(int red, int green, int blue, int alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(int_fast64_t red, int_fast64_t green, int_fast64_t blue, int_fast64_t alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(uint_fast64_t red, uint_fast64_t green, uint_fast64_t blue, uint_fast64_t alpha) noexcept
{
	_constColor[0] = (float)red / 255.0f;
	_constColor[1] = (float)green / 255.0f;
	_constColor[2] = (float)blue / 255.0f;
	_constColor[3] = (float)alpha / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(const float* data) noexcept
{
	_constColor[0] = data[0];
	_constColor[1] = data[1];
	_constColor[2] = data[2];
	_constColor[3] = data[3];
}

//==================================================================================

void ColorParameters::setConstantColor(const double* data) noexcept
{
	_constColor[0] = (float)data[0];
	_constColor[1] = (float)data[1];
	_constColor[2] = (float)data[2];
	_constColor[3] = (float)data[3];
}

//==================================================================================

void ColorParameters::setConstantColor(const char* data) noexcept
{
	_constColor[0] = (float)data[0] / 255.0f;
	_constColor[1] = (float)data[1] / 255.0f;
	_constColor[2] = (float)data[2] / 255.0f;
	_constColor[3] = (float)data[3] / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(const unsigned char* data) noexcept
{
	_constColor[0] = (float)data[0] / 255.0f;
	_constColor[1] = (float)data[1] / 255.0f;
	_constColor[2] = (float)data[2] / 255.0f;
	_constColor[3] = (float)data[3] / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(const unsigned int* data) noexcept
{
	_constColor[0] = (float)data[0] / 255.0f;
	_constColor[1] = (float)data[1] / 255.0f;
	_constColor[2] = (float)data[2] / 255.0f;
	_constColor[3] = (float)data[3] / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(const int_fast64_t* data) noexcept
{
	_constColor[0] = (float)data[0] / 255.0f;
	_constColor[1] = (float)data[1] / 255.0f;
	_constColor[2] = (float)data[2] / 255.0f;
	_constColor[3] = (float)data[3] / 255.0f;
}

//==================================================================================

void ColorParameters::setConstantColor(const uint_fast64_t* data) noexcept
{
	_constColor[0] = (float)data[0] / 255.0f;
	_constColor[1] = (float)data[1] / 255.0f;
	_constColor[2] = (float)data[2] / 255.0f;
	_constColor[3] = (float)data[3] / 255.0f;
}
*/
//==================================================================================

ColorParameters::~ColorParameters() noexcept
{
}
