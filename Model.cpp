#include "Model.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4834)
#pragma warning(disable: 6031)
#endif

//==================================================================================

void Model::_moveProc(Model& a) noexcept
{
	_currentAttachingID.store(a._currentAttachingID.load(std::memory_order_acquire), std::memory_order_release);
	_currentUniformBlockID.store(a._currentUniformBlockID.load(std::memory_order_acquire), std::memory_order_release);
	_models = std::move(a._models);
	_surfaces = std::move(a._surfaces);
	_attachedData = std::move(a._attachedData);
	_uniformBlocks = std::move(a._uniformBlocks);
	_isOK = a._isOK;
	a._isOK = false;
	_parentInstance = a._parentInstance;
	_id = std::move(a._id);
}

//==================================================================================

Model::Model() :
	_isOK(false),
	_currentAttachingID(0),
	_currentUniformBlockID(0)
{
}

//==================================================================================

void Model::setMoveMatrix(const mat4& M) noexcept
{
	mat4 clearMove = M * _aMatrix->inverse();
	mat4 MM;
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		MM = clearMove * (iter->second.get()->aMatrixM());
		iter->second.get()->setMoveMatrix(MM);
	}
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		MM = clearMove * (iter->second.get()->aMatrixM());
		iter->second.get()->setMoveMatrix(MM);
	}
	Transform::setMoveMatrix(M);
}

//==================================================================================

void Model::setBaseMatrix(const mat4& M) noexcept
{
	mat4 clearMove = *_aMatrix * M * _bMatrix->inverse() * _aMatrix->inverse();
	mat4 MM;
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		MM = clearMove * (iter->second.get()->aMatrixM());
		iter->second.get()->setMoveMatrix(MM);
	}
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		MM = clearMove * (iter->second.get()->aMatrixM());
		iter->second.get()->setMoveMatrix(MM);
	}
	Transform::setBaseMatrix(M);
}

//==================================================================================

void Model::setScaleMatrix(const mat4& M) noexcept
{
	mat4 clearMove = *_aMatrix * *_bMatrix * M * _scaleMatrix->inverse() * _bMatrix->inverse() * _aMatrix->inverse();
	mat4 MM;
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		MM = clearMove * (iter->second->aMatrixM());
		iter->second->setMoveMatrix(MM);
	}
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		MM = clearMove * (iter->second->aMatrixM());
		iter->second->setMoveMatrix(MM);
	}
	Transform::setScaleMatrix(M);
}

//==================================================================================

void Model::rotate(Transform::valueType angle, const Transform::vec3& v) noexcept
{
	mat4 rot = Transform::rotateM(angle, v);
	mat4 M = rot * *_bMatrix;
	setBaseMatrix(M);
}

//==================================================================================

void Model::translate(const Transform::vec3& v) noexcept
{
	mat4 tr = Transform::mat4Identity();
	tr(0, 3) = v(0);
	tr(1, 3) = v(1);
	tr(2, 3) = v(2);
	mat4 M = tr * *_bMatrix;
	setBaseMatrix(M);
}

//==================================================================================

void Model::scale(const Transform::vec3& v) noexcept
{
	mat4 M = Transform::mat4Identity();
	M(0, 0) = v(0);
	M(1, 1) = v(1);
	M(2, 2) = v(2);
	mat4 M2 = M * *_scaleMatrix;
	setScaleMatrix(M);
}

//==================================================================================

void Model::toOrigin() noexcept
{
	mat4 clearMove = *_aMatrix * _scaleMatrix->inverse() * _bMatrix->inverse() * _aMatrix->inverse();
	mat4 MM;
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		MM = clearMove * (iter->second->aMatrixM());
		iter->second->setMoveMatrix(MM);
	}
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		MM = clearMove * (iter->second->aMatrixM());
		iter->second->setMoveMatrix(MM);
	}
	Transform::toOrigin();
}

//==================================================================================

Model::idType Model::attach(const std::shared_ptr<Model>& m, bool local)
{
	if (m == std::shared_ptr<Model>()) {
		return ID_TYPE_MAX;
	}
	auto pMp = _modelIDMapped.find(m->id());
	if (pMp != _modelIDMapped.end()) {
		return ID_TYPE_MAX;
	}
	m->_parentInstance = _thisInstance;
	idType currID = _currentAttachingID.fetch_add(1, std::memory_order_acq_rel);
	_modelIDMapped.emplace(m->id(), currID);
	_models.emplace(currID, m);
	for (auto iter = _uniformBlocks.cbegin(); iter != _uniformBlocks.cend(); ++iter) {
		m->bindUniformBlock(iter->second);
	}
	if (local) {
		mat4 M = aMatrixM() * bMatrixM() * scaleMatrixM();
		m->setMoveMatrix(M);
	}
	return currID;
}

