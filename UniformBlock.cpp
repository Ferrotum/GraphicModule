#include "UniformBlock.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif

std::atomic<bool> UniformBlock::_onceInit = true;
unsigned int UniformBlock::_counter;
std::mutex UniformBlock::_mt;
std::atomic<Id<Buffer>::idType> UniformBlock::_numUniformBlocks;

void UniformBlock::_init()
{
	// redo this block
	_counter = 0;
	_numUniformBlocks.store(0, std::memory_order_release);
	_onceInit.store(false, std::memory_order_release);
}

//==================================================================================

void UniformBlock::_moveProc(UniformBlock& a) noexcept
{
	_unbindUniformBlock();
	_blockFields = a._blockFields;
	_blockIndex = a._blockIndex;
	_bindingIndex = a._bindingIndex;
	_ptr = std::move(a._ptr);
	pShaderProgram = a.pShaderProgram;
	_name = std::move(a._name);
	_fieldsParam = std::move(a._fieldsParam);
	_currentSyncObjectValue = std::move(a._currentSyncObjectValue);
	_syncObjs = std::move(a._syncObjs);
	for (int i = 0; i < _syncObjs.size(); ++i) {
		if (_syncObjs[i] == &a._always) {
			_syncObjs[i] = &this->_always;
		}
	}
}

//==================================================================================

void UniformBlock::_unbindUniformBlock() noexcept
{
	_context->setCurrent();
	const size_t BOUND_POINT_SIZE = _context->_boundUniformBlocks.size();
	for (size_t i = 0; i < BOUND_POINT_SIZE; ++i) {
		if (_context->_boundUniformBlocks[i] == _id) {
			_context->_boundUniformBlocks[i] = Id<Buffer>::ID_TYPE_MAX;
			glBindBufferBase(GL_UNIFORM_BUFFER, i, 0);
		}
	}
}

//==================================================================================

void UniformBlock::_create()
{
}

//==================================================================================

UniformBlock::UniformBlock() noexcept :
	_bindingIndex(-1)
{
	std::lock_guard<std::mutex> lk(_mt);
	if (_onceInit.load(std::memory_order_acquire)) {
		_init();
	}
	++_counter;
	_numUniformBlocks.fetch_add(1, std::memory_order_acq_rel);
}

//==================================================================================

UniformBlock::UniformBlock(const std::shared_ptr<ShaderProgram>& pSP, std::string bName) :
	_bindingIndex(-1),
	_name(bName),
	pShaderProgram(pSP),
	_blockFields(0)
{
	{
		std::lock_guard<std::mutex> lk(_mt);
		if (_onceInit.load(std::memory_order_acquire)) {
			_init();
		}
		++_counter;
	}
	_numUniformBlocks.fetch_add(1, std::memory_order_acq_rel);
	if (pShaderProgram == nullptr) {
		std::string retMsg;
		retMsg = bName + ": " + "shader program wrong.";
		throw UniformBlockException(retMsg);
	}
	_bindingIndex = pShaderProgram->uniformBlockBinding(_name);
	if (_bindingIndex == -1) {
		std::string retMsg;
		retMsg = bName + ": " + "uniform block does't exist in shader program.";
		throw UniformBlockException(retMsg);
	}
	_blockIndex = pShaderProgram->uniformBlockIndex(_name);
	int bufferSize = pShaderProgram->_programUniformBlock[_blockIndex].properties.bufferDataSize;
	Buffer::operator=(Buffer(bufferSize, Buffer::Flags::MAP_WRITE));
	_blockFields = pShaderProgram->_programUniformBlock[_blockIndex].fields.size();
	_ptr.resize(_blockFields, nullptr);
	_fieldsParam.resize(_blockFields);
	for (int i = 0; i < _blockFields; ++i) {
		_fieldsParam[i].arraySize = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].arraySize;
		_fieldsParam[i].dataOffset = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].offset;
		_fieldsParam[i].arrayStride = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].arrayStride;
		_fieldsParam[i].componentNum = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].componentNum;
		_fieldsParam[i].componentSize = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].componentSize;
		_fieldsParam[i].matrixStride = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].matrixStride;
	}
	_syncObjs.resize(_blockFields, &this->_always);
	_currentSyncObjectValue.resize(_blockFields, 1);
}

//==================================================================================

