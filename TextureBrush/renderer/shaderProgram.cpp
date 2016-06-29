#include "shaderProgram.h"

using namespace TextureSynthesis;

//////////////////////////////////////////////////////////////////////////
// Implementation of class ShaderProgram
//////////////////////////////////////////////////////////////////////////
CShaderProgram::CShaderProgram() : m_linked(false)
{
	m_programHandle = glCreateProgram();
	if(glIsProgram(m_programHandle) == GL_TRUE)
	{
		std::cout << "SUCCESS: New program generated(program handle = " << m_programHandle << ")" << std::endl;
	}
}

CShaderProgram::~CShaderProgram()
{
	detachShaders();
	if(glIsProgram(m_programHandle) == GL_TRUE)
		glDeleteProgram(m_programHandle);
}

void CShaderProgram::attachShader(CShaderManager::ShaderType vST, const char* shaderFile)
{
	if(glIsShader(m_shaders[vST]) == GL_TRUE)
	{
		std::cout<<"WARNING: Shader handle has been attached to this type shader"<<std::endl;
		return;
	}

	m_shaders[vST] = CShaderManager::getShaderManagerInstance()->getShaderByName(vST, shaderFile);
	glAttachShader(m_programHandle, m_shaders[vST]);
}

void CShaderProgram::detachShaders()
{
	for (int idx = 0; idx < CShaderManager::ST_TOTALTYPENUM; ++idx)
	{
		if(glIsShader(m_shaders[idx]) == GL_TRUE)
		{
			glDetachShader(m_programHandle, m_shaders[idx]);
		}
	}
}

void CShaderProgram::linkProgram()
{
	glLinkProgram(m_programHandle);

	GLint linked;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		std::cout<<"ERROR: Shader program fails linking!"<<std::endl;
		printLinkInfoLog(m_programHandle);
	}
	else
	{
		m_linked = true;
	}
}

void CShaderProgram::printLinkInfoLog(const GLuint& program)
{
	int infoLogLen(0);
	int charsWritten(0);
	GLchar *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

	if(infoLogLen > 1)
	{
		infoLog = new GLchar[infoLogLen];
		glGetProgramInfoLog(program, infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog: " << std::endl << infoLog<<std::endl;
		delete[] infoLog;
	}
}

void CShaderProgram::activate()
{
	GLint currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if(currentProg != (GLint)m_programHandle)
		glUseProgram(m_programHandle);

	//updateUniformVariables();
}

void CShaderProgram::deactivate()
{
	GLint currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if(currentProg == (GLint)m_programHandle)
		glUseProgram(0);
}

void CShaderProgram::updateUniformVariables()
{
	// Update model view matrix if needed
	if (m_modelViewMatUniform.varName != "")
	{
		updateModelViewMat();
	}

	// Update projective matrix if needed
	if (m_projMatUniform.varName != "")
	{
		updateProjMat();
	}

	// Update other uniform variables
	for (int idx = 0; idx < m_iUniformVec.size(); ++idx)
	{
		updateUniformiv(m_iUniformVec[idx]);
	}

	for (int idx = 0; idx < m_fUniformVec.size(); ++idx)
	{
		updateUniformfv(m_fUniformVec[idx]);
	}

	for (int idx = 0; idx < m_matUniformVec.size(); ++idx)
	{
		updateUniformMatrix(m_matUniformVec[idx]);
	}

// 	GLuint *pHandleArray = new GLuint[m_texUniformVec.size()];
// 	for (int idx = 0; idx < m_texUniformVec.size(); ++idx)
// 	{
// 		updateUniformSampler(m_texUniformVec[idx]);
// 		pHandleArray[idx] = *m_texUniformVec[idx].texHandle;
// 	}
}

void CShaderProgram::registerUniformiv(const std::string& varName, int* pValues, int chNum, int eleCount, bool updatePerFrame)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS: New int uniform variable registered with location (" << varLoc << ")" << std::endl;
	}

	UniformVariablei newUniformVar;

	newUniformVar.varName = varName;
	newUniformVar.location = varLoc;
	newUniformVar.pValues = pValues;
	newUniformVar.chNum = chNum;
	newUniformVar.elemCount = eleCount;

	m_iUniformVec.push_back(newUniformVar);
}

void CShaderProgram::registerUniformuiv(const std::string& varName, unsigned int* pValues, int chNum, int eleCount, bool updatePerFrame)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS: New uint uniform variable registered with location (" << varLoc << ")" << std::endl;
	}

	UniformVariableui newUniformVar;

	newUniformVar.varName = varName;
	newUniformVar.location = varLoc;
	newUniformVar.pValues = pValues;
	newUniformVar.chNum = chNum;
	newUniformVar.elemCount = eleCount;

	m_uiUniformVec.push_back(newUniformVar);
}

