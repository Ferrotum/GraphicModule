#include "Surface.h"
#include "DrawingQueueBase.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4244 4267)
#endif

std::atomic<Surface::idType> Surface::_currentID = 0;

void Surface::_moveProc(Surface& a) noexcept
{
	_VAO = std::move(a._VAO);
	_bindedAttribBuffers = std::move(a._bindedAttribBuffers);
	_bindedUniformBlocks = std::move(a._bindedUniformBlocks);
	_vertexIndices = std::move(a._vertexIndices);
	_attachedData = std::move(a._attachedData);
	_vertexNum = a._vertexNum;
	_primitiveType = a._primitiveType;
	_made = a._made;
	a._made = false;
	_pShaderProgram = std::move(a._pShaderProgram);
	_surfaceID = a._surfaceID;
	a._surfaceID = _currentID.fetch_add(1, std::memory_order_acq_rel);
	_indexed = a._indexed;
	_numInstances = a._numInstances;
	_id = std::move(a._id);
	_queue = std::move(a._queue);
	_context = a._context;
}

//==================================================================================

void Surface::_pass() const
{
	if (_made) {
		_VAO.setActive();
		_pShaderProgram->setActive();
		for (auto iter = _bindedUniformBlocks.cbegin(); iter != _bindedUniformBlocks.cend(); ++iter) {
			iter->second._block->useBlock(iter->second._bindingPoint);
		}
		Interactor::_predrawFuncRun();
		if (_numInstances > 1) {
			_context->setCurrent();
			if (_indexed) {
				glDrawElementsInstanced(_primitiveType, _vertexNum, _vertexIndices->_GLelementType, 0, _numInstances);
			}
			else {
				glDrawArraysInstanced(_primitiveType, 0, _vertexNum, _numInstances);
			}
		}
		else {
			_context->setCurrent();
			if (_indexed) {
				glDrawElements(_primitiveType, _vertexNum, _vertexIndices->_GLelementType, 0);
			}
			else {
				glDrawArrays(_primitiveType, 0, _vertexNum);
			}
		}
		Interactor::_postdrawFuncRun();
	}
}

//==================================================================================

Surface::Surface() :
	_made(false),
	_vertexNum(0),
	_primitiveType(GL_POINTS),
	_indexed(false),
	_numInstances(1)
{
	_surfaceID = _currentID.fetch_add(1, std::memory_order_acq_rel);
	_currentFreeID.store(0, std::memory_order_release);
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		_context = context.get();
	}
	_queue = _context->_mainQueue;
}

//==================================================================================

Surface::Surface(GraphicContext::Ptr context) :
	_made(false),
	_vertexNum(0),
	_primitiveType(GL_POINTS),
	_indexed(false),
	_numInstances(1),
	_context(context)
{
	_surfaceID = _currentID.fetch_add(1, std::memory_order_acq_rel);
	_currentFreeID.store(0, std::memory_order_release);
	_queue = _context->_mainQueue;
}

//==================================================================================

bool Surface::setShaderProgram(const std::shared_ptr<ShaderProgram>& p)
{
	bool retValue = false;
	if (p != nullptr && static_cast<bool>(*p) && !p->computing() && _context == p->context()) {
		_pShaderProgram = p;
		retValue = true;
		_made = false;
	}
	return retValue;
}

//==================================================================================

Surface::idType Surface::bindUniformBlock(const std::shared_ptr<UniformBlock>& b)
{
	if (b == nullptr || _context != b->context()) {
		return ID_TYPE_MAX;
	}
	idType boundUBID = _currentFreeID.fetch_add(1, std::memory_order_acq_rel);
	_bindedUniformBlocks.emplace(boundUBID, UniformBlockStruct(b, b->bindingIndex()));
	return boundUBID;
}

//==================================================================================

bool Surface::bindIndices(const std::shared_ptr<IndexBuffer>& p, unsigned int num)
{

	if (p != nullptr && static_cast<bool>(*p) && _context == p->context()) {
		_vertexIndices = p;
		_vertexNum = (num == 0 || num > p->_numElements) ? p->_numElements : num;
		if (_indexed) {
			_made = false;
		}
		return true;
	}
	_indexed = false;
	return false;
}

