#include "Buffer.h"

std::mutex Buffer::_collectionMutex;
std::unordered_map<Id<Buffer>::idType, std::weak_ptr<Buffer> > Buffer::_collection;

std::atomic<bool> Buffer::_onceInit = true;
unsigned int Buffer::_counter;
std::mutex Buffer::_mt;

void Buffer::_deleteBuffer() noexcept
{
	unbindBuffer();
	if (_hBuffer) {
		//_context->setCurrent();
		glDeleteBuffers(1, &_hBuffer);
		_hBuffer = 0;
	}
}

//==================================================================================

void Buffer::_moveProc(Buffer& a) noexcept
{
	_deleteBuffer();
	_id = std::move(a._id);
	_hBuffer = a._hBuffer;
	a._hBuffer = 0;
	_size = a._size;
	a._size = 0;
	_flags = a._flags;
	_context = a._context;
	a._context = nullptr;
#if defined (_USE_OPENGL_ES_)
	_usingMode = a._usingMode;
#else
	_glFlags = a._glFlags;
#endif // _USE_OPENGL_ES_
}

//==================================================================================

void Buffer::_create(GraphicContext::Ptr context, unsigned int size, unsigned int flags, const void* data)
{
	_context = context;
	if (size <= 0) {
		throw BufferException("Buffer size is less then 0.");
	}
#if defined (_USE_OPENGL_ES_)
	if (flags & Buffer::Flags::MAP_WRITE) {
		_usingMode = GL_DYNAMIC_DRAW;
	}
	else if ((flags & Buffer::Flags::MAP_READ)) {
		_usingMode = GL_DYNAMIC_READ;
	}
	else if (flags & Buffer::Flags::DYNAMIC_STORAGE) {
		_usingMode = GL_DYNAMIC_COPY;
	}
#else
	if (flags & Buffer::Flags::DYNAMIC_STORAGE) {
		_glFlags |= GL_DYNAMIC_STORAGE_BIT;
	}
	if (flags & Buffer::Flags::MAP_READ) {
		_glFlags |= GL_MAP_READ_BIT;
	}
	if (flags & Buffer::Flags::MAP_WRITE) {
		_glFlags |= GL_MAP_WRITE_BIT;
	}
	if (flags & Buffer::Flags::MAP_PERSISTENT) {
		_glFlags |= GL_MAP_PERSISTENT_BIT;
	}
	if (flags & Buffer::Flags::MAP_COHERENT) {
		_glFlags |= GL_MAP_COHERENT_BIT;
	}
#endif // 
#if defined(_USE_OPENGL_ES_)
	glGenBuffers(1, &_hBuffer);
	glBindBuffer(GL_COPY_READ_BUFFER, _hBuffer);
	glBufferData(GL_COPY_READ_BUFFER, size, data, _usingMode);
#else
	glCreateBuffers(1, &_hBuffer);
	glNamedBufferStorage(_hBuffer, size, data, _glFlags);
#endif
}

//==================================================================================

void Buffer::_initClass() noexcept
{
	_counter = 0;
	_onceInit.store(false, std::memory_order_release);
}

//==================================================================================

void Buffer::_eraseFromCollection(Buffer* ptr)
{
	std::lock_guard<std::mutex> lk(_collectionMutex);
	if (ptr != nullptr) {
		_collection.erase(ptr->id());
		delete ptr;
	}
}

//==================================================================================

bool Buffer::_emplaceToCollection(const std::shared_ptr<Buffer>& ptr)
{
	std::lock_guard<std::mutex> lk(_collectionMutex);
	_collection.emplace(ptr->id(), ptr);
	return true;
}

//==================================================================================

Buffer::Buffer() noexcept :
	_hBuffer(0),
	_size(0),
	_flags(0),
#if defined (_USE_OPENGL_ES_)
	_usingMode(GL_STATIC_DRAW)
#else
	_glFlags(0)
#endif // _USE_OPENGL_ES_
{
	if (_onceInit.load(std::memory_order_acquire)) {
		std::lock_guard<std::mutex> lk(_mt);
		if (_onceInit.load(std::memory_order_acquire)) {
			_initClass();
		}
	}
	++_counter;
}

//==================================================================================

Buffer::Buffer(unsigned int size, unsigned int flags, const void* data) :
	_hBuffer(0),
	_size(size),
	_flags(flags),
#if defined (_USE_OPENGL_ES_)
	_usingMode(GL_STATIC_DRAW)
#else
	_glFlags(0)
#endif // _USE_OPENGL_ES_
{
	{
		std::lock_guard<std::mutex> lk(_mt);
		if (_onceInit.load(std::memory_order_acquire)) {
			_initClass();
		}
		++_counter;
	}
	auto context = GraphicContext::getDefault();
	if (context != nullptr) {
		context->setCurrent();
		_create(context.get(), size, flags, data);
	}
}

//==================================================================================

Buffer::Buffer(GraphicContext::Ptr context, unsigned int size, unsigned int flags, const void* data) :
	_hBuffer(0),
	_size(size),
	_flags(flags),
#if defined (_USE_OPENGL_ES_)
	_usingMode(GL_STATIC_DRAW)
#else
	_glFlags(0)
