#include "sphereGeometry.h"

using namespace TextureSynthesis;

CSphereGeometry::CSphereGeometry(int vLongReso, int vLatiReso) :m_longReso(vLongReso), m_latiReso(vLatiReso)
{
	initGeometry();
}

CSphereGeometry::~CSphereGeometry()
{

}

void CSphereGeometry::initGeometry()
{
	float latiInterval = MY_PI / m_latiReso;
	float longInterval = MY_2_PI / m_longReso;

	int totalVertexNum = 2 + (m_latiReso - 1) * m_longReso;

	m_numVers = totalVertexNum;

	m_v = new vec3[m_numVers];
	m_n = new vec3[m_numVers];

	int curVerIdx = 0, curTriIdx = 0;
	
	m_v[curVerIdx] = vec3(0.0f, -1.0f, 0.0f);
	m_n[curVerIdx++] = vec3(0.0f, -1.0f, 0.0f);

	for (int latiIdx = 1; latiIdx < m_latiReso; ++latiIdx)
	{
		float curLatiAngle = latiIdx * latiInterval - MY_HALF_PI;

		for (int longIdx = 0; longIdx < m_longReso; ++longIdx)
		{
			float curLongAngle = longIdx * longInterval;
			
			m_v[curVerIdx] = vec3(cos(curLatiAngle) * cos(curLongAngle), sin(curLatiAngle), cos(curLatiAngle) * sin(curLongAngle));
			m_n[curVerIdx++] = vec3(cos(curLatiAngle) * cos(curLongAngle), sin(curLatiAngle), cos(curLatiAngle) * sin(curLongAngle));
		}
	}

	m_v[curVerIdx] = vec3(0.0f, 1.0f, 0.0f);
	m_n[curVerIdx++] = vec3(0.0f, 1.0f, 0.0f);

	if (curVerIdx != m_numVers)
	{
		std::cout << "ERROR: Wrong vertex num" << std::endl;
	}

	// Triangle indices

	m_numTris = (2 + 2 * (m_latiReso - 2)) * m_longReso;

	m_i = new ivec3[m_numTris];

	for (int longIdx = 0; longIdx < m_longReso; ++longIdx)
	{
		m_i[curTriIdx++] = ivec3(0, (longIdx + 1) % m_longReso + 1, longIdx + 1);
	}

	for (int latiIdx = 1; latiIdx < m_latiReso - 1; ++latiIdx)
	{
		for (int longIdx = 0; longIdx < m_longReso; ++longIdx)
		{
			int baseIdx = 1 + (latiIdx - 1) * m_longReso + longIdx;
			int nextIdx = 1 + ((latiIdx - 1) * m_longReso) + (longIdx + 1) % m_longReso;

			m_i[curTriIdx++] = ivec3(baseIdx, nextIdx + m_longReso, baseIdx + m_longReso);
			m_i[curTriIdx++] = ivec3(baseIdx, nextIdx, nextIdx + m_longReso);
		}
	}

	int baseIdx = m_numVers - m_longReso - 1;
	for (int longIdx = 0; longIdx < m_longReso; ++longIdx)
	{
		m_i[curTriIdx++] = ivec3(baseIdx + longIdx, baseIdx + (longIdx + 1) % m_longReso, m_numVers - 1);
	}

	if (curTriIdx != m_numTris)
	{
		std::cout << "ERROR: Wrong triangle num" << std::endl;
	}
}