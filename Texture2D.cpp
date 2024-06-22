#include "Texture2D.h"

thread_local Texture2D::MIPMapGenerateFunc Texture2D::_MIPGenerateFunc = nullptr;

void Texture2D::_generateMIPMap()
{
	if (_MIPGenerateFunc != nullptr) {
		_MIPGenerateFunc(*this);
	}
	else {
#if defined (_USE_OPENGL_ES_)
		glGenerateMipmap(GL_TEXTURE_2D);
#else
		glGenerateTextureMipmap(_handleGL);
#endif
	}
}

//==================================================================================

void Texture2D::_moveProc(Texture2D& a) noexcept
{
}

//==================================================================================

Texture2D::Texture2D(const GraphicContext::Ptr context, int width, int height, int levels, int format) :
	Texture(context)
{
	if (!Texture::_setFormat(format)) {
		return;
	}
	_width = width;
	_height = height;
	_mipLevels = levels;
	_type = Texture::Type::TEXTURE_2D;
	_typeGL = GL_TEXTURE_2D;
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glGenTextures(1, &_handleGL);
	glBindTexture(GL_TEXTURE_2D, _handleGL);
	glTexStorage2D(GL_TEXTURE_2D, _mipLevels, _formatGL, _width, _height);
#else
	glCreateTextures(GL_TEXTURE_2D, 1, &_handleGL);
	glTextureStorage2D(_handleGL, _mipLevels, _formatGL, _width, _height);
#endif
	_isOk = true;
}

//==================================================================================

std::shared_ptr<Texture2D> Texture2D::sharedNew(int width, int height, int levels, int format)
{
	auto context = GraphicContext::getDefault();
	std::shared_ptr<Texture2D> temp;
	if (context != nullptr) {
		temp = std::shared_ptr<Texture2D>(new Texture2D(context.get(), width, height, levels, format));
	}
	return temp;
}

//==================================================================================

std::shared_ptr<Texture2D> Texture2D::sharedNew(const GraphicContext::Ptr context, int width, int height, int levels, int format)
{
	std::shared_ptr<Texture2D> temp(new Texture2D(context, width, height, levels, format));
	return temp;
}

//==================================================================================

std::shared_ptr<Texture2D> Texture2D::sharedNew(int width, int height, int format)
{
	auto context = GraphicContext::getDefault();
	std::shared_ptr<Texture2D> temp;
	int maxWH = std::max<int>(width, height);
	int levels;
	frexp((float)maxWH, &levels);
	if (context != nullptr) {
		temp = std::shared_ptr<Texture2D>(new Texture2D(context.get(), width, height, levels, format));
	}
	return temp;
}

//==================================================================================

std::shared_ptr<Texture2D> Texture2D::sharedNew(const GraphicContext::Ptr context, int width, int height, int format)
{
	std::shared_ptr<Texture2D> temp;
	int maxWH = std::max<int>(width, height);
	int levels;
	frexp((float)maxWH, &levels);
	if (context != nullptr) {
		temp = std::shared_ptr<Texture2D>(new Texture2D(context, width, height, levels, format));
	}
	return temp;
}

//==================================================================================

void Texture2D::store(int x, int y, int width, int height, int sourceFormat, void* data)
{
	if (data == nullptr) {
		return;
	}
	GLenum format = Texture::_glSourceFormat(sourceFormat);
	if (format == 0) {
		return;
	}
	GLenum type = Texture::_glSourceType(sourceFormat);
	if (type == 0) {
		return;
	}
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, type, data);
	static_assert(false);
#else
	glTextureSubImage2D(_handleGL, 0, x, y, width, height, format, type, data);
#endif
	_generateMIPMap();
}

//==================================================================================

void Texture2D::store(int x, int y, int width, int height, int level, int sourceFormat, void* data)
{
	if (data == nullptr) {
		return;
	}
	GLenum format = Texture::_glSourceFormat(sourceFormat);
	if (format == 0) {
		return;
	}
	GLenum type = Texture::_glSourceType(sourceFormat);
	if (type == 0) {
		return;
	}
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, type, data);
	static_assert(false);
#else
	glTextureSubImage2D(_handleGL, level, x, y, width, height, format, type, data);
#endif
}

//==================================================================================

Texture2D::~Texture2D() noexcept
{
}
