#pragma once
#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include "ShaderProgramException.h"
#include "ShaderProgramStructs.h"
#include "Shader.h"
#include "Id.h"

#include <vector>
#include <list>
#include <string>
#include <filesystem>
#include <fstream>
#include <atomic>
#include <unordered_map>
#include <mutex>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif

//If need to generate exceptions
//uncomment code below
#define USE_SHADER_PROGRAM_EXCEPTION 1
#define USE_BINDING_POINT_HAVING_BEEN_SET_IN_SHADER 1

class ShaderProgram final
{
	friend class UniformBlock;
public:
	using Ptr = std::shared_ptr<ShaderProgram>;
private:
	static std::mutex _collectionMutex;
	static std::unordered_map<Id<ShaderProgram>::idType, std::weak_ptr<ShaderProgram> > _collection;

	static std::string _emptyString;
	std::string _programName;
	Id<ShaderProgram> _id;
	unsigned int _programHandle;
	std::list<Shader> _shaderList;
	bool _programLinked;
	std::string _errorMsg;
	GraphicContext::Ptr _context;
	bool _compute;

	std::vector<ProgramInput> _programInput;
	std::vector<ProgramUniform> _programUniform;
	std::vector<ProgramUniformBlock> _programUniformBlock;
	std::vector<ProgramShaderStorageBlock> _programShaderStorage;
	std::vector<ProgramAtomicCounter> _programAtomicCounter;
#if defined (_USE_OPENGL_ES_)
#else
	std::vector<ProgramSubroutineUniform> _programSubroutineUniform;
	std::vector<ProgramSubroutine> _programSubroutine;
#endif // _USE_OPENGL_ES_
	std::vector<ProgramTransformFeedbackVarying> _programTransformFeedbackVarying;
	std::vector<ProgramTransformFeedbackBuffer> _programTransformFeedbackBuffer;
	std::vector<ProgramBufferVariable> _programBufferVariable;

	std::vector<ProgramInput> _getInput();
	std::vector<ProgramUniform> _getUniform();
	std::vector<ProgramUniformBlock> _getUniformBlock();
	std::vector<ProgramShaderStorageBlock> _getShaderStorage();
	std::vector<ProgramAtomicCounter> _getAtomicCounter();
#if defined (_USE_OPENGL_ES_)
#else
	std::vector<ProgramSubroutineUniform> _getSubroutineUniform();
	std::vector<ProgramSubroutine> _getSubroutine();
#endif // _USE_OPENGL_ES_
	std::vector<ProgramTransformFeedbackVarying> _getTransformFeedbackVarying();
	std::vector<ProgramTransformFeedbackBuffer> _getTransformFeedbackBuffer();
	std::vector<ProgramBufferVariable> _getBufferVariable();

	int _typeComponentNum(int i) const noexcept;
	int _typeAtomType(int i) const noexcept;
	void _moveProc(ShaderProgram& a) noexcept;
	void _deleteProgram() noexcept;
	static void _eraseFromCollection(ShaderProgram* ptr);
	static bool _emplaceToCollection(const std::shared_ptr<ShaderProgram>& ptr);
protected:
	ShaderProgram() :
		_programHandle(0),
		_programLinked(false),
		_context(nullptr),
		_compute(false)
	{
		auto context = GraphicContext::getDefault();
		if (context != nullptr) {
			_context = context.get();
		}
		_programName = "Shader program " + std::to_string(_id);
	}
	ShaderProgram(const std::string& str) :
		_programHandle(0),
		_programLinked(false),
		_programName(str),
		_context(nullptr),
		_compute(false)
	{
		auto context = GraphicContext::getDefault();
		if (context != nullptr) {
			_context = context.get();
		}
	}
	ShaderProgram(GraphicContext::Ptr context) :
		_programHandle(0),
		_programLinked(false),
		_context(context),
		_compute(false)
	{
		_programName = "Shader program " + std::to_string(_id);
	}
	ShaderProgram(GraphicContext::Ptr context, const std::string& str) :
		_programHandle(0),
		_programLinked(false),
		_programName(str),
		_context(context),
		_compute(false)
	{
	}
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& a) noexcept :
		_programHandle(0),
		_programLinked(false),
		_id(std::move(a._id))
	{
		_moveProc(a);
	}
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& a) noexcept
	{
		if (&a != this) {
			_moveProc(a);
		}
		return *this;
	}
