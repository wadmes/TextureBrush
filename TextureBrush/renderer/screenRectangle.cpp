#include "screenRectangle.h"

using namespace TextureSynthesis;

CScreenRectangle* CScreenRectangle::getInstance()
{
	CScreenRectangle* s_pRectangle = NULL;

	if (s_pRectangle == NULL)
	{
		s_pRectangle = new CScreenRectangle();
	}

	return s_pRectangle;
}

CScreenRectangle::~CScreenRectangle()
{

}

CScreenRectangle::CScreenRectangle()
{
	initData();
}

void CScreenRectangle::initData()
{
	m_numVers = 4;
	m_numTris = 2;

	m_v = new vec3[4];
	m_i = new ivec3[2];
	m_texCoords = new vec3[4];

	m_v[0] = vec3(-1.0f, -1.0f, 0.0f);
	m_v[1] = vec3(1.0f, -1.0f, 0.0f);
	m_v[2] = vec3(1.0f, 1.0f, 0.0f);
	m_v[3] = vec3(-1.0f, 1.0f, 0.0f);

	m_texCoords[0] = vec3(0.0f, 0.0f, 0.0f);
	m_texCoords[1] = vec3(1.0f, 0.0f, 0.0f);
	m_texCoords[2] = vec3(1.0f, 1.0f, 0.0f);
	m_texCoords[3] = vec3(0.0f, 1.0f, 0.0f);

	m_i[0] = ivec3(0, 1, 2);
	m_i[1] = ivec3(0, 2, 3);
}