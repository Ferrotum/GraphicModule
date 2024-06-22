#pragma once
#ifndef _COMPUTETASK_H_
#define _COMPUTETASK_H_

#include "OpenGLAux.h"
#include "ShaderProgram.h"
#include "UniformBlock.h"
#include "Id.h"
#include "Object.h"
#include "Texture.h"

#include <map>

class ComputeTask : public Object
{
public:
#if defined (_M_AMD64) || defined (_M_ARM64) || defined(__LP64__)
	typedef uint_fast64_t idType;
	enum { ID_TYPE_MAX = ULLONG_MAX };
#else
	typedef unsigned int idType;
	enum { ID_TYPE_MAX = UINT_MAX };
#endif // _M_AMD64 || _M_ARM64
protected:
	struct ShaderStorageBinder {
		std::shared_ptr<Buffer> buffer;
		unsigned int boundFormat;
		unsigned int boundPoint;
		enum { READ, WRITE, READ_WRITE };
	};
	struct ImageBinder {
		std::shared_ptr<Texture> texture;
		unsigned int boundFormat;
		unsigned int boundPoint;
		enum { READ, WRITE, READ_WRITE };
	};
	bool _writeToBuffers;
	bool _writeToTextures;
	bool _useAtomicCounters;
	mutable GLbitfield _barrierFlags;
	std::weak_ptr<ComputeTask> _thisInstance;
	bool _isOk;
	Id<ComputeTask> _id;
	GraphicContext::Ptr _context;
	std::shared_ptr<ShaderProgram> _pShaderProgram;
	unsigned int _dimentions[3];

	std::map<idType, std::shared_ptr<UniformBlock> > _uniformBlocks;
	std::map<idType, ShaderStorageBinder> _shaderStorages;
	std::map<idType, ImageBinder> _images;
	std::map<idType, std::shared_ptr<Texture> > _textures;

	void _syncDataSource() const noexcept;
	void _moveProc(ComputeTask& a) noexcept;
	void _pass() const noexcept;
	ComputeTask(const GraphicContext::Ptr context);
public:
	static std::shared_ptr<ComputeTask> sharedNew();
	static std::shared_ptr<ComputeTask> sharedNew(const GraphicContext::Ptr context);
	ComputeTask(const ComputeTask&) = delete;
	ComputeTask(ComputeTask&& a) noexcept :
		Object(std::move(a))
	{
		_moveProc(a);
	}
	ComputeTask& operator=(const ComputeTask&) = delete;
	ComputeTask& operator=(ComputeTask&& a) noexcept
	{
		if (this != &a) {
			Object::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	bool setShaderProgram(const std::shared_ptr<ShaderProgram>& sProgram);
	std::shared_ptr<ShaderProgram> shaderProgram() const noexcept
	{
		return _pShaderProgram;
	}
	GraphicContext::Ptr context() const noexcept
	{
		return _context;
	}
	void fulfil() const noexcept;
	void setDimentions(unsigned int x, unsigned int y, unsigned int z) noexcept;
	~ComputeTask() noexcept;
};
#endif // !_COMPUTETASK_H_
