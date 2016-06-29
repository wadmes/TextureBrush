#include "cubeGeometry.h"

using namespace TextureSynthesis;

CCubeGeometry::CCubeGeometry()
{
	initGeometry();
}

CCubeGeometry::~CCubeGeometry()
{

}

void CCubeGeometry::initGeometry()
{
	m_numVers = 24;
	m_numTris = 12;

	m_v = new vec3[m_numVers];
	m_n = new vec3[m_numVers];
	m_i = new ivec3[m_numTris];

	int curVerIdx = 0;
	int curFacetIdx = 0;

	m_v[curVerIdx] = vec3(-1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 1.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 1.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 1.0f, 0.0f);
	m_v[curVerIdx] = vec3(-1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 1.0f, 0.0f);

	m_i[curFacetIdx++] = ivec3(0, 1, 2);
	m_i[curFacetIdx++] = ivec3(0, 2, 3);


	m_v[curVerIdx] = vec3(-1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, -1.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, -1.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, -1.0f, 0.0f);
	m_v[curVerIdx] = vec3(-1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, -1.0f, 0.0f);

	m_i[curFacetIdx++] = ivec3(4, 6, 5);
	m_i[curFacetIdx++] = ivec3(4, 7, 6);

	m_v[curVerIdx] = vec3(-1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(-1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(-1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(-1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(1.0f, 0.0f, 0.0f);

	m_i[curFacetIdx++] = ivec3(8, 9, 10);
	m_i[curFacetIdx++] = ivec3(8, 10, 11);

	m_v[curVerIdx] = vec3(1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(-1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(-1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(-1.0f, 0.0f, 0.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(-1.0f, 0.0f, 0.0f);

	m_i[curFacetIdx++] = ivec3(12, 14, 13);
	m_i[curFacetIdx++] = ivec3(12, 15, 14);

	m_v[curVerIdx] = vec3(1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, 1.0f);
	m_v[curVerIdx] = vec3(-1.0f, 1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, 1.0f);
	m_v[curVerIdx] = vec3(-1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, 1.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, 1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, 1.0f);

	m_i[curFacetIdx++] = ivec3(16, 17, 18);
	m_i[curFacetIdx++] = ivec3(16, 18, 19);

	m_v[curVerIdx] = vec3(1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, -1.0f);
	m_v[curVerIdx] = vec3(-1.0f, 1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, -1.0f);
	m_v[curVerIdx] = vec3(-1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, -1.0f);
	m_v[curVerIdx] = vec3(1.0f, -1.0f, -1.0f);
	m_n[curVerIdx++] = vec3(0.0f, 0.0f, -1.0f);

	m_i[curFacetIdx++] = ivec3(20, 23, 21);
	m_i[curFacetIdx++] = ivec3(21, 23, 22);
}