#pragma once

#include "../preHeader.h"
#include "shaderManager.h"

namespace TextureSynthesis
{

enum ShaderUniformParaType
{
	SUPT_INT,
	SUPT_FLOAT_1,
	SUPT_FLOAT_3,
	SUPT_FLOAT_4,
	SUPT_FLOAT_16,
};

struct UniformVariablei
{
	std::string varName;

	GLint location;

	int* pValues;

	int chNum;

	int elemCount;
};

struct UniformVariableui
{
	std::string varName;

	GLint location;

	unsigned int* pValues;

	int chNum;

	int elemCount;
};

struct UniformVariablef
{
	std::string varName;

	GLint location;

	float* pValues;

	int chNum;

	int elemCount;
};

struct UniformVariableMat
{
	std::string varName;

	GLint location;

	float* pValues;

	int rowNum;
	int colNum;

	int elemCount;

	bool transpose;
};

struct UniformVariableSampler
{
	std::string varName;

	GLint location;

	GLuint* texHandle;

	int dimNum;

	int bundlePoint;

	GLuint varLocation;
};

class CShaderProgram
{
public:
	CShaderProgram();
	virtual ~CShaderProgram();

	void attachShader(CShaderManager::ShaderType vST, const char* shaderFile);
	void linkProgram();

	void activate();
	void deactivate();

	void updateUniformVariables();

// General uniform variable register function
	void registerUniformiv(const std::string& varName, int* pValues, int chNum, int eleCount = 1, bool updatePerFrame = true);
	void registerUniformuiv(const std::string& varName, unsigned int* pValues, int chNum, int eleCount = 1, bool updatePerFrame = true);
	void registerUniformfv(const std::string& varName, float* pValues, int chNum, int eleCount = 1, bool updatePerFrame = true);
	void registerUniformMatrix(const std::string& varName, float* pValues, int rowNum, int colNum, int eleCount = 1, bool transpose = false, bool updatePerFrame = true);
	void registerUniformSampler(const std::string& varName, GLuint* pTexHandle, int dimNum, int bundlePoint);

// Specific uniform variable register function
	void registerUniformModelVivwMat(const std::string& varName);
	void registerUniformProjMat(const std::string& varName);

// General registered uniform variable update functions
	void updateUniformiv(const UniformVariablei& vVariable);
	void updateUniformuiv(const UniformVariableui& vVariable);
	void updateUniformfv(const UniformVariablef& vVariable);
	void updateUniformMatrix(const UniformVariableMat& vVariable);
	void updateUniformSampler(const UniformVariableSampler& vVariable);

// General unregistered uniform variable update functions
	void updateUniformiv(const std::string& varName, int* pValues, int chNum, int eleCount = 1);
	void updateUniformuiv(const std::string& varName, unsigned int* pValues, int chNum, int eleCount = 1);
	void updateUniformfv(const std::string& varName, float* pValues, int chNum, int eleCount = 1);
	void updateUniformMatrix(const std::string& varName, float* pValues, int rowNum, int colNum, int eleCount = 1, bool transpose = false);
	void updateUniformSampler(const std::string& varName, GLuint* pTexHandle, int dimNum, int bundlePoint);
	
// Specific uniform variable update Function
	void updateModelViewMat();
	void updateProjMat();
	void updateModelViewMat(const std::string& varName);
	void updateProjMat(const std::string& varName);

	void updateMatrix3x3(const std::string& varName, float *pMat);
	void updateMatrix4x4(const std::string& varName, float *pMat);

	void updateTexture2D(const std::string& uniformName, GLuint glTexHandle, int bundlePoint);
	void updateTexture3D(const std::string& uniformName, GLuint glTexHandle);
	void registerUniformParameter();

private:
	void detachShaders();
	void printLinkInfoLog(const GLuint& program);

private:
	bool m_linked;
	GLuint m_programHandle;
	GLuint m_shaders[CShaderManager::ST_TOTALTYPENUM];

	UniformVariableMat m_modelViewMatUniform;
	UniformVariableMat m_projMatUniform;

	std::vector<UniformVariablei> m_iUniformVec;
	std::vector<UniformVariableui> m_uiUniformVec;
	std::vector<UniformVariablef> m_fUniformVec;
	std::vector<UniformVariableMat> m_matUniformVec;
	std::vector<UniformVariableSampler> m_texUniformVec;
};

}  // end namespace