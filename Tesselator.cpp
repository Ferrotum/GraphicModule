#include "Tesselator.h"

void Tesselator::_moveProc(Tesselator& a) noexcept
{
	_numVerticesPerPatch = a._numVerticesPerPatch;
	_maxVerticesPerPatch = a._maxVerticesPerPatch;
	memcpy(_patchInnerLevel, a._patchInnerLevel, sizeof(int) * 4);
	memcpy(_patchOuterLevel, a._patchOuterLevel, sizeof(int) * 4);
	_isOK = a._isOK;
}

//==================================================================================

Tesselator::Tesselator() noexcept :
	_numVerticesPerPatch(1),
	_maxVerticesPerPatch(1),
	_patchInnerLevel{ 1, 1, 0, 0 },
	_patchOuterLevel{ 1, 1, 1, 1 },
	_isOK(false)
{
}

//==================================================================================

Tesselator::Tesselator(Tesselator&& a) noexcept :
	_numVerticesPerPatch(1),
	_maxVerticesPerPatch(1),
	_patchInnerLevel{ 0, 0, 0, 0 },
	_patchOuterLevel{ 0, 0, 0, 0 },
	_isOK(false)
{
	_moveProc(a);
}

//==================================================================================

bool Tesselator::init() noexcept
{
	if (_isOK) {
		return false;
	}
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &_maxVerticesPerPatch);
	glGetIntegerv(GL_PATCH_VERTICES, &_numVerticesPerPatch);
	glGetIntegerv(GL_PATCH_DEFAULT_INNER_LEVEL, _patchInnerLevel);
	glGetIntegerv(GL_PATCH_DEFAULT_OUTER_LEVEL, _patchOuterLevel);
	_isOK = true;
	return true;
}

//==================================================================================

void Tesselator::setNumVerticesPerPatch(int n) noexcept
{
	if (n < 1 || n > _maxVerticesPerPatch) {
		return;
	}
	_numVerticesPerPatch = n;
}

//==================================================================================

void Tesselator::setPatchInnerLevel(const int* v) noexcept
{
	if (v == nullptr) {
		return;
	}
	for (size_t i = 0; i < 4; ++i) {
		if (v[i] < 1) {
			continue;
		}
		_patchInnerLevel[i] = v[i];
	}
}

//==================================================================================

void Tesselator::setPatchOuterLevel(const int* v) noexcept
{
	if (v == nullptr) {
		return;
	}
	for (size_t i = 0; i < 4; ++i) {
		if (v[i] < 1) {
			continue;
		}
		_patchOuterLevel[i] = v[i];
	}
}
