#pragma once
#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include "Texture.h"

class Texture2D : public Texture
{
	typedef void (*MIPMapGenerateFunc)(Texture2D&);
protected:
	static thread_local MIPMapGenerateFunc _MIPGenerateFunc;
	void _generateMIPMap();
	void _moveProc(Texture2D& a) noexcept;
	Texture2D(const GraphicContext::Ptr context, int width, int height, int levels, int format);
	Texture2D(const Texture2D&) = delete;
	Texture2D(Texture2D&& a) noexcept :
		Texture(std::move(a))
	{
	}
public:
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D& operator=(Texture2D&& a) noexcept
	{
		if (this != &a) {
			Texture::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	static std::shared_ptr<Texture2D> sharedNew(int width, int height, int levels, int format);
	static std::shared_ptr<Texture2D> sharedNew(const GraphicContext::Ptr context, int width, int height, int levels, int format);
	static std::shared_ptr<Texture2D> sharedNew(int width, int height, int format);
	static std::shared_ptr<Texture2D> sharedNew(const GraphicContext::Ptr context, int width, int height, int format);
	void store(int x, int y, int width, int height, int sourceFormat, void* data);
	void store(int x, int y, int width, int height, int level, int sourceFormat, void* data);

	~Texture2D() noexcept override;
};
#endif // !_TEXTURE2D_H_
