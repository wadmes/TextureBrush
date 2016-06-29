#include "triangleMesh.h"

using namespace TextureSynthesis;

CTriangleMesh::CTriangleMesh(TriMeshShadeMode shadeMode) : m_shadeMode(shadeMode)
{
	m_numVers = 0;
	m_numTris = 0;

	m_v = NULL;
	m_n = NULL;
	m_i = NULL;

	m_texCoords = NULL;
	m_idxMaterial = NULL;

	m_tangent = NULL;
	m_biTangent = NULL;

	m_propFloat = NULL;
	m_propFloat2 = NULL;
	m_propFloat3 = NULL;
	m_propFloat4 = NULL;

	m_propInt = NULL;
	m_propUInt = NULL;
}

CTriangleMesh::~CTriangleMesh(void)
{
	unload();
}

// Load geometry from OFF model file
void CTriangleMesh::load(const std::string& strFile)
{
	if(isLoaded())
		return;

	m_strSceneFile = strFile;

	int tempNumPoints = 0;	// Number of x,y,z coordinate triples
	int tempNumFaces = 0;	// Number of polygon sets
	int tempNumEdges = 0;	// Unused, except for reading.
	vec3* tempPoints = NULL;	// An array of x,y,z coordinates.
	int** tempFaces = NULL;		// An array of arrays of point

	int*  tempFaceSizes = NULL;	// An array of polygon point counts.

	bool goodLoad = true;		// Set to false if the file appears
	// not to be a valid OFF file.
	char tempBuf[128];		// A buffer for reading strings
	// from the file.

	// Create an input file stream for the file the CArchive
	// is connected to. This allows use of the overloaded
	// extraction operator for conversion from string
	// data to doubles and ints.
	fstream modelFile;
	modelFile.open(strFile);

	// Grab the first string. If it's "OFF", we think this
	// is an OFF file and continue. Otherwise we give up.
	modelFile >> tempBuf;
	if (strcmp(tempBuf, "OFF") != 0) {
		goodLoad = false;
	}

	// Read the sizes for our two arrays, and the third
	// int on the line. If the important two are zero
	// sized, this is a messed up OFF file. Otherwise,
	// we setup our temporary arrays.
	if (goodLoad) {
		modelFile >> tempNumPoints >> tempNumFaces >> tempNumEdges;
		if (tempNumPoints < 1 ||
			tempNumFaces < 1) {
			// If either of these were negative, we make
			// sure that both are set to zero. This is
			// important for later deleting our temporary
			// storage.
			goodLoad = false;
			tempNumPoints = 0;
			tempNumFaces = 0;
		}
		else {
			tempPoints = new vec3[tempNumPoints];
			tempFaces = new int*[tempNumFaces];
			tempFaceSizes = new int[tempNumFaces];
		}
	}

	if (goodLoad) {
		// Load all of the points.
		for (int idx = 0; idx < tempNumPoints; idx++) {
			//tempPoints[idx] = new double[3];
			modelFile >> tempPoints[idx][0] >> tempPoints[idx][1] >> tempPoints[idx][2];
		}

		// Load all of the faces.
		for (int idx = 0; idx < tempNumFaces; idx++) {
			// This tells us how many points make up
			// this face.
			modelFile >> tempFaceSizes[idx];
			// So we declare a new array of that size
			tempFaces[idx] = new int[tempFaceSizes[idx]];
			// And load its elements with the vertex indices.
			for (int verIdx = 0; verIdx < tempFaceSizes[idx]; verIdx++) {
				modelFile >> tempFaces[idx][verIdx];
			}
			// Clear out any face color data by reading up to
			// the newline. 128 is probably considerably more
			// space than necessary, but better safe than
			// sorry.
			modelFile.getline(tempBuf, 128);
		}
	}

	if (goodLoad) {
		// Calculate triangle number
		int totalTriNum = 0;
		for (int facetIdx = 0; facetIdx < tempNumFaces; ++facetIdx)
		{
			assert(tempFaceSizes[facetIdx] > 2);
			totalTriNum += tempFaceSizes[facetIdx] - 2;
		}

		copyData(tempPoints, tempNumPoints, tempFaceSizes, tempNumFaces, tempFaces, totalTriNum);
	}

	// Now that we're done, we have to make sure we
	// free our dynamic memory.
	delete[]tempPoints;

	for (int idx = 0; idx < tempNumFaces; idx++) {
		delete tempFaces[idx];
	}
	delete[]tempFaces;
	delete[]tempFaceSizes;

	modelFile.close();

	// Post processing
	postProcess();
}

