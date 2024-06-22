#include "ShaderProgram.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4244)
#endif

std::mutex ShaderProgram::_collectionMutex;
std::unordered_map<Id<ShaderProgram>::idType, std::weak_ptr<ShaderProgram> > ShaderProgram::_collection;

std::string ShaderProgram::_emptyString;

//==================================================================================

std::vector<ProgramInput> ShaderProgram::_getInput()
{
	_context->setCurrent();
	int inputNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputNum);
	std::vector<ProgramInput> retValue(inputNum);
	if (inputNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_TYPE,
			GL_ARRAY_SIZE,
			GL_REFERENCED_BY_VERTEX_SHADER,
			GL_REFERENCED_BY_TESS_CONTROL_SHADER,
			GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
			GL_REFERENCED_BY_GEOMETRY_SHADER,
			GL_REFERENCED_BY_FRAGMENT_SHADER,
			GL_REFERENCED_BY_COMPUTE_SHADER,
			GL_LOCATION,
			GL_IS_PER_PATCH,
			GL_LOCATION_COMPONENT
		};
		const int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < inputNum; ++i) {
			int readBytes;
			glGetProgramResourceiv(_programHandle, GL_PROGRAM_INPUT, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readBytes, (int*)(&retValue[i].properties));
			int readNameBytes;
			glGetProgramResourceName(_programHandle, GL_PROGRAM_INPUT, i, MAX_NAME_LENGTH, &readNameBytes, inputName);
			inputName[readNameBytes] = '\0';
			retValue[i].name = inputName;
		}
	}
	return retValue;
}

//==================================================================================

std::vector<ProgramUniform> ShaderProgram::_getUniform()
{
	_context->setCurrent();
	int uniformNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformNum);
	std::vector<ProgramUniform> retValue(uniformNum);
	if (uniformNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_TYPE,
			GL_ARRAY_SIZE,
			GL_OFFSET,
			GL_BLOCK_INDEX,
			GL_ARRAY_STRIDE,
			GL_MATRIX_STRIDE,
			GL_IS_ROW_MAJOR,
			GL_ATOMIC_COUNTER_BUFFER_INDEX,
			GL_REFERENCED_BY_VERTEX_SHADER,
			GL_REFERENCED_BY_TESS_CONTROL_SHADER,
			GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
			GL_REFERENCED_BY_GEOMETRY_SHADER,
			GL_REFERENCED_BY_FRAGMENT_SHADER,
			GL_REFERENCED_BY_COMPUTE_SHADER,
			GL_LOCATION
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < uniformNum; ++i) {
			int readBytes;
			glGetProgramResourceiv(_programHandle, GL_UNIFORM, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readBytes, (int*)(&retValue[i].properties));
			int readNameBytes;
			glGetProgramResourceName(_programHandle, GL_UNIFORM, i, MAX_NAME_LENGTH, &readNameBytes, inputName);
			inputName[retValue[i].properties.nameLength] = '\0';
			retValue[i].name = inputName;
			//
			constexpr int UNIFORM_IN_BLOCK = -1;
			if (retValue[i].properties.location = UNIFORM_IN_BLOCK) {
				retValue[i].blockMember = true;
				const size_t DOT_POS = retValue[i].name.find('.');
				retValue[i].blockName = retValue[i].name.substr(DOT_POS + 1);
			}
			else {
				retValue[i].blockMember = false;
			}
		}
	}
	return retValue;
}

//==================================================================================

std::vector<ProgramUniformBlock> ShaderProgram::_getUniformBlock()
{
	_context->setCurrent();
	int uniformBlockNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockNum);
	std::vector<ProgramUniformBlock> retValue(uniformBlockNum);
	if (uniformBlockNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_BUFFER_BINDING,
			GL_BUFFER_DATA_SIZE,
			GL_NUM_ACTIVE_VARIABLES,
			GL_ACTIVE_VARIABLES,
			GL_REFERENCED_BY_VERTEX_SHADER,
			GL_REFERENCED_BY_TESS_CONTROL_SHADER,
			GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
			GL_REFERENCED_BY_GEOMETRY_SHADER,
			GL_REFERENCED_BY_FRAGMENT_SHADER,
			GL_REFERENCED_BY_COMPUTE_SHADER
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < uniformBlockNum; ++i) {
			int readBytes;
			glGetProgramResourceiv(_programHandle, GL_UNIFORM_BLOCK, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readBytes, (int*)(&retValue[i].properties));
			int readNameBytes;
			glGetProgramResourceName(_programHandle, GL_UNIFORM_BLOCK, i, MAX_NAME_LENGTH, &readNameBytes, inputName);
			inputName[retValue[i].properties.nameLength] = '\0';
			retValue[i].name = inputName;
			int fieldNum = 0;
			for (int j = 0; j < _programUniform.size(); ++j) {
				if (_programUniform[j].properties.blockIndex == i) {
					++fieldNum;
				}
			}
			retValue[i].fields.resize(fieldNum);
			int k = 0;
			for (int j = 0; j < _programUniform.size(); ++j) {
				if (_programUniform[j].properties.blockIndex == i) {
					size_t delim = _programUniform[j].name.find(".");
					retValue[i].fields[k].name = _programUniform[j].name.substr(delim + 1);
					retValue[i].fields[k].offset = _programUniform[j].properties.offset;
					retValue[i].fields[k].arraySize = _programUniform[j].properties.arraySize;
					retValue[i].fields[k].arrayStride = _programUniform[j].properties.arrayStride;
					retValue[i].fields[k].type = _programUniform[j].properties.type;
					retValue[i].fields[k].componentSize = baseTypeComponentSize(retValue[i].fields[k].type);
					retValue[i].fields[k].componentNum = baseTypeComponentNum(retValue[i].fields[k].type);
					retValue[i].fields[k].matrixStride = _programUniform[j].properties.matrixStide;
					int size = baseTypeComponentSize(retValue[i].fields[k].type);
					if (baseTypeComponentNum(retValue[i].fields[k].type) > 1) {
						size = baseTypeComponentNum(retValue[i].fields[k].type) * retValue[i].fields[k].matrixStride;
					}
					if (retValue[i].fields[k].arraySize > 1) {
						size = retValue[i].fields[k].arrayStride * retValue[i].fields[k].arraySize;
					}
					retValue[i].fields[k].size = size;
					++k;
				}
			}
		}
