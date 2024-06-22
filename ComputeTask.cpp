#include "ComputeTask.h"

inline void ComputeTask::_syncDataSource() const noexcept
{
	if (_writeToBuffers) {
		_barrierFlags |= GL_SHADER_STORAGE_BARRIER_BIT;
	}
	if (_writeToTextures) {
		_barrierFlags |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
	}
	if (_useAtomicCounters) {
		_barrierFlags |= GL_ATOMIC_COUNTER_BARRIER_BIT;
	}
	if (_barrierFlags) {
		glMemoryBarrier(_barrierFlags);
	}
}

//==================================================================================

void ComputeTask::_moveProc(ComputeTask& a) noexcept
{
	_context = a._context;
	a._context = nullptr;
	_writeToBuffers = a._writeToBuffers;
	_writeToTextures = a._writeToTextures;
	_useAtomicCounters = a._useAtomicCounters;
	_barrierFlags = a._barrierFlags;
	_id = std::move(a._id);
	_pShaderProgram = std::move(a._pShaderProgram);
	_uniformBlocks = std::move(a._uniformBlocks);
	_shaderStorages = std::move(a._shaderStorages);
	_images = std::move(a._images);
	_textures = std::move(a._textures);
	memcpy(_dimentions, a._dimentions, sizeof(_dimentions));
}

//==================================================================================

void ComputeTask::_pass() const noexcept
{
	if (!_isOk) {
		return;
	}
	//_syncDataSource();
	for (auto iter = _uniformBlocks.begin(); iter != _uniformBlocks.end(); ++iter) {
		iter->second->useBlock(123);
	}
	for (auto iter = _shaderStorages.begin(); iter != _shaderStorages.end(); ++iter) {
		iter->second.buffer->bindBuffer(iter->second.boundPoint);
	}
	_pShaderProgram->setActive();
	_context->setCurrent();
	glDispatchCompute(_dimentions[0], _dimentions[1], _dimentions[2]);
	_syncDataSource();
}

//==================================================================================

ComputeTask::ComputeTask(const GraphicContext::Ptr context) :
	_context(context),
	_isOk(false),
	_dimentions{ 1, 1, 1 },
	_writeToBuffers(false),
	_writeToTextures(false),
	_useAtomicCounters(false),
	_barrierFlags(0)
{
}

//==================================================================================

std::shared_ptr<ComputeTask> ComputeTask::sharedNew()
{
	std::shared_ptr<ComputeTask> temp;
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		temp = std::shared_ptr<ComputeTask>(new ComputeTask(context.get()));
		temp->_thisInstance = temp;
	}
	return temp;
}

//==================================================================================

std::shared_ptr<ComputeTask> ComputeTask::sharedNew(const GraphicContext::Ptr context)
{
	std::shared_ptr<ComputeTask> temp(new ComputeTask(context));
	temp->_thisInstance = temp;
	return temp;
}

//==================================================================================

bool ComputeTask::setShaderProgram(const std::shared_ptr<ShaderProgram>& sProgram)
{
	if (sProgram == std::shared_ptr<ShaderProgram>() || sProgram->context() != _context || !sProgram->computing()) {
		return false;
	}
	_pShaderProgram = sProgram;
	return true;
}

//==================================================================================

void ComputeTask::fulfil() const noexcept
{
	_pass();
}

//==================================================================================

void ComputeTask::setDimentions(unsigned int x, unsigned int y, unsigned int z) noexcept
{
	if (x >= 1 && x <= _context->parameters()->max_compute_work_group_count[0] &&
		y >= 1 && y <= _context->parameters()->max_compute_work_group_count[1] &&
		z >= 1 && z <= _context->parameters()->max_compute_work_group_count[2])
	{
		_dimentions[0] = x;
		_dimentions[1] = y;
		_dimentions[2] = z;
	}
}

//==================================================================================

ComputeTask::~ComputeTask() noexcept
{
}
