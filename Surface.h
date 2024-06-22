#pragma once
#ifndef SURFACE_H_
#define SURFACE_H_

#include "IndexBuffer.h"
#include "Interactor.h"
#include "necessaryMacro.h"
#include "OpenGLAux.h"
#include "ShaderProgram.h"
#include "UniformBlock.h"
#include "VertexArrayObject.h"
#include "VertexAttributesBuffer.h"
#include "Id.h"

#include <atomic>
#include <cstdint>
#include <limits.h>
#include <list>
#include <map>
#include <memory>


class DrawingQueueBase;
class Model;
#ifndef DRAWING_QUEUE_IS_DECLARATED_
#define DRAWING_QUEUE_IS_DECLARATED_ 1
template <typename T, typename KeyCompareFunc = std::less<T> >
class DrawingQueue;
#else
template <typename T, typename KeyCompareFunc>
class DrawingQueue;
#endif //

#define _SURFACE_NODISCARD_WARNING_ [[nodiscard]]

class Surface : public Interactor<float, std::shared_ptr<void>&>
{
private:
	friend Model;
	template <typename T, typename KeyCompareFunc> friend class DrawingQueue;
public:
#if defined (_M_AMD64) || defined (_M_ARM64) || defined(__LP64__)
	typedef uint_fast64_t idType;
	enum { ID_TYPE_MAX = ULLONG_MAX };
#else
	typedef unsigned int idType;
	enum { ID_TYPE_MAX = UINT_MAX };
#endif // _M_AMD64 || _M_ARM64
	class SearchBy
	{
	public:
		enum { BY_ID, BY_POSITION };
	};
	class Primitive_type
	{
	public:
		enum { POINTS, LINES, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN };
	};
protected:
	std::weak_ptr<Surface> _thisInstance;
	std::weak_ptr<Model> _parentInstance;
	static std::atomic<idType> _currentID;
	idType _surfaceID;

	Id<Surface> _id;

	bool _indexed;
	int _numInstances;
	struct VABuffersStruct
	{
		std::shared_ptr<VertexAttributesBuffer> buffer;
		int index;
		bool hasBound;
		bool hasDivided;
		VABuffersStruct(const std::shared_ptr<VertexAttributesBuffer>& p, int i) : buffer(p), index(i), hasBound(false), hasDivided(false)
		{
		}
	};
	struct UniformBlockStruct {
		std::shared_ptr <UniformBlock> _block;
		int _bindingPoint;
		UniformBlockStruct(const std::shared_ptr <UniformBlock>& p, int i) : _block(p), _bindingPoint(i) {}
	};
	VertexArrayObject _VAO;
	std::map<idType, VABuffersStruct> _bindedAttribBuffers;
	std::map<idType, UniformBlockStruct> _bindedUniformBlocks;
	std::atomic<idType> _currentFreeID;
	std::shared_ptr<IndexBuffer> _vertexIndices;
	std::map<idType, std::shared_ptr<void>> _attachedData;
	unsigned int _vertexNum;
	unsigned int _primitiveType;
	GraphicContext::Ptr _context;

	bool _made;
	std::shared_ptr<DrawingQueueBase> _queue;

	std::shared_ptr<ShaderProgram> _pShaderProgram;
	void _moveProc(Surface& a) noexcept;

	void _pass() const;
	// если поверхность не рисуется
	// доделать
	//void (*_notPass)(std::shared_ptr<void>&, std::shared_ptr<void>&);
	//std::shared_ptr<void> _arg1;
	//std::shared_ptr<void> _arg2;
	Surface();
	Surface(GraphicContext::Ptr context);
public:
	Surface(const Surface& a) = delete;
	Surface(Surface&& a) noexcept :
		Interactor(std::move(a))
	{
		_moveProc(a);
	}
	Surface& operator=(const Surface& a) = delete;
	Surface& operator=(Surface&& a) noexcept
	{
		if (this != &a) {
			Interactor::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	bool setShaderProgram(const std::shared_ptr<ShaderProgram>& p);
	std::shared_ptr<ShaderProgram> shaderProgram() const noexcept
	{
		return _pShaderProgram;
	}
	Surface::idType bindUniformBlock(const std::shared_ptr<UniformBlock>& b);
	bool bindIndices(const std::shared_ptr<IndexBuffer>& p, unsigned int num = 0);
	bool isIndexed() const noexcept
	{
		return _indexed;
	}
	void setNumVertices(unsigned int n) noexcept
	{
		if (_indexed) {
			if (_vertexIndices->numElements() >= n) {
				_vertexNum = n;
			}
		}
		else {
			_vertexNum = n;
		}
	}
	unsigned int numVertices() const noexcept
	{
		return _vertexNum;
	}
	void setNumInstances(int n) noexcept
	{
		if (n > 0) {
			_numInstances = n;
		}
	}
	int numInstances() const noexcept
	{
		return _numInstances;
	}
	_SURFACE_NODISCARD_WARNING_ Surface::idType bindVertexAttribBuffer(const std::shared_ptr<VertexAttributesBuffer>& p);
	_SURFACE_NODISCARD_WARNING_ Surface::idType attach(const std::shared_ptr<void>& pData);
	_SURFACE_NODISCARD_WARNING_ Surface::idType attach(const std::shared_ptr<UniformBlock>& b)
	{
		return bindUniformBlock(b);
	}
	bool detach(Surface::idType n) noexcept;
	std::shared_ptr<VertexAttributesBuffer> getVertexAttributesBuffer(Surface::idType n, unsigned int f = 0) const noexcept;
	std::shared_ptr<UniformBlock> getUniformBlock(Surface::idType n, unsigned int f = 0) const noexcept;
	std::shared_ptr<void> getData(Surface::idType n, unsigned int f = 0) const noexcept;
	unsigned int setPrimitiveType(unsigned int newType) noexcept
	{
		unsigned int oldType = _primitiveType;
		_primitiveType = newType;
		return oldType;
	}
	virtual bool build();
	void draw() const noexcept override;
	void update() const noexcept override;
	void clear() noexcept override;
	operator bool() const noexcept override
	{
		return _made;
	}
	static _SURFACE_NODISCARD_WARNING_ std::shared_ptr<Surface> sharedNew()
	{
		std::shared_ptr<Surface> shared(new Surface);
		shared->_thisInstance = shared;
		return shared;
	}
	static _SURFACE_NODISCARD_WARNING_ std::shared_ptr<Surface> sharedNew(GraphicContext::Ptr context)
	{
		std::shared_ptr<Surface> shared(new Surface(context));
		shared->_thisInstance = shared;
		return shared;
	}
	Surface::idType id() const noexcept
	{
		return _surfaceID;
	}
	const std::weak_ptr<Model>& parent() const noexcept
	{
		return _parentInstance;
	}
	void setQueue(const std::shared_ptr<DrawingQueueBase>& queue)
	{
		_queue = queue;
	}
	std::shared_ptr<DrawingQueueBase> queue() const
	{
		return _queue;
	}
	~Surface() noexcept override;
};
#endif // SURFACE_H_