//==================================================================================

Model::idType Model::attach(const std::shared_ptr<Surface>& s, bool local)
{
	if (s == std::shared_ptr<Surface>()) {
		return ID_TYPE_MAX;
	}
	auto pMp = _surfaceIDMapped.find(s->id());
	if (pMp != _surfaceIDMapped.end()) {
		return ID_TYPE_MAX;
	}
	s->_parentInstance = _thisInstance;
	idType currID = _currentAttachingID.fetch_add(1, std::memory_order_acq_rel);
	_surfaceIDMapped.emplace(s->id(), currID);
	_surfaces.emplace(currID, s);
	for (auto iter = _uniformBlocks.cbegin(); iter != _uniformBlocks.cend(); ++iter) {
		s->bindUniformBlock(iter->second);
	}
	if (local) {
		mat4 M = aMatrixM() * bMatrixM() * scaleMatrixM();
		s->setMoveMatrix(M);
	}
	return currID;
}

//==================================================================================

Model::idType Model::attach(const std::shared_ptr<void>& pData)
{
	if (pData == nullptr) {
		return ID_TYPE_MAX;
	}
	idType currID = _currentAttachingID.fetch_add(1, std::memory_order_acq_rel);
	_attachedData.emplace(currID, pData);
	return currID;
}

//==================================================================================

bool Model::detach(Model::idType n)
{
	auto pModel = _models.find(n);
	if (pModel != _models.end()) {
		_modelIDMapped.erase(pModel->second->id());
		_models.erase(pModel);
		return true;
	}
	auto pSurface = _surfaces.find(n);
	if (pSurface != _surfaces.end()) {
		_surfaceIDMapped.erase(pSurface->second->id());
		_surfaces.erase(pSurface);
		return true;
	}
	auto pData = _attachedData.find(n);
	if (pData != _attachedData.end()) {
		_attachedData.erase(pData);
		return true;
	}
	return false;
}
//==================================================================================

std::shared_ptr<Model> Model::getAttachedModel(Model::idType n, int f) const noexcept
{
	if ((f == static_cast<int>(Model::SearchBy::BY_POSITION)) && (n < _models.size())) {
		auto pModel = _models.begin();
		for (int i = 0; i < n; ++i) {
			++pModel;
		}
		return pModel->second;
	}
	else if (f == static_cast<int>(Model::SearchBy::BY_ID)) {
		auto pModel = _models.find(n);
		if (pModel != _models.end()) {
			return pModel->second;
		}
	}
	return std::shared_ptr<Model>();
}

//==================================================================================

std::shared_ptr<Surface> Model::getAttachedSurface(Model::idType n, int f) const noexcept
{
	if ((f == static_cast<int>(Model::SearchBy::BY_POSITION)) && (n < _surfaces.size())) {
		auto pSurface = _surfaces.begin();
		for (int i = 0; i < n; ++i) {
			++pSurface;
		}
		return pSurface->second;
	}
	else if (f == static_cast<int>(Model::SearchBy::BY_ID)) {
		auto pSurface = _surfaces.find(n);
		if (pSurface != _surfaces.end()) {
			return pSurface->second;
		}
	}
	return std::shared_ptr<Surface>();
}

//==================================================================================

std::shared_ptr<void> Model::getAttachedData(Model::idType n, int f) const noexcept
{
	if ((f == static_cast<int>(Model::SearchBy::BY_POSITION)) && (n < _attachedData.size())) {
		auto pData = _attachedData.begin();
		for (int i = 0; i < n; ++i) {
			++pData;
		}
		return pData->second;
	}
	else if (f == static_cast<int>(Model::SearchBy::BY_ID)) {
		auto pData = _attachedData.find(n);
		if (pData != _attachedData.end()) {
			return pData->second;
		}
	}
	return std::shared_ptr<void>();
}


//==================================================================================

Model::idType Model::getAttachedModelId(Model::idType n, int f) const noexcept
{
	if ((f == static_cast<int>(Model::SearchBy::BY_POSITION)) && (n < _models.size())) {
		auto pModel = _models.cbegin();
		for (int i = 0; i < n; ++i) {
			++pModel;
		}
		return pModel->first;
	}
	else if (f == static_cast<int>(Model::SearchBy::BY_ID)) {
		auto p = _modelIDMapped.find(n);
		if (p != _modelIDMapped.end()) {
			return p->second;
		}
	}
	return ID_TYPE_MAX;
}