UniformBlock::UniformBlock(GraphicContext::Ptr context, const std::shared_ptr<ShaderProgram>& pSP, std::string bName) :
	_bindingIndex(-1),
	_name(bName),
	pShaderProgram(pSP),
	_blockFields(0)
{
	{
		std::lock_guard<std::mutex> lk(_mt);
		if (_onceInit.load(std::memory_order_acquire)) {
			_init();
		}
		++_counter;
	}
	_numUniformBlocks.fetch_add(1, std::memory_order_acq_rel);
	if (pShaderProgram == nullptr) {
		std::string retMsg;
		retMsg = bName + ": " + "shader program wrong.";
		throw UniformBlockException(retMsg);
	}
	_bindingIndex = pShaderProgram->uniformBlockBinding(_name);
	if (_bindingIndex == -1) {
		std::string retMsg;
		retMsg = bName + ": " + "uniform block does't exist in shader program.";
		throw UniformBlockException(retMsg);
	}
	_blockIndex = pShaderProgram->uniformBlockIndex(_name);
	int bufferSize = pShaderProgram->_programUniformBlock[_blockIndex].properties.bufferDataSize;
	this;
	Buffer::operator=(Buffer(context, bufferSize, Buffer::Flags::MAP_WRITE));
	_blockFields = pShaderProgram->_programUniformBlock[_blockIndex].fields.size();
	_ptr.resize(_blockFields, nullptr);
	_fieldsParam.resize(_blockFields);
	for (int i = 0; i < _blockFields; ++i) {
		_fieldsParam[i].arraySize = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].arraySize;
		_fieldsParam[i].dataOffset = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].offset;
		_fieldsParam[i].arrayStride = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].arrayStride;
		_fieldsParam[i].componentNum = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].componentNum;
		_fieldsParam[i].componentSize = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].componentSize;
		_fieldsParam[i].matrixStride = pShaderProgram->_programUniformBlock[_blockIndex].fields[i].matrixStride;
	}
	_syncObjs.resize(_blockFields, &this->_always);
	_currentSyncObjectValue.resize(_blockFields, 1);
}

//==================================================================================

bool UniformBlock::bindData(std::string fName, const void* pData, SyncObject<SyncObjectMutex>* flag)
{
	if (!_hBuffer) {
		return false;
	}
	int fieldIndex = 0;
	for (; fieldIndex < _blockFields; ++fieldIndex) {
		if (fName == pShaderProgram->_programUniformBlock[_blockIndex].fields[fieldIndex].name) {
			break;
		}
	}
	if (fieldIndex == _blockFields) {
		return false;
	}
	_ptr[fieldIndex] = pData;
	if (flag != nullptr) {
		_syncObjs[fieldIndex] = flag;
	}
	return true;
}

//==================================================================================

bool UniformBlock::useBlock() const noexcept
{
	bool retValue = false;
	if (_hBuffer && _context->_boundUniformBlocks[_bindingIndex] != id()) {
		_context->setCurrent();
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindingIndex, _hBuffer);
		_context->_boundUniformBlocks[_bindingIndex] = id();
		retValue = true;
	}
	return retValue;
}

//==================================================================================

bool UniformBlock::useBlock(int BP) const noexcept
{
	bool retValue = false;
	if (_hBuffer && (BP >= 0) && _context->_boundUniformBlocks[BP] != id()) {
		_context->setCurrent();
		glBindBufferBase(GL_UNIFORM_BUFFER, BP, _hBuffer);
		_context->_boundUniformBlocks[BP] = id();
		retValue = true;
	}
	return retValue;
}

//==================================================================================

void UniformBlock::update() const noexcept
{
	if (!_hBuffer) {
		return;
	}
	bool modified = false;
	for (int i = 0; i < _blockFields; ++i) {
		if (_currentSyncObjectValue[i] != _syncObjs[i]->count()) {
			modified = true;
			break;
		}
	}
	if (!modified) {
		return;
	}
	_context->setCurrent();
#if defined(_USE_OPENGL_ES_)
	if (_context->_boundCopyWriteBuffer != _id) {
		_context->_boundCopyWriteBuffer = _id;
		glBindBuffer(GL_COPY_WRITE_BUFFER, _hBuffer);
	}
	char* pData = (char*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, _size, GL_MAP_WRITE_BIT);
#else
	char* pData = (char*)glMapNamedBufferRange(_hBuffer, 0, _size, GL_MAP_WRITE_BIT);
	if (pData == nullptr) {
		return;
	}
#endif
	for (int i = 0; i < _blockFields; ++i) {
		if (_ptr[i] == nullptr) {
			continue;
		}
		if (_currentSyncObjectValue[i] != _syncObjs[i]->count()) {
			std::shared_lock<SyncObject<SyncObjectMutex>> lk(*_syncObjs[i]);
			for (int j = 0; j < _fieldsParam[i].arraySize; ++j) {
				for (int k = 0; k < _fieldsParam[i].componentNum; ++k) {
					const ptrdiff_t resOff = _fieldsParam[i].dataOffset + j * _fieldsParam[i].arrayStride + k * _fieldsParam[i].matrixStride;
					memcpy(pData + resOff, (char*)_ptr[i] + (k + j * _fieldsParam[i].componentNum) * _fieldsParam[i].componentSize, _fieldsParam[i].componentSize);
				}
			}
			if (_syncObjs[i] != &this->_always) {
				_currentSyncObjectValue[i] = _syncObjs[i]->count();
			}
		}
	}
#if defined(_USE_OPENGL_ES_)
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);
#else
	glUnmapNamedBuffer(_hBuffer);
#endif
}

//==================================================================================

UniformBlock::~UniformBlock()
{
	const size_t idx = _context->_boundUniformBlocks.size();
	_unbindUniformBlock();
	_numUniformBlocks.fetch_sub(1, std::memory_order_acq_rel);
	std::lock_guard<std::mutex> lk(_mt);
	--_counter;
	if (_counter == 0) {
		_onceInit.store(true, std::memory_order_release);
	}
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