void CTriangleMesh::postProcess()
{
	computeBoundingBox();
	normalize();

	if (m_shadeMode == TMSM_FLAT)
	{
		computeFlatNormal();
	}
	else
	{
		computeSmoothNormal();
	}
}

void CTriangleMesh::copyData(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum)
{
	if (m_shadeMode == TMSM_FLAT)
	{
		copyData4FlatMesh(pVertexData, verNum, pFacetVerNumData, facetNum, pFacetVerIdxData, triNum);
	}
	else
	{
		copyData4SmoothMesh(pVertexData, verNum, pFacetVerNumData, facetNum, pFacetVerIdxData, triNum);
	}
}

void CTriangleMesh::copyData4FlatMesh(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum)
{
	m_numVers = triNum * 3;
	m_v = new vec3[m_numVers];

	m_numTris = triNum;
	m_i = new ivec3[m_numTris];

	int curTriIdx = 0;
	int curVerIdx = 0;
	for (int facetIdx = 0; facetIdx < facetNum; ++facetIdx)
	{
		for (int startVerIdx = 0; startVerIdx < pFacetVerNumData[facetIdx] - 2; ++startVerIdx)
		{
			if (startVerIdx == 0)
			{
				m_v[curVerIdx] = pVertexData[pFacetVerIdxData[facetIdx][startVerIdx]];
				curVerIdx++;

				m_v[curVerIdx] = pVertexData[pFacetVerIdxData[facetIdx][startVerIdx + 1]];
				curVerIdx++;

				m_v[curVerIdx] = pVertexData[pFacetVerIdxData[facetIdx][startVerIdx + 2]];
				curVerIdx++;

				m_i[curTriIdx] = ivec3(curTriIdx * 3 + 0, curTriIdx * 3 + 1, curTriIdx * 3 + 2);

				++curTriIdx;
			}
			else
			{
				m_v[curVerIdx] = m_v[curVerIdx - 3];
				curVerIdx++;

				m_v[curVerIdx] = m_v[curVerIdx - 2];
				curVerIdx++;

				m_v[curVerIdx] = pVertexData[pFacetVerIdxData[facetIdx][startVerIdx + 2]];
				curVerIdx++;

				m_i[curTriIdx] = ivec3(curTriIdx * 3 + 0, curTriIdx * 3 + 1, curTriIdx * 3 + 2);

				++curTriIdx;
			}
		}
	}
}

void CTriangleMesh::copyData4SmoothMesh(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum)
{
	m_numVers = verNum;
	m_v = new vec3[m_numVers];
	for (int verIdx = 0; verIdx < m_numVers; ++verIdx)
	{
		m_v[verIdx] = vec3(pVertexData[verIdx][0], pVertexData[verIdx][1], pVertexData[verIdx][2]);
	}

	m_numTris = triNum;
	m_i = new ivec3[m_numTris];

	int curTriIdx = 0;
	for (int facetIdx = 0; facetIdx < facetNum; ++facetIdx)
	{
		for (int startVerIdx = 0; startVerIdx < pFacetVerNumData[facetIdx] - 2; ++startVerIdx)
		{
			if (startVerIdx == 0)
			{
				m_i[curTriIdx] = ivec3(pFacetVerIdxData[facetIdx][startVerIdx],
					pFacetVerIdxData[facetIdx][startVerIdx + 1],
					pFacetVerIdxData[facetIdx][startVerIdx + 2]);

				++curTriIdx;
			}
			else
			{
				m_i[curTriIdx] = ivec3(m_i[curTriIdx - 1][0], m_i[curTriIdx - 1][2], pFacetVerIdxData[facetIdx][startVerIdx + 2]);

				++curTriIdx;
			}
		}
	}
}

void CTriangleMesh::copyVertexData()
{

}

void CTriangleMesh::copyFacetData()
{

}

void CTriangleMesh::unload()
{
	if(!m_numTris)
		return;
	
	SAFE_DELETE_ARRAY(m_v);
	SAFE_DELETE_ARRAY(m_n);
	SAFE_DELETE_ARRAY(m_i);
	SAFE_DELETE_ARRAY(m_texCoords);

	SAFE_DELETE_ARRAY(m_idxMaterial);

	SAFE_DELETE_ARRAY(m_tangent);
	SAFE_DELETE_ARRAY(m_biTangent);

	SAFE_DELETE_ARRAY(m_propFloat);
	SAFE_DELETE_ARRAY(m_propFloat2);
	SAFE_DELETE_ARRAY(m_propFloat3);
	SAFE_DELETE_ARRAY(m_propFloat4);

	SAFE_DELETE_ARRAY(m_propInt);

	SAFE_DELETE_ARRAY(m_propUInt);

	m_numVers = 0;
	m_numTris = 0;
}

