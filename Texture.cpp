#include "Texture.h"

void Texture::_deleteTexture() noexcept
{
	_context->setCurrent();
	if (_handleGL) {
		glDeleteTextures(1, &_handleGL);
		_handleGL = 0;
	}
}

void Texture::_moveProc(Texture& a) noexcept
{
	_isOk = a._isOk;
	_context = a._context;
	a._context = nullptr;
	_id = std::move(a._id);
	_deleteTexture();
	_handleGL = a._handleGL;
	a._handleGL = 0;
	_format = a._format;
	_type = a._type;
	_typeGL = a._typeGL;
	_formatGL = a._formatGL;
	_mipLevels = a._mipLevels;
	_width = a._width;
	_height = a._height;
	_depth = a._depth;
	_layers = a._layers;
	_samples = a._samples;
	_layered = a._layered;
	_multisampled = a._multisampled;
}

//==================================================================================

bool Texture::_setFormat(int format) noexcept
{
	switch (format) {
	case Format::R8:
	{
		_formatGL = GL_R8;
		break;
	}
	case Format::RG8:
	{
		_formatGL = GL_RG8;
		break;
	}
	case Format::RGB8:
	{
		_formatGL = GL_RGB8;
		break;
	}
	case Format::RGBA8:
	{
		_formatGL = GL_RGBA8;
		break;
	}
	case Format::R32I:
	{
		_formatGL = GL_R32I;
		break;
	}
	case Format::RG32I:
	{
		_formatGL = GL_RG32I;
		break;
	}
	case Format::RGB32I:
	{
		_formatGL = GL_RGB32I;
		break;
	}
	case Format::RGBA32I:
	{
		_formatGL = GL_RGBA32I;
		break;
	}
	case Format::R32F:
	{
		_formatGL = GL_R32F;
		break;
	}
	case Format::RG32F:
	{
		_formatGL = GL_RG32F;
		break;
	}
	case Format::RGB32F:
	{
		_formatGL = GL_RGB32F;
		break;
	}
	case Format::RGBA32F:
	{
		_formatGL = GL_RGBA32F;
		break;
	}
	case Format::DEPTH32F:
	{
		_formatGL = GL_DEPTH_COMPONENT32F;
		break;
	}
	default:
	{
		return false;
	}
	}
	_format = format;
	return true;
}

//==================================================================================

GLenum Texture::_glSourceFormat(int format) noexcept
{
	GLenum retValue = 0;
	switch (format) {
	case SourceFormat::R8:
	case SourceFormat::R32I:
	case SourceFormat::R32F:
	{
		retValue = GL_RED;
		break;
	}
	case SourceFormat::RG8:
	case SourceFormat::RG32I:
	case SourceFormat::RG32F:
	{
		retValue = GL_RG;
		break;
	}
	case SourceFormat::RGB8:
	case SourceFormat::RGB32I:
	case SourceFormat::RGB32F:
	{
		retValue = GL_RGB;
		break;
	}
	case SourceFormat::RGBA8:
	case SourceFormat::RGBA32I:
	case SourceFormat::RGBA32F:
	{
		retValue = GL_RGBA;
		break;
	}
	case SourceFormat::DEPTH32F:
	{
		retValue = GL_DEPTH_COMPONENT;
		break;
	}
	case SourceFormat::STENCIL:
	{
		retValue = GL_STENCIL_INDEX;
		break;
	}
	}
	return retValue;
}

//==================================================================================

GLenum Texture::_glSourceType(int format) noexcept
{
	GLenum retValue = 0;
	switch (format) {
	case SourceFormat::R8:
	case SourceFormat::RG8:
	case SourceFormat::RGB8:
	case SourceFormat::RGBA8:
	{
		retValue = GL_UNSIGNED_BYTE;
		break;
	}
	case SourceFormat::R32I:
	case SourceFormat::RG32I:
	case SourceFormat::RGB32I:
	case SourceFormat::RGBA32I:
	{
		retValue = GL_INT;
		break;
	}
	case SourceFormat::R32F:
	case SourceFormat::RG32F:
	case SourceFormat::RGB32F:
	case SourceFormat::RGBA32F:
	{
		retValue = GL_FLOAT;
		break;
	}
	case SourceFormat::DEPTH32F:
	{
		retValue = GL_DEPTH_COMPONENT;
		break;
	}
	case SourceFormat::STENCIL:
	{
		retValue = GL_STENCIL_INDEX;
		break;
	}
	}
	return retValue;
}

//==================================================================================

Texture::Texture(const GraphicContext::Ptr context) :
	_isOk(false),
	_context(context),
	_handleGL(0),
	_mipLevels(0),
	_width(0),
	_height(0),
	_depth(0),
	_layers(0),
	_samples(0),
	_layered(false),
	_multisampled(false)
{

}

//==================================================================================

void Texture::useAsTexture(unsigned int index) const noexcept
{
	if (!_isOk) {
		return;
	}
#if defined (_USE_OPENGL_ES_)
	static_assert(false);
	if (_context->_activeTextures[index] == _id && ) {
		return;
	}
	_context->_activeTextures[index] = _id;
	_context->setCurrent();
	glBindTexture(_typeGL, _handleGL);
	glActiveTexture(GL_TEXTURE0 + index);
#else
	if (_context->_activeTextures[index] == _id) {
		return;
	}
	_context->_activeTextures[index] = _id;
	_context->setCurrent();
	glBindTextureUnit(index, _handleGL);
#endif
}

//==================================================================================

void Texture::useAsImage(unsigned int index, unsigned int mode, unsigned int level, unsigned int layer) const noexcept
{
	if (!_isOk || mode < 1 || mode > 3 || level >= _mipLevels || (layer >= _layers && _layered)) {
		return;
	}
	if (_context->_activeImages[index].id == _id && 
		_context->_activeImages[index].level == level &&
		_context->_activeImages[index].layer == layer &&
		_context->_activeImages[index].access == mode) {
		return;
	}
	_context->_activeImages[index] = GraphicContext::BoundImage(_id, level, layer, mode);
	GLuint access = 0;
	switch (mode) {
	case ImageMode::READ:
	{
		access = GL_READ_ONLY;
		break;
	}
	case ImageMode::WRITE:
	{
		access = GL_WRITE_ONLY;
		break;
	}
	case ImageMode::READ_WRITE:
	{
		access = GL_READ_WRITE;
		break;
	}
	}
	GLboolean layered = _layered == true ? GL_TRUE : GL_FALSE;
	_context->setCurrent();
	glBindImageTexture(index, _handleGL, level, layered, layer, access, _formatGL);
}

//==================================================================================

Texture::~Texture() noexcept
{
	_deleteTexture();
}
