#include "Texture2DMultisample.h"

void Texture2DMultisample::_moveProc(Texture2DMultisample& a) noexcept
{
}

//==================================================================================

Texture2DMultisample::Texture2DMultisample(const GraphicContext::Ptr context, int width, int height, int samples, int format) :
	Texture(context)
{
	if (!Texture::_setFormat(format)) {
		return;
	}
	_width = width;
	_height = height;
	_multisampled = true;
	_samples = samples;
	_type = Texture::Type::TEXTURE_MULTISAMPLE_2D;
	_typeGL = GL_TEXTURE_2D_MULTISAMPLE;
	_context->setCurrent();
#if defined (_USE_OPENGL_ES_)
	glGenTextures(1, &_handleGL);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _handleGL);
	glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _formatGL, _width, _height, GL_TRUE);
#else
	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &_handleGL);
	glTextureStorage2DMultisample(_handleGL, _samples, _formatGL, _width, _height, GL_TRUE);
#endif
	_isOk = true;
}

//==================================================================================

std::shared_ptr<Texture2DMultisample> Texture2DMultisample::sharedNew(int width, int height, int samples, int format)
{
	auto context = GraphicContext::getDefault();
	std::shared_ptr<Texture2DMultisample> temp;
	if (context != nullptr) {
		temp = std::shared_ptr<Texture2DMultisample>(new Texture2DMultisample(context.get(), width, height, samples, format));
	}
	return temp;
}

//==================================================================================

std::shared_ptr<Texture2DMultisample> Texture2DMultisample::sharedNew(const GraphicContext::Ptr context, int width, int height, int samples, int format)
{
	std::shared_ptr<Texture2DMultisample> temp(new Texture2DMultisample(context, width, height, samples, format));
	return temp;
}

//==================================================================================

Texture2DMultisample::~Texture2DMultisample()
{
}
