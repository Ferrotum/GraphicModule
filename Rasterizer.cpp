#include "Rasterizer.h"

bool Rasterizer::init() noexcept
{
	if (_isOK) {
		return false;
	}
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, _lineWidthRange);
	glGetFloatv(GL_POINT_SIZE_RANGE, _pointSizeRange);
	glGetFloatv(GL_LINE_WIDTH, &_lineWidth);
	glGetFloatv(GL_POINT_SIZE, &_pointSize);
	float value = 0.0f;
	glGetFloatv(GL_CULL_FACE, &value);
	_culling = value == 1.0f ? true : false;
	value = 0.0f;
	glGetFloatv(GL_CULL_FACE_MODE, &value);
	if (value == GL_FRONT) {
		_cullFaceMode = CullingFace::FRONT;
	}
	else if (value == GL_BACK) {
		_cullFaceMode = CullingFace::BACK;
	}
	else if (value == GL_FRONT_AND_BACK) {
		_cullFaceMode = CullingFace::FRONT_AND_BACK;
	}
	value = 0.0f;
	glGetFloatv(GL_FRONT_FACE, &value);
	if (value == GL_CCW) {
		_frontFace = Direction::COUNTERCLOCKWISE;
	}
	else if (value == GL_CW) {
		_frontFace = Direction::CLOCKWISE;
	}
	_isOK = true;
	return true;
}

//==================================================================================

void Rasterizer::_moveProc(Rasterizer& a) noexcept
{
	_isOK = a._isOK;
	memcpy(_lineWidthRange, a._lineWidthRange, sizeof(float) * 2);
	memcpy(_pointSizeRange, a._pointSizeRange, sizeof(float) * 2);
	_pointSize = a._pointSize;
	_lineWidth = a._lineWidth;
	_culling = a._culling;
	_frontFace = a._frontFace;
	_cullFaceMode = a._cullFaceMode;
}

//==================================================================================

Rasterizer::Rasterizer() :
	_isOK(false),
	_lineWidthRange{ 0.0f, 0.0f },
	_pointSizeRange{ 0.0f, 0.0f },
	_pointSize(1.0f),
	_lineWidth(1.0f),
	_culling(false),
	_frontFace(Direction::COUNTERCLOCKWISE),
	_cullFaceMode(CullingFace::BACK)
{
}

//==================================================================================

Rasterizer::Rasterizer(Rasterizer&& a) noexcept :
	_isOK(false),
	_lineWidthRange{ 0.0f, 0.0f },
	_pointSizeRange{ 0.0f, 0.0f },
	_pointSize(1.0f),
	_lineWidth(1.0f),
	_culling(false),
	_frontFace(Direction::COUNTERCLOCKWISE),
	_cullFaceMode(CullingFace::BACK)
{
	_moveProc(a);
}

//==================================================================================

void Rasterizer::setLineWidth(float w) noexcept
{
	if (w >= _lineWidthRange[0] && w <= _lineWidthRange[1]) {
		_lineWidth = w;
		glLineWidth(_lineWidth);
	}
}

//==================================================================================

void Rasterizer::setPointSize(float s) noexcept
{
	if (s >= _pointSizeRange[0] && s <= _pointSizeRange[1]) {
		_pointSize = s;
		glPointSize(s);
	}
}

//==================================================================================

void Rasterizer::setCulling(bool b) noexcept
{
	if (b != _culling) {
		_culling = b;
		if (_culling) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}
}

//==================================================================================

void Rasterizer::setFrontFace(unsigned int ff) noexcept
{
	if (ff > Direction::CLOCKWISE || _frontFace == ff) {
		return;
	}
	_frontFace = ff;
	switch (_frontFace) {
	case Direction::COUNTERCLOCKWISE:
	{
		glFrontFace(GL_CCW);
		break;
	}
	case Direction::CLOCKWISE:
	{
		glFrontFace(GL_CW);
		break;
	}
	}
}

//==================================================================================

void Rasterizer::setCullFace(unsigned int cf) noexcept
{
	if (cf > CullingFace::CullingFace::FRONT_AND_BACK || cf == _cullFaceMode) {
		return;
	}
	_cullFaceMode = cf;
	switch (_cullFaceMode) {
	case CullingFace::CullingFace::FRONT:
	{
		glCullFace(GL_FRONT);
		break;
	}
	case CullingFace::BACK:
	{
		glCullFace(GL_BACK);
		break;
	}
	case CullingFace::FRONT_AND_BACK:
	{
		glCullFace(GL_FRONT_AND_BACK);
		break;
	}
	}
}

//==================================================================================

Rasterizer::~Rasterizer() noexcept
{
}