//==================================================================================

Model::idType Model::getAttachedSurfaceId(Surface::idType n, int f) const noexcept
{
	if ((f == static_cast<int>(Model::SearchBy::BY_POSITION)) && (n < _surfaces.size())) {
		auto pSurface = _surfaces.cbegin();
		for (int i = 0; i < n; ++i) {
			++pSurface;
		}
		return pSurface->first;
	}
	else if (f == static_cast<int>(Model::SearchBy::BY_ID)) {
		auto p = _surfaceIDMapped.find(n);
		if (p != _surfaceIDMapped.end()) {
			return p->second;
		}
	}
	return ID_TYPE_MAX;
}

//==================================================================================

Model::idType Model::getAttachedModelBoundIdByPosition(Model::idType n) const noexcept
{
	if (n >= _models.size()) {
		return ID_TYPE_MAX;
	}
	auto iter = _models.cbegin();
	for (size_t i = 0; i < n; ++i) {
		++iter;
	}
	return iter->first;
}

//==================================================================================

Model::idType Model::getAttachedSurfaceBoundIdByPosition(Model::idType n) const noexcept
{
	if (n >= _surfaces.size()) {
		return ID_TYPE_MAX;
	}
	auto iter = _surfaces.cbegin();
	for (size_t i = 0; i < n; ++i) {
		++iter;
	}
	return iter->first;
}

//==================================================================================

Model::idType Model::getAttachedDataBoundIdByPosition(Model::idType n) const noexcept
{
	if (n >= _attachedData.size()) {
		return ID_TYPE_MAX;
	}
	auto iter = _attachedData.cbegin();
	for (size_t i = 0; i < n; ++i) {
		++iter;
	}
	return iter->first;
}

//==================================================================================

Model::idType Model::bindUniformBlock(const std::shared_ptr<UniformBlock>& pUB)
{
	if (pUB == nullptr || !static_cast<bool>(*pUB)) {
		return UINT_MAX;
	}
	Model::idType ubID = _currentUniformBlockID.fetch_add(1, std::memory_order_acq_rel);
	_uniformBlocks.emplace(ubID, pUB);
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		iter->second.get()->bindUniformBlock(pUB);
	}
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		iter->second.get()->bindUniformBlock(pUB);
	}
	return ubID;
}

//==================================================================================

void Model::update() const noexcept
{
	for (auto iter = _surfaces.cbegin(); iter != _surfaces.cend(); ++iter) {
		iter->second.get()->update();
	}
	for (auto iter = _models.cbegin(); iter != _models.cend(); ++iter) {
		iter->second.get()->update();
	}
	if (_surfaces.empty() && _models.empty()) {
		for (auto iter = _uniformBlocks.cbegin(); iter != _uniformBlocks.cend(); ++iter) {
			iter->second.get()->update();
		}
	}
}

//==================================================================================

void Model::draw() const noexcept
{
	if (_hiden) {
		return;
	}
	Interactor::_predrawFuncRun();
	for (auto iter = _surfaces.cbegin(); iter != _surfaces.cend(); ++iter) {
		iter->second.get()->draw();
	}
	for (auto iter = _models.cbegin(); iter != _models.cend(); ++iter) {
		iter->second.get()->draw();
	}
	Interactor::_postdrawFuncRun();
}

//==================================================================================

void Model::clear() noexcept
{
	//Interactor::clear();
	//_models.clear();
	//_surfaces.clear();
	//_attachedData.clear();
	//_currentAttachingID.store(0, std::memory_order_release);
	//_modelIDMapped.clear();
	//_surfaceIDMapped.clear();
	//_uniformBlocks.clear();
	//_currentUniformBlockID.store(0, std::memory_order_release);
	//_isOK = false;
	//_parentInstance.reset();
	//_thisInstance.reset();
	*this = Model();
}

//==================================================================================

void Model::hide(bool b) noexcept
{
	if (Interactor::hiden() == b) {
		return;
	}
	Interactor::hide(b);
	for (auto iter = _models.begin(); iter != _models.end(); ++iter) {
		iter->second.get()->hide(_hiden);
	}
	for (auto iter = _surfaces.begin(); iter != _surfaces.end(); ++iter) {
		iter->second.get()->hide(_hiden);
	}
}

//==================================================================================

Model::~Model()
{
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