public:
	static std::shared_ptr<ShaderProgram> sharedNew(const std::string& str)
	{
		std::shared_ptr<ShaderProgram> ptr(new ShaderProgram(str), ShaderProgram::_eraseFromCollection);
		_emplaceToCollection(ptr);
		return ptr;
	}
	static std::shared_ptr<ShaderProgram> sharedNew()
	{
		std::shared_ptr<ShaderProgram> ptr(new ShaderProgram, ShaderProgram::_eraseFromCollection);
		_emplaceToCollection(ptr);
		return ptr;
	}
	static std::shared_ptr<ShaderProgram> sharedNew(GraphicContext::Ptr context)
	{
		std::shared_ptr<ShaderProgram> ptr(new ShaderProgram(context), ShaderProgram::_eraseFromCollection);
		_emplaceToCollection(ptr);
		return ptr;
	}
	static std::shared_ptr<ShaderProgram> sharedNew(GraphicContext::Ptr context, const std::string& str)
	{
		std::shared_ptr<ShaderProgram> ptr(new ShaderProgram(context, str), ShaderProgram::_eraseFromCollection);
		_emplaceToCollection(ptr);
		return ptr;
	}
	bool appendShaderFromFile(const std::wstring& fName, int shType);
	bool appendShaderFromFile(const std::wstring& fName);
	bool appendShaderFromTxt(const char* str, int shType);
	bool appendShaderFromTxt(const std::string& str, int shType);
	bool link();

	unsigned int handle() const noexcept
	{
		return _programHandle;
	}

	int attribNum() const noexcept
	{
		return _programInput.size();
	}
	const std::string& attibNameByIndex(int index) const noexcept
	{
		if (index >= 0 && index < _programInput.size()) {
			return _programInput[index].name;
		}
		else {
			return _emptyString;
		}
	}
	int attribLocation(const std::string& attribName) const noexcept;
	int attribLocationByIndex(int index) const noexcept
	{
		if (index >= 0 && index < _programInput.size()) {
			return _programInput[index].properties.location;
		}
		else {
			return -1;
		}
	}
	int attribType(const std::string& attribName) const noexcept;
	int attribTypeByIndex(int index) const noexcept
	{
		if (index >= 0 && index < _programInput.size()) {
			return _programInput[index].properties.type;
		}
		else {
			return -1;
		}
	}
	int attribAtomComponentNum(const std::string& attribName) const noexcept;
	int attribAtomComponentNumByIndex(int index) const noexcept
	{
		if (index >= 0 && index < _programInput.size()) {
			return _typeComponentNum(_programInput[index].properties.type);
		}
		else {
			return -1;
		}
	}
	int attribAtomType(const std::string& attribName) const noexcept;
	int attribAtomTypeByIndex(int index) const noexcept
	{
		if (index >= 0 && index < _programInput.size()) {
			return _typeAtomType(_programInput[index].properties.type);
		}
		else {
			return -1;
		}
	}

	int uniformNum() const noexcept
	{
		return _programUniform.size();
	}
	int uniformLocation(const std::string& uniformName) const noexcept;

	int uniformBlockIndex(const std::string& blockName) const noexcept;
	int uniformBlockBinding(const std::string& blockName) const noexcept;

	int shaderStorageBlockBinding(const std::string& blockName) const noexcept;

	void setActive() const noexcept
	{
		if (_programLinked && (_context->_activeProgram != _id)) {
			_context->setCurrent();
			_context->_activeProgram = _id;
			glUseProgram(_programHandle);
		}
	}
	static const char* getTypeString(int v);
	static const wchar_t* getTypeWString(int v);
	static int baseTypeComponentSize(int v) noexcept;
	static int baseTypeComponentNum(int v) noexcept;

	unsigned int  subroutineIndex(const std::string& subroutineName) const noexcept;
	int subroutineShaderType(const std::string& subroutineName) const noexcept;

	operator bool() const noexcept
	{
		return _programLinked;
	}
	Id<ShaderProgram>::idType id() const noexcept
	{
		return _id;
	}
	static std::shared_ptr<ShaderProgram> getFromCollection(Id<ShaderProgram>::idType n);
	const std::string& name() const noexcept
	{
		return _programName;
	}
	const std::string& error() const noexcept
	{
		return _errorMsg;
	}
	bool computing() const noexcept
	{
		return _compute;
	}
	GraphicContext::Ptr context() const noexcept
	{
		return _context;
	}
	~ShaderProgram() noexcept;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif // SHADERPROGRAM_H_