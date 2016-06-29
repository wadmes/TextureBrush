#include "shaderManager.h"

#include "../commonUtility.h"

using namespace TextureSynthesis;

CShaderManager::CShaderManager()
{
}

CShaderManager* CShaderManager::getShaderManagerInstance()
{
	static CShaderManager sysShaderManager;
	return &sysShaderManager;
}

CShaderManager::~CShaderManager()
{
	releaseShaders();

	std::cout<<"Release: Shader manager released!"<<std::endl;
}

GLuint CShaderManager::getShaderByName(ShaderType vST, const char* shaderName)
{
	std::map<std::string, GLuint>::iterator itr = m_shaders.find(shaderName);
	if (itr == m_shaders.end())
	{
		loadShader(vST, shaderName);
	}

	return m_shaders[shaderName];
}

void CShaderManager::loadShader(ShaderType vST, const char* shaderFile)
{
	GLuint shaderHandle;

	int len;
	char* shaderBuf = CCommonUtility::loadFile(shaderFile, len);

	shaderHandle = glCreateShader(vST);

	const GLchar * glFilePtr = shaderBuf;
	glShaderSource(shaderHandle, 1, &glFilePtr, &len);

	GLint compiled;
	glCompileShader(shaderHandle);
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		std::cout<<"ERROR: Shader fails compilation!"<<std::endl;
		printShaderInfoLog(shaderHandle);
		exit(-1);
	}

	std::cout<<"SUCCESS: New shader generated(shader handle = "<<shaderHandle<<")"<<std::endl;
	m_shaders[shaderFile] = shaderHandle;

	free(shaderBuf);
}

void CShaderManager::releaseShaders()
{
	for (std::map<std::string, GLuint>::const_iterator cItr = m_shaders.begin(); cItr != m_shaders.end(); ++cItr)
	{
		deleteShader((*cItr).second);
	}
}

void CShaderManager::deleteShader(GLuint shaderHandle)
{
	if(glIsShader(shaderHandle) == GL_TRUE)
		glDeleteShader(shaderHandle);
}

void CShaderManager::printShaderInfoLog(const GLuint& shader)
{
	int infoLogLen(0);
	int charsWritten(0);
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if(infoLogLen > 1)
	{
		infoLog = new GLchar[infoLogLen];
		glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
		std::cout << "Shader InfoLog: " << std::endl << infoLog<<std::endl;
		delete[] infoLog;
	}
}