#include "screenPassVBO.h"
#include "screenRectangle.h"

using namespace TextureSynthesis;

CScreenPassVBO::CScreenPassVBO() : CVertexBufferObject(CScreenRectangle::getInstance(), VBOBM_SCREENRECTANGLE)
{
}

CScreenPassVBO::~CScreenPassVBO()
{

}

void CScreenPassVBO::display()
{
	glBindVertexArray(m_VAO);

	glDrawElementsBaseVertex(GL_TRIANGLES, m_triNum * 3, GL_UNSIGNED_INT, (void*)0, 0);

	glBindVertexArray(0);
}