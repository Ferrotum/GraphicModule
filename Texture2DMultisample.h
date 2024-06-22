#pragma once
#ifndef _TEXTURE2DMULTISAMPLE_H_
#define _TEXTURE2DMULTISAMPLE_H_

#include "Texture.h"

class Texture2DMultisample : public Texture
{
protected:
	void _moveProc(Texture2DMultisample& a) noexcept;

	Texture2DMultisample(const GraphicContext::Ptr context, int width, int height, int samples, int format);
	Texture2DMultisample(const Texture2DMultisample&) = delete;
	Texture2DMultisample(Texture2DMultisample&& a) noexcept :
		Texture(std::move(a))
	{
		_moveProc(a);
	}
public:
	Texture2DMultisample& operator=(const Texture2DMultisample&) = delete;
	Texture2DMultisample& operator=(Texture2DMultisample&& a) noexcept
	{
		if (this != &a) {
			Texture::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	static std::shared_ptr<Texture2DMultisample> sharedNew(int width, int height, int samples, int format);
	static std::shared_ptr<Texture2DMultisample> sharedNew(const GraphicContext::Ptr context, int width, int height, int samples, int format);
	~Texture2DMultisample();
};
#endif // !_TEXTURE2DMULTISAMPLE_H_