#ifndef USE_BINDING_POINT_HAVING_BEEN_SET_IN_SHADER
		for (int i = 0; i < uniformBlockNum; ++i) {
			glUniformBlockBinding(_programHandle, i, i);
			retValue[i].properties.bufferBinding = i;
		}
#endif // USE_BINDING_POINT_HAVING_SET_IN_SHADER
	}
	return retValue;
}

//==================================================================================

std::vector<ProgramShaderStorageBlock> ShaderProgram::_getShaderStorage()
{
	_context->setCurrent();
	int uniformBlockNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockNum);
	std::vector<ProgramShaderStorageBlock> retValue(uniformBlockNum);
	if (uniformBlockNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_BUFFER_BINDING,
			GL_BUFFER_DATA_SIZE,
			GL_NUM_ACTIVE_VARIABLES,
			GL_ACTIVE_VARIABLES,
			GL_REFERENCED_BY_VERTEX_SHADER,
			GL_REFERENCED_BY_TESS_CONTROL_SHADER,
			GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
			GL_REFERENCED_BY_GEOMETRY_SHADER,
			GL_REFERENCED_BY_FRAGMENT_SHADER,
			GL_REFERENCED_BY_COMPUTE_SHADER
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < uniformBlockNum; ++i) {
			int readBytes;
			glGetProgramResourceiv(_programHandle, GL_SHADER_STORAGE_BLOCK, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readBytes, (int*)(&retValue[i].properties));
			int readNameBytes;
			glGetProgramResourceName(_programHandle, GL_SHADER_STORAGE_BLOCK, i, MAX_NAME_LENGTH, &readNameBytes, inputName);
			inputName[retValue[i].properties.nameLength] = '\0';
			retValue[i].name = inputName;
		}
		for (int i = 0; i < uniformBlockNum; ++i) {
			glShaderStorageBlockBinding(_programHandle, i, i);
			retValue[i].properties.bufferBinding = i;
		}
	}
	return retValue;
}

//==================================================================================

std::vector<ProgramAtomicCounter> ShaderProgram::_getAtomicCounter()
{
	_context->setCurrent();
	int atomicCounterNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_ATOMIC_COUNTER_BUFFER, GL_ACTIVE_RESOURCES, &atomicCounterNum);
	std::vector<ProgramAtomicCounter> retValue(atomicCounterNum);
	if (atomicCounterNum) {
		const unsigned int properties[] = {
			GL_BUFFER_BINDING,
			GL_BUFFER_DATA_SIZE,
			GL_NUM_ACTIVE_VARIABLES,
			GL_ACTIVE_VARIABLES
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		for (int i = 0; i < atomicCounterNum; ++i) {
			int readProperties;
			glGetProgramResourceiv(_programHandle, GL_ATOMIC_COUNTER_BUFFER, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readProperties, (GLint*)&retValue[i].properties);
			for (int j = 0; j < _programUniform.size(); ++j) {
				if (_programUniform[j].properties.atomicCounterBufferIndex == i) {
					retValue[i].name = _programUniform[j].name;
					retValue[i].offset = _programUniform[j].properties.offset;
				}
			}
		}
	}
	return retValue;
}

//==================================================================================

#if defined (_USE_OPENGL_ES_)
#else
std::vector<ProgramSubroutineUniform> ShaderProgram::_getSubroutineUniform()
{
	_context->setCurrent();
	unsigned int shaderTypes[]{ GL_VERTEX_SHADER,
								GL_TESS_CONTROL_SHADER,
								GL_TESS_EVALUATION_SHADER,
								GL_GEOMETRY_SHADER,
								GL_FRAGMENT_SHADER,
								GL_COMPUTE_SHADER };
	const int shaderTypesArraySize = sizeof(shaderTypes) / sizeof(shaderTypes[0]);
	unsigned int subroutineTypes[]{ GL_VERTEX_SUBROUTINE_UNIFORM,
									GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
									GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
									GL_GEOMETRY_SUBROUTINE_UNIFORM,
									GL_FRAGMENT_SUBROUTINE_UNIFORM,
									GL_COMPUTE_SUBROUTINE_UNIFORM
	};
	const int subroutineTypesArraySize = sizeof(subroutineTypes) / sizeof(subroutineTypes[0]);
	int subroutineTypeNums[subroutineTypesArraySize];
	int subroutineNum = 0;
	for (int i = 0; i < subroutineTypesArraySize; ++i) {
		glGetProgramInterfaceiv(_programHandle, subroutineTypes[i], GL_ACTIVE_RESOURCES, subroutineTypeNums + i);
		subroutineNum += subroutineTypeNums[i];
	}
	std::vector<ProgramSubroutineUniform> retValue(subroutineNum);
	if (subroutineNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_ARRAY_SIZE,
			GL_NUM_COMPATIBLE_SUBROUTINES,
			GL_COMPATIBLE_SUBROUTINES,
			GL_LOCATION
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		int index = 0;
		constexpr int MAX_NAME_LENGTH = 256;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < subroutineTypesArraySize; ++i) {
			for (int j = 0; j < subroutineTypeNums[i]; ++j) {
				int readProperties;
				glGetProgramResourceiv(_programHandle, subroutineTypes[i], j, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readProperties, (GLint*)&retValue[index].properties);
				int readNameBytes;
				glGetProgramResourceName(_programHandle, subroutineTypes[i], j, MAX_NAME_LENGTH, &readNameBytes, inputName);
				inputName[retValue[index].properties.nameLength] = '\0';
				retValue[index].name = inputName;
				retValue[index].shaderType = shaderTypes[i];
				++index;
			}
		}
	}
	return retValue;
}

