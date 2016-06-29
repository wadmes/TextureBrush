#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

class CShaderManager
{
public:
	enum ShaderType
	{
		ST_VERTEX = GL_VERTEX_SHADER,
		ST_FRAGMENT = GL_FRAGMENT_SHADER,
		ST_GEOMETRY,
		ST_COMPUTE,
		ST_TOTALTYPENUM
	};

public:
	static CShaderManager* getShaderManagerInstance();
	virtual ~CShaderManager();

	GLuint getShaderByName(ShaderType vST, const char* shaderName);

private:
	CShaderManager();

	void loadShader(ShaderType vST, const char* shaderFile);
	void releaseShaders();
	void deleteShader(GLuint shaderHandle);

	void printShaderInfoLog(const GLuint& shader);

private:
	std::map<std::string, GLuint> m_shaders;
};

}  // End namespace