void CTriangleMesh::setVertex(vec3* pVertices, int vNum)
{
	if (m_numVers != 0)
	{
		m_numVers = 0;
		SAFE_DELETE_ARRAY(m_v);
	}

	m_numVers = vNum;
	m_v = new vec3[vNum];
	memcpy(m_v, pVertices, sizeof(vec3) * vNum);
}

void CTriangleMesh::setVerNormal(vec3* pNormals, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout<<"ERROR: Vertex number and normal number don't match!"<<std::endl;
		return;
	}

	if (m_n != NULL)
	{
		SAFE_DELETE_ARRAY(m_n);
	}

	m_n = new vec3[vNum];
	memcpy(m_n, pNormals, sizeof(vec3) * vNum);
}

void CTriangleMesh::setIndices(ivec3* pIndices, int vNum)
{
	if (m_numTris != 0)
	{
		m_numTris = 0;
		SAFE_DELETE_ARRAY(m_i);
	}

	m_numTris = vNum;
	m_i = new ivec3[vNum];
	memcpy(m_i, pIndices, sizeof(ivec3) * vNum);
}

void CTriangleMesh::setTexcoords(vec3* pTexcoords, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout<<"ERROR: Vertex number and texcoord number don't match!"<<std::endl;
		return;
	}

	if (m_texCoords != NULL)
	{
		SAFE_DELETE_ARRAY(m_texCoords);
	}

	m_texCoords = new vec3[vNum];
	memcpy(m_texCoords, pTexcoords, sizeof(vec3) * vNum);
}

void CTriangleMesh::setTangent(vec3* pTangents, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout<<"ERROR: Vertex number and tangent number don't match!"<<std::endl;
		return;
	}

	if (m_tangent != NULL)
	{
		SAFE_DELETE_ARRAY(m_tangent);
	}

	m_tangent = new vec3[vNum];
	memcpy(m_tangent, pTangents, sizeof(vec3) * vNum);
}

void CTriangleMesh::setBiTangent(vec3* pBiTangents, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout<<"ERROR: Vertex number and bi-tangent number don't match!"<<std::endl;
		return;
	}

	if (m_biTangent != NULL)
	{
		SAFE_DELETE_ARRAY(m_biTangent);
	}

	m_biTangent = new vec3[vNum];
	memcpy(m_biTangent, pBiTangents, sizeof(vec3) * vNum);
}

void CTriangleMesh::setPropFloat(float *pPropData, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout << "ERROR: Vertex number and bi-tangent number don't match!" << std::endl;
		return;
	}

	if (m_propFloat != NULL)
	{
		SAFE_DELETE_ARRAY(m_propFloat);
	}

	m_propFloat = new float[vNum];
	memcpy(m_propFloat, pPropData, sizeof(float)* vNum);
}

void CTriangleMesh::setPropFloat2(vec2 *pPropData, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout << "ERROR: Vertex number and bi-tangent number don't match!" << std::endl;
		return;
	}

	if (m_propFloat2 != NULL)
	{
		SAFE_DELETE_ARRAY(m_propFloat);
	}

	m_propFloat2 = new vec2[vNum];
	memcpy(m_propFloat2, pPropData, sizeof(vec2)* vNum);
}

void CTriangleMesh::setPropInt(int *pPropData, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout << "ERROR: Vertex number and bi-tangent number don't match!" << std::endl;
		return;
	}

	if (m_propInt != NULL)
	{
		SAFE_DELETE_ARRAY(m_propInt);
	}

	m_propInt = new int[vNum];
	memcpy(m_propInt, pPropData, sizeof(int)* vNum);
}

void CTriangleMesh::setPropUInt(uint *pPropData, int vNum)
{
	if (m_numVers != vNum)
	{
		std::cout << "ERROR: Vertex number and bi-tangent number don't match!" << std::endl;
		return;
	}

	if (m_propUInt != NULL)
	{
		SAFE_DELETE_ARRAY(m_propUInt);
	}

	m_propUInt = new uint[vNum];
	memcpy(m_propUInt, pPropData, sizeof(uint)* vNum);
}