void CShaderProgram::registerUniformfv(const std::string& varName, float* pValues, int chNum, int eleCount, bool updatePerFrame)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS: New float uniform variable registered with location (" << varLoc << ")" << std::endl;
	}

	UniformVariablef newUniformVar;

	newUniformVar.varName = varName;
	newUniformVar.location = varLoc;
	newUniformVar.pValues = pValues;
	newUniformVar.chNum = chNum;
	newUniformVar.elemCount = eleCount;

	m_fUniformVec.push_back(newUniformVar);
}

void CShaderProgram::registerUniformMatrix(const std::string& varName, float* pValues, int rowNum, int colNum, int eleCount, bool transpose, bool updatePerFrame)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS: New matrix uniform variable registered with location (" << varLoc << ")" << std::endl;
	}

	UniformVariableMat newMatUniformVar;

	newMatUniformVar.varName = varName;
	newMatUniformVar.location = varLoc;
	newMatUniformVar.pValues = pValues;
	newMatUniformVar.rowNum = rowNum;
	newMatUniformVar.colNum = colNum;
	newMatUniformVar.transpose = transpose;
	newMatUniformVar.elemCount = eleCount;

	m_matUniformVec.push_back(newMatUniformVar);
}

void CShaderProgram::registerUniformSampler(const std::string& varName, GLuint* pTexHandle, int dimNum, int bundlePoint)
{
	assert(pTexHandle != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS: New sampler uniform variable registered with location (" << varLoc << ")" << std::endl;
	}

	UniformVariableSampler newSamplerUniformVar;

	newSamplerUniformVar.varName = varName;
	newSamplerUniformVar.location = varLoc;
	newSamplerUniformVar.texHandle = pTexHandle;
	newSamplerUniformVar.dimNum = dimNum;
	newSamplerUniformVar.bundlePoint = bundlePoint;
}

void CShaderProgram::registerUniformModelVivwMat(const std::string& varName)
{
	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Model view Uniform variable " << varName << " not found!" << std::endl;
	}

	m_modelViewMatUniform.varName = varName;
	m_modelViewMatUniform.location = varLoc;
	m_modelViewMatUniform.rowNum = 4;
	m_modelViewMatUniform.colNum = 4;
	m_modelViewMatUniform.elemCount = 1;
	m_modelViewMatUniform.transpose = false;
}

void CShaderProgram::registerUniformProjMat(const std::string& varName)
{
	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Projective Uniform variable " << varName << " not found!" << std::endl;
	}

	m_projMatUniform.varName = varName;
	m_projMatUniform.location = varLoc;
	m_projMatUniform.rowNum = 4;
	m_projMatUniform.colNum = 4;
	m_projMatUniform.elemCount = 1;
	m_projMatUniform.transpose = false;
}

void CShaderProgram::updateUniformiv(const UniformVariablei& vVariable)
{
	switch (vVariable.chNum)
	{
	case 1:
		glUniform1iv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 2:
		glUniform2iv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 3:
		glUniform3iv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 4:
		glUniform4iv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	}
}

void CShaderProgram::updateUniformuiv(const UniformVariableui& vVariable)
{
	switch (vVariable.chNum)
	{
	case 1:
		glUniform1uiv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 2:
		glUniform2uiv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 3:
		glUniform3uiv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 4:
		glUniform4uiv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	}
}

void CShaderProgram::updateUniformfv(const UniformVariablef& vVariable)
{
	switch (vVariable.chNum)
	{
	case 1:
		glUniform1fv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 2:
		glUniform2fv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 3:
		glUniform3fv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	case 4:
		glUniform4fv(vVariable.location, vVariable.elemCount, vVariable.pValues);
		break;
	}
}

void CShaderProgram::updateUniformMatrix(const UniformVariableMat& vVariable)
{
	switch (vVariable.rowNum)
	{
	case 2:
		if (vVariable.colNum != 2)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix2fv(vVariable.location, vVariable.elemCount, vVariable.transpose, vVariable.pValues);
		}
		break;
	case 3:
		if (vVariable.colNum != 3)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix3fv(vVariable.location, vVariable.elemCount, vVariable.transpose, vVariable.pValues);
		}
		break;
	case 4:
		if (vVariable.colNum != 4)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix4fv(vVariable.location, vVariable.elemCount, vVariable.transpose, vVariable.pValues);
		}
		break;
	}
}

void CShaderProgram::updateUniformSampler(const UniformVariableSampler& vVariable)
{
	/*switch (vVariable.dimNum)
	{
	case 1:
		glActiveTexture(GL_TEXTURE0 + vVariable.bundlePoint);
		glBindTexture(GL_TEXTURE_1D, *vVariable.texHandle);
		glUniform1i(vVariable.varLocation, vVariable.bundlePoint);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE0 + vVariable.bundlePoint);
		glBindTexture(GL_TEXTURE_2D, *vVariable.texHandle);
		glUniform1i(vVariable.varLocation, vVariable.bundlePoint);
		break;
	case 3:
		glActiveTexture(GL_TEXTURE0 + vVariable.bundlePoint);
		glBindTexture(GL_TEXTURE_3D, *vVariable.texHandle);
		glUniform1i(vVariable.varLocation, vVariable.bundlePoint);
		break;
	}*/
}

