#pragma once

namespace TextureSynthesis
{

class CVertexBufferObject;
class CShaderProgram;

class CRenderPass
{
public:
	CRenderPass();
	virtual ~CRenderPass();

private:
	CVertexBufferObject* m_pScene;
	CShaderProgram* m_pShaderProgram;
};

}