#pragma once
#ifndef COLOROUTPUT_H_
#define COLOROUTPUT_H_

#include "OpenGLAux.h"

class ColorOutput
{
private:
	int _index;
	void _moveProc(ColorOutput& a) noexcept;
protected:
	bool _isOK;

	bool _depthTest;
	bool _blend;
	bool _stencilTest;
	bool _scissorTest;
public:
	ColorOutput();
	ColorOutput(const ColorOutput&) = delete;
	ColorOutput(ColorOutput&& a) noexcept;
	ColorOutput& operator=(const ColorOutput&) = delete;
	ColorOutput& operator=(ColorOutput&& a) noexcept
	{
		if (this != &a) {
			_moveProc(a);
		}
		return *this;
	}
	bool init() noexcept;
	void setDepthTest(bool b) noexcept
	{
		if (b != _depthTest) {
			_depthTest = b;
			if (_depthTest) {
				glEnablei(GL_DEPTH_TEST, _index);
			}
			else {
				glDisablei(GL_DEPTH_TEST, _index);
			}
		}
	}
	bool depthTest() const noexcept
	{
		return _depthTest;
	}
	void setBlend(bool b) noexcept
	{
		if (b != _blend) {
			_blend = b;
			if (_blend) {
				glEnablei(GL_BLEND, _index);
			}
			else {
				glDisablei(GL_BLEND, _index);
			}
		}
	}
	bool blend() const noexcept
	{
		return _blend;
	}
	void setStencilTest(bool b) noexcept
	{
		if (b != _stencilTest) {
			_stencilTest = b;
			if (_stencilTest) {
				glEnablei(GL_STENCIL_TEST, _index);
			}
			else {
				glDisablei(GL_STENCIL_TEST, _index);
			}
		}
	}
	bool stencilTest() const noexcept
	{
		return _stencilTest;
	}
	void setScissorTest(bool b) noexcept
	{
		if (b != _scissorTest) {
			_scissorTest = b;
			if (_scissorTest) {
				glEnablei(GL_SCISSOR_TEST, _index);
			}
			else {
				glDisablei(GL_SCISSOR_TEST, _index);
			}
		}
	}
	bool scissorTest() const noexcept
	{
		return _scissorTest;
	}

	operator bool() const noexcept
	{
		return _isOK;
	}
	~ColorOutput();
};

#endif // COLOROUTPUT_H_