void CShaderProgram::updateUniformiv(const std::string& varName, int* pValues, int chNum, int eleCount)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}

	switch (chNum)
	{
	case 1:
		glUniform1iv(varLoc, eleCount, pValues);
		break;
	case 2:
		glUniform2iv(varLoc, eleCount, pValues);
		break;
	case 3:
		glUniform3iv(varLoc, eleCount, pValues);
		break;
	case 4:
		glUniform4iv(varLoc, eleCount, pValues);
		break;
	}
}

void CShaderProgram::updateUniformuiv(const std::string& varName, unsigned int* pValues, int chNum, int eleCount)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}

	switch (chNum)
	{
	case 1:
		glUniform1uiv(varLoc, eleCount, pValues);
		break;
	case 2:
		glUniform2uiv(varLoc, eleCount, pValues);
		break;
	case 3:
		glUniform3uiv(varLoc, eleCount, pValues);
		break;
	case 4:
		glUniform4uiv(varLoc, eleCount, pValues);
		break;
	}
}

void CShaderProgram::updateUniformfv(const std::string& varName, float* pValues, int chNum, int eleCount)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}

	switch (chNum)
	{
	case 1:
		glUniform1fv(varLoc, eleCount, pValues);
		break;
	case 2:
		glUniform2fv(varLoc, eleCount, pValues);
		break;
	case 3:
		glUniform3fv(varLoc, eleCount, pValues);
		break;
	case 4:
		glUniform4fv(varLoc, eleCount, pValues);
		break;
	}
}

void CShaderProgram::updateUniformMatrix(const std::string& varName, float* pValues, int rowNum, int colNum, int eleCount, bool transpose)
{
	assert(pValues != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}

	switch (rowNum)
	{
	case 2:
		if (colNum != 2)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix2fv(varLoc, eleCount, transpose, pValues);
		}
		break;
	case 3:
		if (colNum != 3)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix3fv(varLoc, eleCount, transpose, pValues);
		}
		break;
	case 4:
		if (colNum != 4)
		{
			std::cout << "ERROR: Unsupported matrix type" << std::endl;
			exit(-1);
		}
		else
		{
			glUniformMatrix4fv(varLoc, eleCount, transpose, pValues);
		}
		break;
	}
}

void CShaderProgram::updateUniformSampler(const std::string& varName, GLuint* pTexHandle, int dimNum, int bundlePoint)
{
	assert(pTexHandle != NULL);

	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Uniform variable " << varName << " not found!" << std::endl;
	}
}

void CShaderProgram::updateMatrix3x3(const std::string& varName, float *pMat)
{
	GLint mvLoc = glGetUniformLocation(m_programHandle, varName.c_str());
	
	glUniformMatrix3fv(mvLoc, 1, GL_FALSE, pMat);
}

void CShaderProgram::updateMatrix4x4(const std::string& varName, float *pMat)
{
	GLint mvLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, pMat);
}

void CShaderProgram::updateModelViewMat()
{
	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);

	glUniformMatrix4fv(m_modelViewMatUniform.location, 1, GL_FALSE, mv);
}

void CShaderProgram::updateProjMat()
{
	GLfloat proj[16];
	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	glUniformMatrix4fv(m_projMatUniform.location, 1, GL_FALSE, proj);
}

void CShaderProgram::updateModelViewMat(const std::string& varName)
{
	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Model view Uniform variable " << varName << " not found!" << std::endl;
	}

	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	glUniformMatrix4fv(varLoc, 1, GL_FALSE, mv);
}

void CShaderProgram::updateProjMat(const std::string& varName)
{
	GLint varLoc = glGetUniformLocation(m_programHandle, varName.c_str());

	if (varLoc < 0)
	{
		std::cout << "WARNING: Projective Uniform variable " << varName << " not found!" << std::endl;
	}

	GLfloat proj[16];
	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	glUniformMatrix4fv(varLoc, 1, GL_FALSE, proj);
}

void CShaderProgram::updateTexture2D(const std::string& uniformName, GLuint glTexHandle, int bundlePoint)
{
	GLint mvLoc = glGetUniformLocation(m_programHandle, uniformName.c_str());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glTexHandle);

	glUniform1i(mvLoc, 0);
}

void CShaderProgram::updateTexture3D(const std::string& uniformName, GLuint glTexHandle)
{
	GLint mvLoc = glGetUniformLocation(m_programHandle, uniformName.c_str());

	//glActiveTexture(GL_TEXTURE0 + 0);
	//glBindTexture(GL_TEXTURE_3D, glTexHandle);

	glUniform1i(mvLoc, 0);
}