//==================================================================================

std::vector<ProgramSubroutine> ShaderProgram::_getSubroutine()
{
	_context->setCurrent();
	unsigned int shaderTypes[]{ GL_VERTEX_SHADER,
								GL_TESS_CONTROL_SHADER,
								GL_TESS_EVALUATION_SHADER,
								GL_GEOMETRY_SHADER,
								GL_FRAGMENT_SHADER,
								GL_COMPUTE_SHADER };
	const int shaderTypesArraySize = sizeof(shaderTypes) / sizeof(shaderTypes[0]);
	unsigned int subroutineTypes[]{ GL_VERTEX_SUBROUTINE,
								GL_TESS_CONTROL_SUBROUTINE,
								GL_TESS_EVALUATION_SUBROUTINE,
								GL_GEOMETRY_SUBROUTINE,
								GL_FRAGMENT_SUBROUTINE,
								GL_COMPUTE_SUBROUTINE
	};
	const int subroutineTypesArraySize = sizeof(subroutineTypes) / sizeof(subroutineTypes[0]);
	int subroutineTypeNums[subroutineTypesArraySize];
	int subroutineNum = 0;
	for (int i = 0; i < subroutineTypesArraySize; ++i) {
		glGetProgramInterfaceiv(_programHandle, subroutineTypes[i], GL_ACTIVE_RESOURCES, subroutineTypeNums + i);
		subroutineNum += subroutineTypeNums[i];
	}
	std::vector<ProgramSubroutine> retValue(subroutineNum);
	if (subroutineNum) {
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		int index = 0;
		for (int i = 0; i < subroutineTypesArraySize; ++i) {
			for (int j = 0; j < subroutineTypeNums[i]; ++j) {
				int readNameBytes;
				glGetActiveSubroutineName(_programHandle, shaderTypes[i], j, MAX_NAME_LENGTH, &readNameBytes, inputName);
				inputName[readNameBytes] = '\0';
				retValue[index].name = inputName;
				retValue[index].shaderType = shaderTypes[i];
				retValue[index].index = j;
				++index;
			}
		}
	}
	return retValue;
}
#endif // _USE_OPENGL_ES_

//==================================================================================

std::vector<ProgramTransformFeedbackVarying> ShaderProgram::_getTransformFeedbackVarying()
{
	_context->setCurrent();
	int varyingNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_TRANSFORM_FEEDBACK_VARYING, GL_ACTIVE_RESOURCES, &varyingNum);
	std::vector<ProgramTransformFeedbackVarying> temp(varyingNum);
	if (varyingNum) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_TYPE,
			GL_ARRAY_SIZE,
			GL_OFFSET,
			GL_TRANSFORM_FEEDBACK_BUFFER_INDEX
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < varyingNum; ++i) {
			int readProperties;
			glGetProgramResourceiv(_programHandle, GL_TRANSFORM_FEEDBACK_VARYING, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readProperties, (int*)&temp[i].properties);
			int readBytes;
			glGetProgramResourceName(_programHandle, GL_TRANSFORM_FEEDBACK_VARYING, i, MAX_NAME_LENGTH, &readBytes, inputName);
			inputName[temp[i].properties.nameLength] = '\0';
			temp[i].name = inputName;
		}
	}
	return temp;
}

//==================================================================================

