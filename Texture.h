#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Object.h"
#include "OpenGLAux.h"
#include "Id.h"
#include "GraphicContext.h"

class Texture : public Object
{
protected:
	bool _isOk;
	GraphicContext::Ptr _context;
	Id<Texture> _id;
	GLuint _handleGL;
	int _format;
	int _type;
	GLenum _typeGL;
	unsigned int _formatGL;
	int _mipLevels;
	int _width;
	int _height;
	int _depth;
	int _layers;
	int _samples;
	bool _layered;
	bool _multisampled;
private:
	void _deleteTexture() noexcept;
	void _moveProc(Texture& a) noexcept;
	bool _compatibleFormat(GLuint format) const noexcept
	{
		return format == GL_R8 ||
			format == GL_R8_SNORM ||
			format == GL_R16 ||
			format == GL_R16_SNORM ||
			format == GL_RG8 ||
			format == GL_RG8_SNORM ||
			format == GL_RG16 ||
			format == GL_RG16_SNORM ||
			format == GL_R3_G3_B2 ||
			format == GL_RGB4 ||
			format == GL_RGB5 ||
			format == GL_RGB8 ||
			format == GL_RGB8_SNORM ||
			format == GL_RGB10 ||
			format == GL_RGB12 ||
			format == GL_RGB16_SNORM ||
			format == GL_RGBA2 ||
			format == GL_RGBA4 ||
			format == GL_RGB5_A1 ||
			format == GL_RGBA8 ||
			format == GL_RGBA8_SNORM ||
			format == GL_RGB10_A2 ||
			format == GL_RGB10_A2UI ||
			format == GL_RGBA12 ||
			format == GL_RGBA16 ||
			format == GL_SRGB8 ||
			format == GL_SRGB8_ALPHA8 ||
			format == GL_R16F ||
			format == GL_RG16F ||
			format == GL_RGB16F ||
			format == GL_RGBA16F ||
			format == GL_R32F ||
			format == GL_RG32F ||
			format == GL_RGB32F ||
			format == GL_RGBA32F ||
			format == GL_R11F_G11F_B10F ||
			format == GL_RGB9_E5 ||
			format == GL_R8I ||
			format == GL_R8UI ||
			format == GL_R16I ||
			format == GL_R16UI ||
			format == GL_R32I ||
			format == GL_R32UI ||
			format == GL_RG8I ||
			format == GL_RG8UI ||
			format == GL_RG16I ||
			format == GL_RG16UI ||
			format == GL_RG32I ||
			format == GL_RG32UI ||
			format == GL_RGB8I ||
			format == GL_RGB8UI ||
			format == GL_RGB16I ||
			format == GL_RGB16UI ||
			format == GL_RGB32I ||
			format == GL_RGB32UI ||
			format == GL_RGBA8I ||
			format == GL_RGBA8UI ||
			format == GL_RGBA16I ||
			format == GL_RGBA16UI ||
			format == GL_RGBA32I ||
			format == GL_RGBA32UI;
	}
protected:
	bool _setFormat(int format) noexcept;
	static GLenum _glSourceFormat(int format) noexcept;
	static GLenum _glSourceType(int format) noexcept;
	Texture(const GraphicContext::Ptr context);
	Texture(const Texture&) = delete;
	Texture(Texture&& a) noexcept :
		Object(std::move(a))
	{
		_moveProc(a);
	}
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& a) noexcept
	{
		if (this != &a) {
			Object::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
public:
	struct Format {
		enum {	R8,
				RG8,
				RGB8,
				RGBA8,
				R32I,
				RG32I,
				RGB32I,
				RGBA32I,
				R32F,
				RG32F,
				RGB32F,
				RGBA32F,
				DEPTH32F};
	};
	struct Type {
		enum {	TEXTURE_1D,
				TEXTURE_2D,
				TEXTURE_3D,
				TEXTURE_ARRAY_1D,
				TEXTURE_ARRAY_2D,
				TEXTURE_MULTISAMPLE_2D,
				TEXTURE_NULTISAMPLE_2D_ARRAY,
				TEXTURE_CUBE_MAP,
				TEXTURE_CUBE_MAP_ARRAY };
	};
	struct ImageMode {
		enum { READ = 1, WRITE = 2, READ_WRITE = 3 };
	};
	struct SourceFormat
	{
		enum { R8,
			RG8,
			RGB8,
			BGR8,
			RGBA8,
			BGRA8,
			R32I,
			RG32I,
			RGB32I,
			BGR32I,
			RGBA32I,
			BGRA32I,
			R32F,
			RG32F,
			RGB32F,
			BGR32F,
			RGBA32F,
			BGRA32F,
			DEPTH32F,
			STENCIL };
	};
	int format() const noexcept
	{
		return _format;
	}
	int width() const noexcept
	{
		return _width;
	}
	int height() const noexcept
	{
		return _height;
	}
	int depth() const noexcept
	{
		return _depth;
	}
	int layers() const noexcept
	{
		return _layers;
	}
	int mipLevels() const noexcept
	{
		return _mipLevels;
	}
	void useAsTexture(unsigned int index) const noexcept;
	void useAsImage(unsigned int index, unsigned int mode, unsigned int level = 0, unsigned int layer = 0) const noexcept;
	virtual operator bool() const noexcept
	{
		return _handleGL != 0;
	}
	operator GLuint() const noexcept
	{
		return _handleGL;
	}
	virtual ~Texture() noexcept override;
};

#endif // _TEXTURE_H_