//==================================================================================

Surface::idType Surface::bindVertexAttribBuffer(const std::shared_ptr<VertexAttributesBuffer>& p)
{
	Surface::idType index = ID_TYPE_MAX;
	if (p != nullptr && static_cast<bool>(*p) && p->getBuffer()->context() == _context) {
		index = _currentID.fetch_add(1, std::memory_order_acq_rel);
		int idx = _bindedAttribBuffers.size();
		_bindedAttribBuffers.emplace(index, VABuffersStruct(p, idx));
	}
	return index;
}

//==================================================================================

bool Surface::detach(Surface::idType n) noexcept
{
	auto pUB = _bindedUniformBlocks.find(n);
	if (pUB != _bindedUniformBlocks.end()) {
		_bindedUniformBlocks.erase(pUB);
		return true;
	}
	auto pData = _attachedData.find(n);
	if (pData != _attachedData.end()) {
		_attachedData.erase(pData);
		return true;
	}
	auto pVA = _bindedAttribBuffers.find(n);
	if (pVA != _bindedAttribBuffers.end()) {
		_bindedAttribBuffers.erase(pVA);
		return true;
	}
	return false;
}

//==================================================================================

std::shared_ptr<VertexAttributesBuffer> Surface::getVertexAttributesBuffer(Surface::idType n, unsigned int f) const noexcept
{
	if ((f == static_cast<int>(Surface::SearchBy::BY_POSITION)) && (n < _bindedAttribBuffers.size())) {
		auto iter = _bindedAttribBuffers.cbegin();
		for (int i = 0; i < n; ++i) {
			++iter;
		}
		return iter->second.buffer;
	}
	else if (f == static_cast<int>(Surface::SearchBy::BY_ID)) {
		auto iter = _bindedAttribBuffers.find(n);
		if (iter != _bindedAttribBuffers.cend()) {
			iter->second.buffer;
		}
	}
	return std::shared_ptr<VertexAttributesBuffer>();
}

//==================================================================================

std::shared_ptr<UniformBlock> Surface::getUniformBlock(Surface::idType n, unsigned int f) const noexcept
{
	if ((f == static_cast<int>(Surface::SearchBy::BY_POSITION)) && (n < _bindedUniformBlocks.size())) {
		auto iter = _bindedUniformBlocks.cbegin();
		for (int i = 0; i < n; ++i) {
			++iter;
		}
		return iter->second._block;
	}
	else if (f == static_cast<int>(Surface::SearchBy::BY_ID)) {
		auto iter = _bindedUniformBlocks.find(n);
		if (iter != _bindedUniformBlocks.cend()) {
			iter->second._block;
		}
	}
	return std::shared_ptr<UniformBlock>();
}

//==================================================================================

std::shared_ptr<void> Surface::getData(Surface::idType n, unsigned int f) const noexcept
{
	if ((f == static_cast<int>(Surface::SearchBy::BY_POSITION)) && (n < _attachedData.size())) {
		auto iter = _attachedData.cbegin();
		for (int i = 0; i < n; ++i) {
			++iter;
		}
		return iter->second;
	}
	else if (f == static_cast<int>(Surface::SearchBy::BY_ID)) {
		auto iter = _attachedData.find(n);
		if (iter != _attachedData.cend()) {
			iter->second;
		}
	}
	return std::shared_ptr<void>();
}

//==================================================================================

Surface::idType Surface::attach(const std::shared_ptr<void>& pData)
{
	if (pData == nullptr) {
		return ID_TYPE_MAX;
	}
	idType boundUBID = _currentFreeID.fetch_add(1, std::memory_order_acq_rel);
	_attachedData.emplace(boundUBID, pData);
	return boundUBID;
}

//==================================================================================

bool Surface::build()
{
	_made = false;
	for (auto iter = _bindedAttribBuffers.begin(); iter != _bindedAttribBuffers.end(); ++iter) {
		iter->second.hasBound = false;
	}
	_VAO = VertexArrayObject(_context);
	_VAO.setActive();
	if (!static_cast<bool>(_VAO) || _pShaderProgram == nullptr || !static_cast<bool>(*_pShaderProgram)) {
		return false;
	}
	_context->setCurrent();
	if (_vertexIndices != nullptr && static_cast<bool>(*_vertexIndices)) {
#if defined (_USE_OPENGL_ES_)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexIndices->handle());
#else
		glVertexArrayElementBuffer(_VAO.handle(), _vertexIndices->handle());
#endif // _USE_OPENGL_ES_
		_indexed = true;
	}
	int attribNum = _pShaderProgram->attribNum();
	for (int i = 0; i < attribNum; ++i) {
		for (auto iter = _bindedAttribBuffers.begin(); iter != _bindedAttribBuffers.end(); ++iter) {
			auto attrPtr = (iter->second.buffer)->_attributeProperties.find(_pShaderProgram->attibNameByIndex(i));
			if (attrPtr != (iter->second.buffer)->_attributeProperties.end()) {
				if (!(iter->second.hasBound)) {
#if defined (_USE_OPENGL_ES_)
					glBindVertexBuffer(iter->second.index, ((iter->second.buffer)->_buffer)->handle(), 0, iter->second.buffer->_stride);
#else
					glVertexArrayVertexBuffer(_VAO.handle(), iter->second.index, ((iter->second.buffer)->_buffer)->handle(), 0, iter->second.buffer->_stride);
#endif // _USE_OPENGL_ES_
					iter->second.hasBound = true;
				}
				if (!iter->second.hasDivided && (iter->second.buffer->_bufferDivisor != 0)) {
#if defined (_USE_OPENGL_ES_)
					glVertexBindingDivisor(iter->second.index, iter->second.buffer->_bufferDivisor);
#else
					glVertexArrayBindingDivisor(_VAO.handle(), iter->second.index, iter->second.buffer->_bufferDivisor);
#endif // _USE_OPENGL_ES_
					iter->second.hasDivided = true;
				}
				int attribLocation = _pShaderProgram->attribLocationByIndex(i);
				int attribSize = _pShaderProgram->attribAtomComponentNumByIndex(i);
				int attribType = _pShaderProgram->attribAtomTypeByIndex(i);
#if defined (_USE_OPENGL_ES_)
				glEnableVertexAttribArray(attribLocation);
				glVertexAttribBinding(attribLocation, iter->second.index);
				glVertexAttribFormat(attribLocation, attribSize, attribType, GL_FALSE, attrPtr->second.offset);
#else
				glEnableVertexArrayAttrib(_VAO.handle(), attribLocation);
				glVertexArrayAttribBinding(_VAO.handle(), attribLocation, iter->second.index);
				glVertexArrayAttribFormat(_VAO.handle(), attribLocation, attribSize, attribType, GL_FALSE, attrPtr->second.offset);
#endif // _USE_OPENGL_ES_
				if (attrPtr->second.divisor > 0) {
					glVertexAttribDivisor(attribLocation, attrPtr->second.divisor);
				}
				break;
			}
		}
	}
	for (auto iter = _bindedUniformBlocks.begin(); iter != _bindedUniformBlocks.end(); ++iter) {
		int BP = _pShaderProgram->uniformBlockBinding(iter->second._block->name());
		iter->second._bindingPoint = BP;
	}
	_made = true;
	return true;
}

//==================================================================================

void Surface::draw() const noexcept
{
	if (_hiden) {
		return;
	}
	//_pass();
	_queue->push(_thisInstance.lock());
}

//==================================================================================

void Surface::update() const noexcept
{
	if (!_made) {
		return;
	}
	for (auto iter = _bindedUniformBlocks.cbegin(); iter != _bindedUniformBlocks.cend(); ++iter) {
		if (iter->second._bindingPoint >= 0) {
			iter->second._block->update();
		}
	}
}

//==================================================================================

void Surface::clear() noexcept
{
	//Interactor::clear();
	//_bindedAttribBuffers.clear();
	//_bindedUniformBlocks.clear();
	//_vertexIndices = std::shared_ptr<IndexBuffer>();
	//_pShaderProgram = std::shared_ptr<ShaderProgram>();
	//_VAO = VertexArrayObject();
	//_made = false;
	*this = Surface();
}

//==================================================================================

Surface::~Surface() noexcept
{
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
