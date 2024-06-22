#pragma once
#ifndef _COLORPARAMETERS_H_
#define _COLORPARAMETERS_H_

#include <cstdint>
#include <cstring>

class ColorParameters
{
protected:
	unsigned int _srcFactor;
	unsigned int _destFactor;
	unsigned int _equation;
	bool _useBlending;
	bool _masked[4];
	//float _constColor[4];

	void _moveProc(ColorParameters& a) noexcept;
	void _copyProc(const ColorParameters& a) noexcept;
public:
	struct Component
	{
		enum {	RED,
				GREEN,
				BLUE,
				ALPHA };
	};
	struct Equation
	{
		enum {	ADD,
				SUBTRACT,
				REVERSE_SUBTRACT,
				MIN,
				MAX };
	};
	struct Factor
	{
		enum {	ZERO,
				ONE,
				SRC,
				ONE_MINUS_SRC,
				DEST,
				ONE_MINUS_DEST,
				SRC_ALPHA,
				ONE_MINUS_SRC_ALPHA,
				DEST_ALPHA,
				ONE_MINUS_DEST_ALPHA,
				CONSTANT,
				ONE_MINUS_CONSTANT,
				CONSTANT_ALPHA,
				ONE_MINUS_CONSTANT_ALPHA };
	};
	ColorParameters() noexcept;
	ColorParameters(const ColorParameters& a) noexcept;
	ColorParameters(ColorParameters&& a) noexcept;
	ColorParameters& operator=(const ColorParameters& a) noexcept
	{
		if (this != &a) {
			_copyProc(a);
		}
		return *this;
	}
	ColorParameters& operator=(ColorParameters&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	void setSourceFactor(unsigned int newVal) noexcept;
	unsigned int sourceFactor() const noexcept
	{
		return _srcFactor;
	}
	void setDestinationFactor(unsigned int newVal) noexcept;
	unsigned int destinationFactor() const noexcept
	{
		return _destFactor;
	}
	void setEquation(unsigned int newVal) noexcept;
	unsigned int equation() const noexcept
	{
		return _equation;
	}
	void useBlending(bool b) noexcept;
	bool isBlended() const noexcept
	{
		return _useBlending;
	}
	void mask(unsigned int component, bool b) noexcept;
	bool masked(unsigned int component) noexcept;
	//void setConstantColor(float red, float green, float blue, float alpha) noexcept;
	//void setConstantColor(double red, double green, double blue, double alpha) noexcept;
	//void setConstantColor(char red, char green, char blue, char alpha) noexcept;
	//void setConstantColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) noexcept;
	//void setConstantColor(int red, int green, int blue, int alpha) noexcept;
	//void setConstantColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) noexcept;
	//void setConstantColor(int_fast64_t red, int_fast64_t green, int_fast64_t blue, int_fast64_t alpha) noexcept;
	//void setConstantColor(uint_fast64_t red, uint_fast64_t green, uint_fast64_t blue, uint_fast64_t alpha) noexcept;
	//void setConstantColor(const float* data) noexcept;
	//void setConstantColor(const double* data) noexcept;
	//void setConstantColor(const char* data) noexcept;
	//void setConstantColor(const unsigned char* data) noexcept;
	//void setConstantColor(const unsigned int* data) noexcept;
	//void setConstantColor(const int_fast64_t* data) noexcept;
	//void setConstantColor(const uint_fast64_t* data) noexcept;
	//void setActive() const noexcept;
	~ColorParameters() noexcept;
};

#endif // !_COLORPARAMETERS_H_