std::vector<ProgramTransformFeedbackBuffer> ShaderProgram::_getTransformFeedbackBuffer()
{
	_context->setCurrent();
	int transformFeedbackNum = 0;
	glGetProgramInterfaceiv(_programHandle, GL_TRANSFORM_FEEDBACK_BUFFER, GL_ACTIVE_RESOURCES, &transformFeedbackNum);
	std::vector<ProgramTransformFeedbackBuffer> temp(transformFeedbackNum);
	if (transformFeedbackNum) {
		const unsigned int properties[] = {
			GL_BUFFER_BINDING,
			GL_NUM_ACTIVE_VARIABLES,
			GL_ACTIVE_VARIABLES,
			GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		for (int i = 0; i < transformFeedbackNum; ++i) {
			int readProperties;
			glGetProgramResourceiv(_programHandle, GL_TRANSFORM_FEEDBACK_BUFFER, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readProperties, (int*)&temp[i].properties);
		}
	}
	return temp;
}

//==================================================================================

std::vector<ProgramBufferVariable> ShaderProgram::_getBufferVariable()
{
	_context->setCurrent();
	int bufferVariable = 0;
	glGetProgramInterfaceiv(_programHandle, GL_BUFFER_VARIABLE, GL_ACTIVE_RESOURCES, &bufferVariable);
	std::vector<ProgramBufferVariable> temp(bufferVariable);
	if (bufferVariable) {
		const unsigned int properties[] = {
			GL_NAME_LENGTH,
			GL_TYPE,
			GL_ARRAY_SIZE,
			GL_OFFSET,
			GL_BLOCK_INDEX,
			GL_ARRAY_STRIDE,
			GL_MATRIX_STRIDE,
			GL_IS_ROW_MAJOR,
			GL_REFERENCED_BY_VERTEX_SHADER,
			GL_REFERENCED_BY_TESS_CONTROL_SHADER,
			GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
			GL_REFERENCED_BY_GEOMETRY_SHADER,
			GL_REFERENCED_BY_FRAGMENT_SHADER,
			GL_REFERENCED_BY_COMPUTE_SHADER,
			GL_TOP_LEVEL_ARRAY_SIZE,
			GL_TOP_LEVEL_ARRAY_STRIDE
		};
		constexpr int PROPERTIES_NUM = sizeof(properties) / sizeof(properties[0]);
		constexpr int MAX_NAME_LENGTH = 1024;
		char inputName[MAX_NAME_LENGTH];
		for (int i = 0; i < bufferVariable; ++i) {
			int readBytes;
			glGetProgramResourceiv(_programHandle, GL_UNIFORM, i, PROPERTIES_NUM, properties, PROPERTIES_NUM, &readBytes, (int*)(&temp[i].properties));
			int readNameBytes;
			glGetProgramResourceName(_programHandle, GL_UNIFORM, i, MAX_NAME_LENGTH, &readNameBytes, inputName);
			inputName[temp[i].properties.nameLength] = '\0';
			temp[i].name = inputName;
		}
	}
	return temp;
}

//==================================================================================

int ShaderProgram::_typeComponentNum(int i) const noexcept
{
	int retValue;
	switch (i) {
	case GL_FLOAT:
	case GL_INT:
	case GL_BOOL:
	case GL_UNSIGNED_INT:
	case GL_DOUBLE:
	{
		retValue = 1;
		break;
	}
	case GL_FLOAT_VEC2:
	case GL_INT_VEC2:
	case GL_UNSIGNED_INT_VEC2:
	case GL_BOOL_VEC2:
	case GL_DOUBLE_VEC2:
	{
		retValue = 2;
		break;
	}
	case GL_FLOAT_VEC3:
	case GL_INT_VEC3:
	case GL_UNSIGNED_INT_VEC3:
	case GL_BOOL_VEC3:
	case GL_DOUBLE_VEC3:
	{
		retValue = 3;
		break;
	}
	case GL_FLOAT_VEC4:
	case GL_INT_VEC4:
	case GL_UNSIGNED_INT_VEC4:
	case GL_BOOL_VEC4:
	case GL_DOUBLE_VEC4:
	case GL_FLOAT_MAT2:
	case GL_DOUBLE_MAT2:
	{
		retValue = 4;
		break;
	}
	case GL_FLOAT_MAT2x3:
	case GL_FLOAT_MAT3x2:
	case GL_DOUBLE_MAT2x3:
	case GL_DOUBLE_MAT3x2:
	{
		retValue = 6;
		break;
	}
	case GL_FLOAT_MAT2x4:
	case GL_FLOAT_MAT4x2:
	case GL_DOUBLE_MAT2x4:
	case GL_DOUBLE_MAT4x2:
	{
		retValue = 8;
		break;
	}
	case GL_FLOAT_MAT3:
	case GL_DOUBLE_MAT3:
	{
		retValue = 9;
		break;
	}
	case GL_FLOAT_MAT3x4:
	case GL_FLOAT_MAT4x3:
	case GL_DOUBLE_MAT3x4:
	case GL_DOUBLE_MAT4x3:
	{
		retValue = 12;
		break;
	}
	case GL_FLOAT_MAT4:
	case GL_DOUBLE_MAT4:
	{
		retValue = 16;
		break;
	}
	default:
	{
		retValue = -1;
		break;
	}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::_typeAtomType(int i) const noexcept
{
	int retValue;
	switch (i) {
	case GL_FLOAT:
	case GL_FLOAT_VEC2:
	case GL_FLOAT_VEC3:
	case GL_FLOAT_VEC4:
	case GL_FLOAT_MAT2:
	case GL_FLOAT_MAT2x3:
	case GL_FLOAT_MAT2x4:
	case GL_FLOAT_MAT3:
	case GL_FLOAT_MAT3x2:
	case GL_FLOAT_MAT3x4:
	case GL_FLOAT_MAT4:
	case GL_FLOAT_MAT4x2:
	case GL_FLOAT_MAT4x3:
	{
		retValue = GL_FLOAT;
		break;
	}
	case GL_INT:
	case GL_INT_VEC2:
	case GL_INT_VEC3:
	case GL_INT_VEC4:
	{
		retValue = GL_INT;
		break;
	}
	case GL_UNSIGNED_INT:
	case GL_UNSIGNED_INT_VEC2:
	case GL_UNSIGNED_INT_VEC3:
	case GL_UNSIGNED_INT_VEC4:
	{
		retValue = GL_UNSIGNED_INT;
		break;
	}
	case GL_BOOL:
	case GL_BOOL_VEC2:
	case GL_BOOL_VEC3:
	case GL_BOOL_VEC4:
	{
		retValue = GL_BOOL;
		break;
	}
	case GL_DOUBLE:
	case GL_DOUBLE_VEC2:
	case GL_DOUBLE_VEC3:
	case GL_DOUBLE_VEC4:
	case GL_DOUBLE_MAT2:
	case GL_DOUBLE_MAT2x3:
	case GL_DOUBLE_MAT2x4:
	case GL_DOUBLE_MAT3:
	case GL_DOUBLE_MAT3x2:
	case GL_DOUBLE_MAT3x4:
	case GL_DOUBLE_MAT4:
	case GL_DOUBLE_MAT4x2:
	case GL_DOUBLE_MAT4x3:
	{
		retValue = GL_DOUBLE;
		break;
	}
	default:
	{
		retValue = -1;
		break;
	}
	}
	return retValue;
}

//==================================================================================

void ShaderProgram::_moveProc(ShaderProgram& a) noexcept
{
	_deleteProgram();
	_programLinked = a._programLinked;
	_programHandle = a._programHandle;
	a._programHandle = 0;
	_shaderList = std::move(a._shaderList);
	_programInput = std::move(a._programInput);
	_programUniform = std::move(a._programUniform);
	_programUniformBlock = std::move(a._programUniformBlock);
	_programShaderStorage = std::move(a._programShaderStorage);
	_programAtomicCounter = std::move(a._programAtomicCounter);
	_programName = std::move(a._programName);
	_context = a._context;
	_compute = a._compute;
}

//==================================================================================

void ShaderProgram::_deleteProgram() noexcept
{
	if (_programHandle) {
		_context->setCurrent();
		if (_context->_activeProgram == id()) {
			_context->_activeProgram = Id<ShaderProgram>::ID_TYPE_MAX;
			_context->setActiveNoProgram();
		}
		glDeleteProgram(_programHandle);
		_programHandle = 0;
	}
}

//==================================================================================

void ShaderProgram::_eraseFromCollection(ShaderProgram* ptr)
{
	std::lock_guard<std::mutex> lk(_collectionMutex);
	if (ptr != nullptr) {
		_collection.erase(ptr->id());
		delete ptr;
	}
}

//==================================================================================

bool ShaderProgram::_emplaceToCollection(const std::shared_ptr<ShaderProgram>& ptr)
{
	std::lock_guard<std::mutex> lk(_collectionMutex);
	_collection.emplace(ptr->id(), ptr);
	return true;
}

//==================================================================================

bool ShaderProgram::appendShaderFromFile(const std::wstring& fName, int shType)
{
	using namespace std::filesystem;
	path fPath(fName);
	if (!exists(fPath) || !is_regular_file(fPath)) {
		_errorMsg = _programName + ": " + fPath.string() + std::string(" File does not exist.");
#if defined(USE_SHADER_PROGRAM_EXCEPTION)
		throw ShaderProgramException(_errorMsg);
#endif
		return false;
	}
	char* strBuffer;
#ifdef FILESYSTEMTHREAD_H_
#define FILESYSTEM_WORK_THREAD 1
#endif // FILESYSTEMTHREAD_H_
#ifdef FILESYSTEM_WORK_THREAD
	exit(-1);
#else
	const int FILE_SIZE = file_size(fPath);
	const int STR_SIZE = FILE_SIZE + 1;
	std::ifstream f(fPath, std::ios_base::in | std::ios_base::binary);
	if (!f.is_open()) {
		_errorMsg = _programName + ": " + fPath.string() + std::string(" File cannot be opened.");
#if defined(USE_SHADER_PROGRAM_EXCEPTION)
		throw ShaderProgramException(_errorMsg);
#endif
		return false;
	}
	strBuffer = new char[STR_SIZE];
	f.read(strBuffer, FILE_SIZE);
	strBuffer[f.gcount()] = '\0';
	f.close();
#endif // FILESYSTEM_WORK_THREAD
	int hashValue = 0;
	_shaderList.emplace_back(_context, strBuffer, shType, hashValue);
	delete[] strBuffer;
	if (shType == Shader::ShaderType::COMPUTING) {
		_compute = true;
	}
	return true;
}

//==================================================================================

bool ShaderProgram::appendShaderFromFile(const std::wstring& fName)
{
	using namespace std::filesystem;
	path fPath(fName);
	std::wstring fileExt = fPath.extension().wstring();
	const std::wstring shdExt[]{ L".vert",
							L".frag",
							L".geom",
							L".tesc",
							L".tese",
							L".comp",
							L".vs",
							L".fs",
							L".gs",
							L".tcs",
							L".tes",
							L".cs"
	};
	const int shdTypeEnum[]{ Shader::ShaderType::VERTEX,
						Shader::ShaderType::FRAGMENT,
						Shader::ShaderType::GEOMETRIC,
						Shader::ShaderType::TESS_CONTROL,
						Shader::ShaderType::TESS_EVALUATION,
						Shader::ShaderType::COMPUTING };
	int i = 0;
	const int SHD_EXT_SIZE = sizeof(shdExt) / sizeof(shdExt[0]);
	for (; i < SHD_EXT_SIZE; ++i) {
		if (fileExt == shdExt[i]) {
			break;
		}
	}
	if (i == 12) {
		_errorMsg = _programName + ": " + fPath.string() + std::string("\nExtention is not known.");
#if defined(USE_SHADER_PROGRAM_EXCEPTION)
		throw ShaderProgramException(_errorMsg);
#endif
		return false;
	}
	const int shaderType = shdTypeEnum[i % 6];
	return appendShaderFromFile(fName, shaderType);
}

//==================================================================================

bool ShaderProgram::appendShaderFromTxt(const char* str, int shType)
{
	if (shType == Shader::ShaderType::COMPUTING) {
		_compute = true;
	}
	int hashValue = 0;
	_shaderList.emplace_back(_context, str, shType, hashValue);
	return true;
}

//==================================================================================

bool ShaderProgram::appendShaderFromTxt(const std::string& str, int shType)
{
	if (shType == Shader::ShaderType::COMPUTING) {
		_compute = true;
	}
	int hashValue = 0;
	_shaderList.emplace_back(_context, str.c_str(), shType, hashValue);
	return true;
}

//==================================================================================

bool ShaderProgram::link()
{
	// ???????????
	if (_programLinked) {
		return false;
	}
	_context->setCurrent();
	_programHandle = glCreateProgram();
	if (!_programHandle) {
		_errorMsg = _programName + ": Program cannot be created.";
#if defined(USE_SHADER_PROGRAM_EXCEPTION)
		throw ShaderProgramException(_errorMsg);
#endif
		return false;
	}
	for (auto iter = _shaderList.cbegin(); iter != _shaderList.cend(); ++iter) {
		glAttachShader(_programHandle, iter->handle());
	}
	glLinkProgram(_programHandle);
	int status;
	glGetProgramiv(_programHandle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int logSize;
		glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &logSize);
		char* errStr = new char[(logSize + 16) & ~0xF];
		int len;
		glGetProgramInfoLog(_programHandle, logSize, &len, errStr);
		errStr[len] = '\0';
		std::string errMsg(errStr);
		delete[] errStr;
		throw ShaderProgramException(errMsg);
	}
	_programInput = std::move(ShaderProgram::_getInput());
	_programUniform = std::move(ShaderProgram::_getUniform());
	_programUniformBlock = std::move(ShaderProgram::_getUniformBlock());
	_programShaderStorage = std::move(ShaderProgram::_getShaderStorage());
	_programAtomicCounter = std::move(ShaderProgram::_getAtomicCounter());
#if defined (_USE_OPENGL_ES_)
#else
	_programSubroutineUniform = std::move(ShaderProgram::_getSubroutineUniform());
	_programSubroutine = std::move(ShaderProgram::_getSubroutine());
#endif // _USE_OPENGL_ES_
	_programTransformFeedbackVarying = std::move(ShaderProgram::_getTransformFeedbackVarying());
	_programTransformFeedbackBuffer = std::move(ShaderProgram::_getTransformFeedbackBuffer());
	_programBufferVariable = std::move(ShaderProgram::_getBufferVariable());
	_programLinked = true;
	return true;
}

//==================================================================================

int ShaderProgram::attribLocation(const std::string& attribName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programInput.size(); ++i) {
		if (attribName == _programInput[i].name) {
			retValue = _programInput[i].properties.location;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::attribType(const std::string& attribName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programInput.size(); ++i) {
		if (attribName == _programInput[i].name) {
			retValue = _programInput[i].properties.type;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::attribAtomComponentNum(const std::string& attribName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programInput.size(); ++i) {
		if (attribName == _programInput[i].name) {
			retValue = _programInput[i].properties.type;
			break;
		}
	}
	return _typeComponentNum(retValue);
}

//==================================================================================

int ShaderProgram::attribAtomType(const std::string& attribName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programInput.size(); ++i) {
		if (attribName == _programInput[i].name) {
			retValue = _programInput[i].properties.type;
			break;
		}
	}
	return _typeAtomType(retValue);
}

//==================================================================================

int ShaderProgram::uniformLocation(const std::string& uniformName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programUniform.size(); ++i) {
		if (uniformName == _programUniform[i].name) {
			retValue = _programUniform[i].properties.location;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::uniformBlockIndex(const std::string& blockName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programUniformBlock.size(); ++i) {
		if (blockName == _programUniformBlock[i].name) {
			retValue = i;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::uniformBlockBinding(const std::string& blockName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programUniformBlock.size(); ++i) {
		if (blockName == _programUniformBlock[i].name) {
			retValue = _programUniformBlock[i].properties.bufferBinding;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::shaderStorageBlockBinding(const std::string& blockName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programShaderStorage.size(); ++i) {
		if (blockName == _programShaderStorage[i].name) {
			retValue = _programShaderStorage[i].properties.bufferBinding;
			break;
		}
	}
	return retValue;
}

//==================================================================================

const char* ShaderProgram::getTypeString(int v)
{
	int index;
	static const char* str[]
	{
		"undefined type",
		"float",
		"vec2",
		"vec3",
		"vec4",
		"int",
		"ivec2",
		"ivec3",
		"ivec4",
		"bool",
		"bvec2",
		"bvec3",
		"bvec4",
		"mat2",
		"mat3",
		"mat4",
		"sampler 1D",
		"sampler 2D",
		"sampler 3D",
		"sampler cube",
		"sampler 1D shadow",
		"sampler 2D shadow",
		"mat2x3",
		"mat2x4",
		"mat3x2",
		"mat3x4",
		"mat4x2",
		"mat4x3"
	};
	switch (v)
	{
	case GL_FLOAT: {
		index = 1;
		break;
	}
	case GL_FLOAT_VEC2: {
		index = 2;
		break;
	}
	case GL_FLOAT_VEC3: {
		index = 3;
		break;
	}
	case GL_FLOAT_VEC4: {
		index = 4;
		break;
	}
	case GL_INT: {
		index = 5;
		break;
	}
	case GL_INT_VEC2: {
		index = 6;
		break;
	}
	case GL_INT_VEC3: {
		index = 7;
		break;
	}
	case GL_INT_VEC4: {
		index = 8;
		break;
	}
	case GL_BOOL: {
		index = 9;
		break;
	}
	case GL_BOOL_VEC2: {
		index = 10;
		break;
	}
	case GL_BOOL_VEC3: {
		index = 11;
		break;
	}
	case GL_BOOL_VEC4: {
		index = 12;
		break;
	}
	case GL_FLOAT_MAT2: {
		index = 13;
		break;
	}
	case GL_FLOAT_MAT3: {
		index = 14;
		break;
	}
	case GL_FLOAT_MAT4: {
		index = 15;
		break;
	}
	case GL_SAMPLER_1D: {
		index = 16;
		break;
	}
	case GL_SAMPLER_2D: {
		index = 17;
		break;
	}
	case GL_SAMPLER_3D: {
		index = 18;
		break;
	}
	case GL_SAMPLER_CUBE: {
		index = 19;
		break;
	}
	case GL_SAMPLER_1D_SHADOW: {
		index = 20;
		break;
	}
	case GL_SAMPLER_2D_SHADOW: {
		index = 21;
		break;
	}
	case GL_FLOAT_MAT2x3: {
		index = 22;
		break;
	}
	case GL_FLOAT_MAT2x4: {
		index = 23;
		break;
	}
	case GL_FLOAT_MAT3x2: {
		index = 24;
		break;
	}
	case GL_FLOAT_MAT3x4: {
		index = 25;
		break;
	}
	case GL_FLOAT_MAT4x2: {
		index = 26;
		break;
	}
	case GL_FLOAT_MAT4x3: {
		index = 27;
		break;
	}
	default: {
		index = 0;
		break;
	}
	};
	return str[index];
}

//==================================================================================

const wchar_t* ShaderProgram::getTypeWString(int v)
{
	int index;
	static const wchar_t* str[]
	{
		L"undefined type",
		L"float",
		L"vec2",
		L"vec3",
		L"vec4",
		L"int",
		L"ivec2",
		L"ivec3",
		L"ivec4",
		L"bool",
		L"bvec2",
		L"bvec3",
		L"bvec4",
		L"mat2",
		L"mat3",
		L"mat4",
		L"sampler 1D",
		L"sampler 2D",
		L"sampler 3D",
		L"sampler cube",
		L"sampler 1D shadow",
		L"sampler 2D shadow",
		L"mat2x3",
		L"mat2x4",
		L"mat3x2",
		L"mat3x4",
		L"mat4x2",
		L"mat4x3"
	};
	switch (v)
	{
	case GL_FLOAT: {
		index = 1;
		break;
	}
	case GL_FLOAT_VEC2: {
		index = 2;
		break;
	}
	case GL_FLOAT_VEC3: {
		index = 3;
		break;
	}
	case GL_FLOAT_VEC4: {
		index = 4;
		break;
	}
	case GL_INT: {
		index = 5;
		break;
	}
	case GL_INT_VEC2: {
		index = 6;
		break;
	}
	case GL_INT_VEC3: {
		index = 7;
		break;
	}
	case GL_INT_VEC4: {
		index = 8;
		break;
	}
	case GL_BOOL: {
		index = 9;
		break;
	}
	case GL_BOOL_VEC2: {
		index = 10;
		break;
	}
	case GL_BOOL_VEC3: {
		index = 11;
		break;
	}
	case GL_BOOL_VEC4: {
		index = 12;
		break;
	}
	case GL_FLOAT_MAT2: {
		index = 13;
		break;
	}
	case GL_FLOAT_MAT3: {
		index = 14;
		break;
	}
	case GL_FLOAT_MAT4: {
		index = 15;
		break;
	}
	case GL_SAMPLER_1D: {
		index = 16;
		break;
	}
	case GL_SAMPLER_2D: {
		index = 17;
		break;
	}
	case GL_SAMPLER_3D: {
		index = 18;
		break;
	}
	case GL_SAMPLER_CUBE: {
		index = 19;
		break;
	}
	case GL_SAMPLER_1D_SHADOW: {
		index = 20;
		break;
	}
	case GL_SAMPLER_2D_SHADOW: {
		index = 21;
		break;
	}
	case GL_FLOAT_MAT2x3: {
		index = 22;
		break;
	}
	case GL_FLOAT_MAT2x4: {
		index = 23;
		break;
	}
	case GL_FLOAT_MAT3x2: {
		index = 24;
		break;
	}
	case GL_FLOAT_MAT3x4: {
		index = 25;
		break;
	}
	case GL_FLOAT_MAT4x2: {
		index = 26;
		break;
	}
	case GL_FLOAT_MAT4x3: {
		index = 27;
		break;
	}
	default: {
		index = 0;
		break;
	}
	};
	return str[index];
}

//==================================================================================

int ShaderProgram::baseTypeComponentSize(int v) noexcept
{
	int size_;
	switch (v) {
	case GL_FLOAT:
	case GL_BOOL:
	case GL_INT:
	case GL_UNSIGNED_INT:
	{
		size_ = sizeof(float);
		break;
	}
	case GL_FLOAT_VEC2:
	case GL_BOOL_VEC2:
	case GL_INT_VEC2:
	case GL_UNSIGNED_INT_VEC2:
	case GL_FLOAT_MAT2:
	case GL_FLOAT_MAT3x2:
	case GL_FLOAT_MAT4x2:
	case GL_DOUBLE:
	{
		size_ = 2 * sizeof(float);
		break;
	}
	case GL_FLOAT_VEC3:
	case GL_BOOL_VEC3:
	case GL_INT_VEC3:
	case GL_UNSIGNED_INT_VEC3:
	case GL_FLOAT_MAT3:
	case GL_FLOAT_MAT2x3:
	{
		size_ = 3 * sizeof(float);
		break;
	}
	case GL_FLOAT_VEC4:
	case GL_BOOL_VEC4:
	case GL_INT_VEC4:
	case GL_UNSIGNED_INT_VEC4:
	case GL_FLOAT_MAT4:
	case GL_FLOAT_MAT2x4:
	case GL_FLOAT_MAT3x4:
	case GL_DOUBLE_VEC2:
	case GL_DOUBLE_MAT2:
	case GL_DOUBLE_MAT3x2:
	case GL_DOUBLE_MAT4x2:
	{
		size_ = 4 * sizeof(float);
		break;
	}
	case GL_DOUBLE_MAT3:
	case GL_DOUBLE_MAT2x3:
	case GL_DOUBLE_MAT4x3:
	case GL_DOUBLE_VEC3:
	{
		size_ = 3 * sizeof(double);
		break;
	}
	case GL_DOUBLE_VEC4:
	case GL_DOUBLE_MAT2x4:
	case GL_DOUBLE_MAT3x4:
	case GL_DOUBLE_MAT4:
	{
		size_ = 4 * sizeof(double);
		break;
	}
	default:
	{
		size_ = -1;
		break;
	}
	}
	return size_;
}

//==================================================================================

int ShaderProgram::baseTypeComponentNum(int v) noexcept
{
	int num_;
	switch (v) {
	case GL_FLOAT:
	case GL_BOOL:
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_DOUBLE:
	case GL_FLOAT_VEC2:
	case GL_BOOL_VEC2:
	case GL_INT_VEC2:
	case GL_UNSIGNED_INT_VEC2:
	case GL_DOUBLE_VEC2:
	case GL_FLOAT_VEC3:
	case GL_BOOL_VEC3:
	case GL_INT_VEC3:
	case GL_UNSIGNED_INT_VEC3:
	case GL_DOUBLE_VEC3:
	case GL_FLOAT_VEC4:
	case GL_BOOL_VEC4:
	case GL_INT_VEC4:
	case GL_UNSIGNED_INT_VEC4:
	case GL_DOUBLE_VEC4:
	{
		num_ = 1;
		break;
	}
	case GL_FLOAT_MAT2:
	case GL_FLOAT_MAT2x3:
	case GL_FLOAT_MAT2x4:
	case GL_DOUBLE_MAT2:
	case GL_DOUBLE_MAT2x3:
	case GL_DOUBLE_MAT2x4:
	{
		num_ = 2;
		break;
	}
	case GL_FLOAT_MAT3:
	case GL_FLOAT_MAT3x2:
	case GL_FLOAT_MAT3x4:
	case GL_DOUBLE_MAT3:
	case GL_DOUBLE_MAT3x2:
	case GL_DOUBLE_MAT3x4:
	{
		num_ = 3;
		break;
	}
	case GL_FLOAT_MAT4:
	case GL_FLOAT_MAT4x2:
	case GL_FLOAT_MAT4x3:
	case GL_DOUBLE_MAT4:
	case GL_DOUBLE_MAT4x2:
	case GL_DOUBLE_MAT4x3:
	{
		num_ = 4;
		break;
	}
	default:
	{
		num_ = -1;
		break;
	}
	}
	return num_;
}

//==================================================================================

#if defined (_USE_OPENGL_ES_)
#else
unsigned int ShaderProgram::subroutineIndex(const std::string& subroutineName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programSubroutine.size(); ++i) {
		if (subroutineName == _programSubroutine[i].name) {
			retValue = _programSubroutine[i].index;
			break;
		}
	}
	return retValue;
}

//==================================================================================

int ShaderProgram::subroutineShaderType(const std::string& subroutineName) const noexcept
{
	int retValue = -1;
	if (!_programHandle) {
		return retValue;
	}
	for (int i = 0; i < _programSubroutine.size(); ++i) {
		if (subroutineName == _programSubroutine[i].name) {
			retValue = _programSubroutine[i].shaderType;
			break;
		}
	}
	return retValue;
}

//==================================================================================

std::shared_ptr<ShaderProgram> ShaderProgram::getFromCollection(Id<ShaderProgram>::idType n)
{
	std::shared_ptr<ShaderProgram> retValue;
	std::lock_guard<std::mutex> lk(_collectionMutex);
	auto iter = _collection.find(n);
	if (iter != _collection.end()) {
		retValue = iter->second.lock();
	}
	return retValue;
}
#endif // _USE_OPENGL_ES_

//==================================================================================

ShaderProgram::~ShaderProgram() noexcept
{
	_context->setActiveNoProgram();
	_deleteProgram();
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif