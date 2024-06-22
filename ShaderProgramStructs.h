#pragma once
#ifndef SHADERPROGRAMSTRUCTS_H_
#define SHADERPROGRAMSTRUCTS_H_

#include <string>
#include <vector>

//--------------------------------------------------------
struct InputStruct
{
	int nameLength;
	int type;
	int arraySize;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
	int location;
	int isPerPatch;
	int locationComponent;
};
struct UniformStruct
{
	int nameLength;
	int type;
	int arraySize;
	int offset;
	int blockIndex;
	int arrayStride;
	int matrixStide;
	int isRowMajor;
	int atomicCounterBufferIndex;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
	int location;
};
struct UniformBlockStruct
{
	int nameLength;
	int bufferBinding;
	int bufferDataSize;
	int numActiveVariables;
	int activeVariables;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
};
struct OutputStruct
{
	int nameLength;
	int type;
	int arraySize;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
	int location;
	int locationIndex;
	int isPerPatch;
	int locationComponent;
};
struct ShaderStorageStruct
{
	int nameLength;
	int bufferBinding;
	int bufferDataSize;
	int numActiveVariables;
	int activeVariables;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
};
struct AtomicCounterStruct
{
	int bufferBinding;
	int bufferDataSize;
	int numActiveVariables;
	int activeVariables;
};
struct SubroutineStruct
{
	int nameLength;
	int arraySize;
	int numCompatibleSubroutines;
	int compatibleSubroutines;
	int location;
};
struct TransformFeedbackBufferStruct
{
	int bufferBinding;
	int numActiveVariables;
	int activeVariables;
	int transformFeedbackBufferStride;
};
struct TransformFeedbackVaryingStruct
{
	int nameLength;
	int type;
	int arraySize;
	int offset;
	int transformFeedbackBufferIndex;
};
struct BufferVariableStruct
{
	int nameLength;
	int type;
	int arraySize;
	int offset;
	int blockIndex;
	int arrayStride;
	int matrixStide;
	int isRowMajor;
	int vertex;
	int tessControl;
	int tessEvaluation;
	int geometry;
	int fragment;
	int compute;
	int topLevelArraySize;
	int topLevelArrayStride;
};
//--------------------------------------------------------
struct ProgramInput
{
	InputStruct properties;
	std::string name;
};
struct ProgramUniform
{
	UniformStruct properties;
	int size;
	bool blockMember;
	std::string blockName;
	std::string name;
};
struct UniformBlockField
{
	std::string name;
	int offset;
	int componentSize;
	int componentNum;
	int arraySize;
	int arrayStride;
	int matrixStride;
	int type;
	int size;
	bool isStruct;
	UniformBlockField* innerStruct;
};
class ProgramUniformBlock
{
public:
	UniformBlockStruct properties;
	std::string name;
	std::vector<UniformBlockField> fields;
};
struct ProgramShaderStorageBlock
{
	ShaderStorageStruct properties;
	std::string name;
};
struct ProgramAtomicCounter
{
	AtomicCounterStruct properties;
	int offset;
	std::string name;
};
struct ProgramSubroutineUniform
{
	SubroutineStruct properties;
	unsigned int shaderType;
	std::string name;
	std::string subroutineName;
};
struct ProgramSubroutine
{
	std::string name;
	unsigned int shaderType;
	int index;
};
struct ProgramTransformFeedbackBuffer
{
	TransformFeedbackBufferStruct properties;
};
struct ProgramTransformFeedbackVarying
{
	std::string name;
	TransformFeedbackVaryingStruct properties;
};
struct ProgramBufferVariable
{
	std::string name;
	BufferVariableStruct properties;
};
//--------------------------------------------------------

#endif // SHADERPROGRAMSTRUCTS_H_
