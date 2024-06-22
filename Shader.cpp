#include "Shader.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4267)
#endif

void Shader::_createShader(const char* str, int len)
{
	switch (_type) {
	case ShaderType::VERTEX:
	{
		_openGLType = GL_VERTEX_SHADER;
		break;
	}
	case ShaderType::TESS_CONTROL:
	{
		_openGLType = GL_TESS_CONTROL_SHADER;
		break;
	}
	case ShaderType::TESS_EVALUATION:
	{
		_openGLType = GL_TESS_EVALUATION_SHADER;
		break;
	}
	case ShaderType::GEOMETRIC:
	{
		_openGLType = GL_GEOMETRY_SHADER;
		break;
	}
	case ShaderType::FRAGMENT:
	{
		_openGLType = GL_FRAGMENT_SHADER;
		break;
	}
	case ShaderType::COMPUTING:
	{
		_openGLType = GL_COMPUTE_SHADER;
		break;
	}
	}
	_context->setCurrent();
	_handle = glCreateShader(_openGLType);
	if (!_handle) {
		_errorMessage = "Shader cannot be created.";
#if defined (USE_SHADER_EXCEPTION)
		throw ShaderProgramException(_errorMessage);
#endif
		return;
	}
	glShaderSource(_handle, 1, &str, &len);
	glCompileShader(_handle);
	int status;
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int errStrLen = 0;
		glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &errStrLen);
		char* errStr = new char[(errStrLen + 16) & ~0xF];
		int len;
		glGetShaderInfoLog(_handle, errStrLen, &len, errStr);
		errStr[len] = '\0';
		_errorMessage = errStr;
		delete[] errStr;
#if defined (USE_SHADER_EXCEPTION)
		throw ShaderProgramException(_errorMessage);
#endif
	}
}

//==================================================================================

void Shader::_moveProc(Shader& a) noexcept
{
	_deleteShader();
	_hash = a._hash;
	_handle = a._handle;
	a._handle = 0;
	_type = a._type;
	_openGLType = a._openGLType;
	_id = std::move(a._id);
	_context = a._context;
}

//==================================================================================

void Shader::_deleteShader() noexcept
{
	if (_handle) {
		_context->setCurrent();
		glDeleteShader(_handle);
		_handle = 0;
	}
}

//==================================================================================

Shader::Shader(GraphicContext::Ptr context, const char* s, int t, size_t h) :
	_handle(0),
	_hash(h),
	_type(t),
	_errorMessage(""),
	_context(context)
{
	if (t < ShaderType::VERTEX || t > ShaderType::COMPUTING) {
		_errorMessage = "Shader type is unknown.";
#if defined (USE_SHADER_EXCEPTION)
		throw ShaderProgramException(_errorMessage);
#endif
		return;
	}
	if (s == nullptr) {
		_errorMessage = "Shader ../source code is empty.";
#if defined (USE_SHADER_EXCEPTION)
		throw ShaderProgramException(_errorMessage);
#endif
		return;
	}
	const int strLen = strlen(s);
	_createShader(s, strLen);
}

//==================================================================================

Shader::Shader(GraphicContext::Ptr constext, const std::string& s, int t, size_t h) :
	_handle(0),
	_hash(h),
	_type(t),
	_context(constext)
{
	if (t < ShaderType::VERTEX || t > ShaderType::COMPUTING) {
		_errorMessage = "Shader type is unknown.";
#if defined (USE_SHADER_EXCEPTION)
		throw ShaderProgramException(_errorMessage);
#endif
		return;
	}
	if (s.empty()) {
		throw ShaderProgramException("Shader cannot be created.");
	}
	_createShader(s.c_str(), s.length());
}

//==================================================================================

Shader::~Shader()
{
	_deleteShader();
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