#endif // _USE_OPENGL_ES_
{
	{
		std::lock_guard<std::mutex> lk(_mt);
		if (_onceInit.load(std::memory_order_acquire)) {
			_initClass();
		}
		++_counter;
	}
	if (context == nullptr) {
		return;
	}
	context->setCurrent();
	_create(context, size, flags, data);
}

//==================================================================================

bool Buffer::getSubData(size_t offset, size_t size, void* ptr) const
{
	if (_hBuffer == 0 || ptr == nullptr || ((_flags & Buffer::Flags::DYNAMIC_STORAGE) == 0)) {
		return false;
	}
	_context->setCurrent();
#if defined(_USE_OPENGL_ES_)
	if (_context->_boundCopyReadBuffer != _id) {
		_context->_boundCopyReadBuffer = _id;
		glBindBuffer(GL_COPY_READ_BUFFER, _hBuffer);
	}
	glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, ptr);
#else
	glGetNamedBufferSubData(_hBuffer, offset, size, ptr);
#endif
	return true;
}

//==================================================================================

bool Buffer::storeSubData(size_t offset, size_t size, const void* ptr) const
{
	if (_hBuffer == 0 || ptr == nullptr || ((_flags & Buffer::Flags::DYNAMIC_STORAGE) == 0)) {
		return false;
	}
	_context->setCurrent();
#if defined(_USE_OPENGL_ES_)
	if (_context->_boundCopyWriteBuffer != _id) {
		_context->_boundCopyWriteBuffer = _id;
		glBindBuffer(GL_COPY_WRITE_BUFFER, _hBuffer);
	}
	glBufferSubData(GL_COPY_WRITE_BUFFER, offset, size, ptr);
#else
	glNamedBufferSubData(_hBuffer, offset, size, ptr);
#endif
	return true;
}

//==================================================================================

void Buffer::bindBuffer(int BP) noexcept
{
	if (_hBuffer && _context->_boundShaderStorageBuffers[BP] != _id) {
	_context->setCurrent();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BP, _hBuffer);
		_context->_boundShaderStorageBuffers[BP] = _id;
	}
}

//==================================================================================

void Buffer::unbindBuffer() noexcept
{
	if (!_hBuffer) {
		return;
	}
	_context->setCurrent();
#if defined(_USE_OPENGL_ES_)
#define _UNBIND_BUFFER_(a, token)  if (_context->a == _id) {	\
		_context->a = _id;										\
		glBindBuffer(token, 0);									\
	}
	_UNBIND_BUFFER_(_boundArrayBuffer, GL_ARRAY_BUFFER)
	_UNBIND_BUFFER_(_boundAtomicCounterBuffer, GL_ATOMIC_COUNTER_BUFFER)
	_UNBIND_BUFFER_(_boundCopyReadBuffer, GL_COPY_READ_BUFFER)
	_UNBIND_BUFFER_(_boundCopyWriteBuffer, GL_COPY_WRITE_BUFFER)
	_UNBIND_BUFFER_(_boundDispatchIndirectBuffer, GL_DISPATCH_INDIRECT_BUFFER)
	_UNBIND_BUFFER_(_boundDrawIndirectBuffer, GL_DRAW_INDIRECT_BUFFER)
	_UNBIND_BUFFER_(_boundElementArrayBuffer, GL_ELEMENT_ARRAY_BUFFER)
	_UNBIND_BUFFER_(_boundPixelPackBuffer, GL_PIXEL_PACK_BUFFER)
	_UNBIND_BUFFER_(_boundPixelUnpackBuffer, GL_PIXEL_UNPACK_BUFFER)
	_UNBIND_BUFFER_(_boundShaderStorageBuffer, GL_SHADER_STORAGE_BUFFER)
	_UNBIND_BUFFER_(_boundTextureBuffer, GL_TEXTURE_BUFFER)
	_UNBIND_BUFFER_(_boundTransformFeedbackBuffer, GL_TRANSFORM_FEEDBACK_BUFFER)
	_UNBIND_BUFFER_(_boundUniformBuffer, GL_UNIFORM_BUFFER)

#undef _UNBIND_BUFFER_
#endif
	const size_t BOUND_POINT_SIZE = _context->_boundShaderStorageBuffers.size();
	for (size_t i = 0; i < BOUND_POINT_SIZE; ++i) {
		if (_context->_boundShaderStorageBuffers[i] == _id) {
			_context->_boundShaderStorageBuffers[i] = Id<Buffer>::ID_TYPE_MAX;
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)i, 0);
		}
	}
}

//==================================================================================

std::shared_ptr<Buffer> Buffer::getFromCollection(Id<Buffer>::idType n)
{
	std::shared_ptr<Buffer> retValue;
	std::lock_guard<std::mutex> lk(_collectionMutex);
	auto iter = _collection.find(n);
	if (iter != _collection.end()) {
		retValue = iter->second.lock();
	}
	return retValue;
}

//==================================================================================

Buffer::~Buffer() noexcept
{
	_deleteBuffer();
	std::lock_guard<std::mutex> lk(_mt);
	--_counter;
	if (_counter == 0) {
		_onceInit.store(true, std::memory_order_release);
	}
}
