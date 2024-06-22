#pragma once
#ifndef MODEL_H_
#define MODEL_H_

#include "Interactor.h"
#include "Surface.h"
#include "UniformBlock.h"
#include "Id.h"

#include <atomic>
#include <cstdint>
#include <limits.h>
#include <map>
#include <memory>

#ifndef MODEL_NODISCARD_WARNING_
#define MODEL_NODISCARD_WARNING_ [[nodiscard]]
#endif // MODEL_NODISCARD_WARNING_

class Model : public Interactor<float, std::shared_ptr<void>&>
{
public:
#if defined (_M_AMD64) || defined (_M_ARM64) || defined (__LP64__)
	typedef uint_fast64_t idType;
	enum { ID_TYPE_MAX = ULLONG_MAX };
#else
	typedef unsigned int idType;
	enum { ID_TYPE_MAX = UINT_MAX };
#endif // _M_AMD64 || _M_ARM64 || __LP64__
public:
	struct SearchBy
	{
		enum { BY_ID, BY_POSITION };
	};
protected:
	std::weak_ptr<Model> _thisInstance;
	std::weak_ptr<Model> _parentInstance;

	Model::idType _modelID;

	Id<Model> _id;

	std::map<Model::idType, std::shared_ptr<Model>> _models;
	std::map<Model::idType, std::shared_ptr<Surface>> _surfaces;
	std::map<Model::idType, std::shared_ptr<void>> _attachedData;
	std::atomic<idType> _currentAttachingID;
	std::map<Model::idType, Model::idType> _modelIDMapped;
	std::map<Surface::idType, Model::idType> _surfaceIDMapped;

	std::map<idType, std::shared_ptr<UniformBlock>> _uniformBlocks;
	std::atomic<idType> _currentUniformBlockID;
	bool _isOK;
	void _moveProc(Model& a) noexcept;
	Model();
public:
	Model(const Model&) = delete;
	Model(Model&& a) noexcept :
		Interactor(std::move(a))
	{
		_moveProc(a);
	}
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&& a) noexcept
	{
		if (this != &a) {
			Interactor::operator=(std::move(a));
			_moveProc(a);
		}
		return *this;
	}
	void setMoveMatrix(const mat4& M) noexcept override;
	void setBaseMatrix(const mat4& M) noexcept override;
	void setScaleMatrix(const mat4& M) noexcept override;
	void rotate(Transform::valueType angle, Transform::valueType x, Transform::valueType y, Transform::valueType z) noexcept override
	{
		Transform::vec3 v(x, y, z);
		rotate(angle, v);
	}
	void rotate(Transform::valueType angle, const Transform::vec3& v) noexcept override;
	void translate(Transform::valueType x, Transform::valueType y, Transform::valueType z) noexcept override
	{
		Transform::vec3 v(x, y, z);
		translate(v);
	}
	void translate(const Transform::vec3& v) noexcept override;
	void scale(Transform::valueType x, Transform::valueType y, Transform::valueType z) noexcept override
	{
		Transform::vec3 v(x, y, z);
		scale(v);
	}
	void scale(const Transform::vec3& v) noexcept override;
	void toOrigin() noexcept override;
	MODEL_NODISCARD_WARNING_ Model::idType attach(const std::shared_ptr<Model>& m, bool local = true);
	MODEL_NODISCARD_WARNING_ Model::idType attach(const std::shared_ptr<Surface>& s, bool local = true);
	MODEL_NODISCARD_WARNING_ Model::idType attach(const std::shared_ptr<void>& pData);
	MODEL_NODISCARD_WARNING_ Model::idType attach(const std::shared_ptr<UniformBlock>& pUB);
	bool detach(Model::idType n);
	std::shared_ptr<Model> getAttachedModel(Model::idType n, int f = 0) const noexcept;
	std::shared_ptr<Surface> getAttachedSurface(Model::idType n, int f = 0) const noexcept;
	std::shared_ptr<void> getAttachedData(Model::idType n, int f = 0) const noexcept;
	idType getAttachedModelId(Model::idType n, int f = 0) const noexcept;
	idType getAttachedSurfaceId(Surface::idType n, int f = 0) const noexcept;
	idType getAttachedModelBoundIdByPosition(Model::idType n) const noexcept;
	idType getAttachedSurfaceBoundIdByPosition(Model::idType n) const noexcept;
	idType getAttachedDataBoundIdByPosition(Model::idType n) const noexcept;

	MODEL_NODISCARD_WARNING_ idType bindUniformBlock(const std::shared_ptr<UniformBlock>& pUB);
	MODEL_NODISCARD_WARNING_ static std::shared_ptr<Model> sharedNew()
	{
		std::shared_ptr<Model> shared(new Model);
		shared->_thisInstance = shared;
		return shared;
	}
	const std::weak_ptr<Model>& parent() const noexcept
	{
		return _parentInstance;
	}
	void update() const noexcept override;
	void draw() const noexcept override;
	void clear() noexcept override;
	void hide(bool b) noexcept override;
	size_t numAttachedModels() const noexcept
	{
		return _models.size();
	}
	size_t numAttachedSurfaces() const noexcept
	{
		return _surfaces.size();
	}
	size_t numAttachedData() const noexcept
	{
		return _attachedData.size();
	}
	Id<Model>::idType id() const noexcept
	{
		return _id;
	}
	operator bool() const noexcept override
	{
		return !_models.empty() || !_surfaces.empty();
	}
	~Model() noexcept override;
};

#endif // MODEL_H_
