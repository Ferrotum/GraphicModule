#pragma once
#ifndef UNIFORMBLOCK_H_
#define UNIFORMBLOCK_H_

#include "Buffer.h"
#include "ShaderProgram.h"
#include "UniformBlockException.h"
#include "SyncObject.h"

#include "Types_using_in_tempates.h"

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

class UniformBlock : public Buffer
{
private:
	struct InnerParameters {
		ptrdiff_t dataOffset;
		int arraySize;
		int arrayStride;
		int matrixStride;
		int componentSize;
		int componentNum;
	};
	int _blockFields;
	int _blockIndex;
	int _bindingIndex;
	std::vector<const void*> _ptr;
	std::shared_ptr<ShaderProgram> pShaderProgram;
	std::string _name;
	std::vector<InnerParameters> _fieldsParam;

	mutable std::vector<SyncObject<SyncObjectMutex>::counterType> _currentSyncObjectValue;
	std::vector<SyncObject<SyncObjectMutex>*> _syncObjs;
	SyncObject<SyncObjectMutex> _always;

	static std::atomic<Id<Buffer>::idType> _numUniformBlocks;

	static std::atomic<bool> _onceInit;
	static unsigned int _counter;
	static std::mutex _mt;

	void _init();
	void _moveProc(UniformBlock& a) noexcept;
	void _unbindUniformBlock() noexcept;
	void _create();
public:
	UniformBlock() noexcept;
	UniformBlock(const std::shared_ptr<ShaderProgram>& pSP, std::string bName);
	UniformBlock(GraphicContext::Ptr context, const std::shared_ptr<ShaderProgram>& pSP, std::string bName);
	UniformBlock(const UniformBlock&) = delete;
	UniformBlock(UniformBlock&& a) noexcept :
		Buffer(std::move(a))
	{
		_moveProc(a);
		_numUniformBlocks.fetch_add(1, std::memory_order_acq_rel);
	}
	UniformBlock& operator=(const UniformBlock&) = delete;
	UniformBlock& operator=(UniformBlock&& a) noexcept
	{
		if (&a != this) {
			Buffer::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	const std::string& name() const noexcept
	{
		return _name;
	}
	bool bindData(std::string fName, const void* pData, SyncObject<SyncObjectMutex>* flag = nullptr);
	bool useBlock() const noexcept;
	bool useBlock(int BP) const noexcept;
	void update() const noexcept;
	int bindingIndex() const noexcept
	{
		return _bindingIndex;
	}
	operator bool() const noexcept
	{
		return _hBuffer != 0;
	}
	~UniformBlock();
};

#endif // UNIFORMBLOCK_H_