void CTriangleMesh::windTriVertexIdxOrder()
{
	for (int triIdx = 0; triIdx < m_numTris; ++triIdx)
	{
		m_i[triIdx] = ivec3(m_i[triIdx][0], m_i[triIdx][1], m_i[triIdx][1]);
	}
}

void CTriangleMesh::normalize()
{
	vec3 center = m_boundMin + m_boundMax;
	center *= 0.5f;

	float scale = max(max(m_boundMax[0] - m_boundMin[0], m_boundMax[1] - m_boundMin[1]), m_boundMax[2] - m_boundMin[2]);
	scale = 2.0f / scale;

	for (int verIdx = 0; verIdx < m_numVers; ++verIdx)
	{
		m_v[verIdx] -= center;

		m_v[verIdx] *= scale;
	}

	m_boundMin -= center;
	m_boundMin *= scale;

	m_boundMax -= center;
	m_boundMax *= scale;
}

void CTriangleMesh::computeBoundingBox()
{
	m_boundMin = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_boundMax = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int verIdx = 0; verIdx < m_numVers; ++verIdx)
	{
		m_boundMin = vec3(min(m_boundMin[0], m_v[verIdx][0]),
			min(m_boundMin[1], m_v[verIdx][1]),
			min(m_boundMin[2], m_v[verIdx][2])); 

		m_boundMax = vec3(max(m_boundMax[0], m_v[verIdx][0]),
			max(m_boundMax[1], m_v[verIdx][1]),
			max(m_boundMax[2], m_v[verIdx][2]));
	}
}

void CTriangleMesh::computeFlatNormal()
{
	if (m_n != NULL)
	{
		SAFE_DELETE_ARRAY(m_n);
	}

	m_n = new vec3[m_numTris * 3];
	memset(m_n, 0, sizeof(vec3)* m_numTris * 3);

	for (int triIdx = 0; triIdx < m_numTris; ++triIdx)
	{
		vec3 v0, v1, v2;
		v0 = m_v[m_i[triIdx][0]];
		v1 = m_v[m_i[triIdx][1]];
		v2 = m_v[m_i[triIdx][2]];

		vec3 triNormal = glm::cross(v1 - v0, v2 - v0);

		if (glm::length(triNormal) < 0.000001f)
		{
			//cout << "WARNING: Zero facet normal!" << endl;
		}

		vec3 normalizedNormal = glm::normalize(triNormal);
		m_n[triIdx * 3 + 0] = normalizedNormal;
		m_n[triIdx * 3 + 1] = normalizedNormal;
		m_n[triIdx * 3 + 2] = normalizedNormal;
	}
}

void CTriangleMesh::computeSmoothNormal()
{
	if (m_n != NULL)
	{
		SAFE_DELETE_ARRAY(m_n);
	}

	m_n = new vec3[m_numVers];
	memset(m_n, 0, sizeof(vec3)* m_numVers);

	for (int triIdx = 0; triIdx < m_numTris; ++triIdx)
	{
		vec3 v0, v1, v2;
		v0 = m_v[m_i[triIdx][0]];
		v1 = m_v[m_i[triIdx][1]];
		v2 = m_v[m_i[triIdx][2]];

		vec3 triNormal = glm::cross(v1 - v0, v2- v0);

		if (glm::length(triNormal) < 0.000001f)
		{
			//cout << "WARNING: Zero facet normal!" << endl;
		}

		m_n[m_i[triIdx][0]] += triNormal;
		m_n[m_i[triIdx][1]] += triNormal;
		m_n[m_i[triIdx][2]] += triNormal;
	}

	for (int verIdx = 0; verIdx < m_numVers; ++verIdx)
	{
		if (glm::length(m_n[verIdx]) < 0.000001f)
		{
			//cout << "WARNING: Zero vertex normal!" << endl;
		}
		else
		{
			m_n[verIdx] = glm::normalize(m_n[verIdx]);
		}
	}
}

void CTriangleMesh::checkMatch(int vNum)
{
	if (m_shadeMode == TMSM_FLAT)
	{
		if (m_numTris != vNum)
		{
			cout << "ERROR: data count doesn't match with triangle number!" << endl;
			exit(-1);
		}
	}
	else
	{
		if (m_numVers != vNum)
		{
			cout << "ERROR: data count doesn't match with vertex number!" << endl;
			exit(-1);
		}
	}
}