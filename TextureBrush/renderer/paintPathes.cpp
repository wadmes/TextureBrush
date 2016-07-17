#include "paintPathes.h"

#include <set>

#include "pixelBufferObject.h"
#include "renderSystemConfig.h"
#include "brushGlobalRes.h"
#include "geodesicMesh.h"

#include "triangleMesh.h"
#include "vertexBufferObject.h"

using std::set;
using namespace TextureSynthesis;

CPaintPathes* CPaintPathes::Instance()
{
	static CPaintPathes* s_pPaintPathes = NULL;

	if (s_pPaintPathes == NULL)
	{
		s_pPaintPathes = new CPaintPathes();
	}

	return s_pPaintPathes;
}

CPaintPathes::CPaintPathes() : m_pPBO(NULL), m_pTriangleIdxData(NULL)
{
	int winWidth, winHeight;
	CRenderSystemConfig::getSysCfgInstance()->getWinSize(winWidth, winHeight);

	m_pPBO = new CPixelBufferObject(TEXELFMT_RGBA, TEXELTYPE_FLOAT, winWidth, winHeight);
}

CPaintPathes::~CPaintPathes()
{
	SAFE_DELETE(m_pPBO);
}

void CPaintPathes::startNewPath()
{
	cout << "Info: New painting path!" << endl;
	m_pathPointVec.clear();
}

void CPaintPathes::addPointToPath(const ivec2 &newPos)
{
	if (m_pathPointVec.size() == 0)
	{
		cout << "[" << newPos[0] << "," << newPos[1] << "]" << endl;
		m_pathPointVec.push_back(newPos);
		return;
	}

	ivec2 prePos = m_pathPointVec[m_pathPointVec.size() - 1];

	if (prePos[0] != newPos[0] || prePos[1] != newPos[1])
	{
		cout << "[" << newPos[0] << "," << newPos[1] << "]" << endl;
		m_pathPointVec.push_back(newPos);
	}
	
	
	// Transform path vertices in pixel coordinates into world coordinates
	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float depth;
	glReadPixels(newPos[0], newPos[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	cout << "Depth is " << depth << endl;

	double objPos[3];
	gluUnProject(newPos[0], newPos[1], depth, modelview, projection, viewport, &objPos[0], &objPos[1], &objPos[2]);

	cout << objPos[0] << " " << objPos[1] << " " << objPos[2] << endl;
}

void CPaintPathes::endPath()
{
	cout << "Info: End painting path!" << endl;
	m_pathVec.clear(); // Only maintain one curve currently, remove this to maintain multiple curves
	m_pathVec.push_back(m_pathPointVec);
}

// Copy rendered triangle index texture from pixel buffer object
void CPaintPathes::extractTriangleIndexTexture(GLuint texId)
{
	//cout << "Info: Start a new sketch!" << endl;
	m_pTriangleIdxData = m_pPBO->getDataPointer(texId);
}

void CPaintPathes::compute3dPath()
{
	int winWidth, winHeight;
	CRenderSystemConfig::getSysCfgInstance()->getWinSize(winWidth, winHeight);

	m_triIdxVec.clear();
	m_pathVec3D.clear();

	ivec3 *pTriIndices = CBrushGlobalRes::s_pSmoothMesh->getTriIdx();
	float* pTriMarkData = CBrushGlobalRes::s_pFlatMesh->getPropFloatData();
	// Reset all triangles as not marked
	for (int idx = 0; idx < CBrushGlobalRes::s_pFlatMesh->getVerNum(); ++idx) {
		pTriMarkData[idx] = 0.0f;
	}

	vector<int> newPathTriangleIdxVec;
	vector<vec3> newPathPointVec3D;
	set<int> curveTriangleIdxSet; // Set to maintain triangle index on curve to keep unique
	// Only one path currently
	for (int pathIdx = 0; pathIdx < m_pathVec.size(); ++pathIdx)
	{
		newPathTriangleIdxVec.clear();
		newPathPointVec3D.clear();
		curveTriangleIdxSet.clear();

		cout << "Info: New path in 3D" << endl;

		//Test
		set<int> chosenVerIdxSet;
		set<int> bannedVerIdxSet;
		chosenVerIdxSet.clear();
		bannedVerIdxSet.clear();
		//Test End

		for (int pointIdx = 0; pointIdx < m_pathVec[pathIdx].size(); ++pointIdx)
		{
			ivec2 curPointScreenPos = m_pathVec[pathIdx][pointIdx];
			int pixelIdx = (winHeight - curPointScreenPos[1] - 1) * winWidth + curPointScreenPos[0];

			// Fetch vertex index from frame buffer texture
			int curTriIdx = m_pTriangleIdxData[pixelIdx] - 1;

			if (curTriIdx < 0 || curTriIdx >= CBrushGlobalRes::s_pSmoothMesh->getTriNum())
			{
				continue;
			}

			// Add triangle index to set
			curveTriangleIdxSet.insert(curTriIdx);

			// A simple scheme to add vertex index
			/*cout << "Size is " << pTriIndices[curTriIdx].length() << endl;
			cout << "Three points are " << pTriIndices[curTriIdx][0] << " " << pTriIndices[curTriIdx][1] << " " << pTriIndices[curTriIdx][2] << endl;
			if (newPathTriangleIdxVec.size() == 0 || (pTriIndices[curTriIdx][0] != newPathTriangleIdxVec[newPathTriangleIdxVec.size() - 1]))
			{
			newPathTriangleIdxVec.push_back(pTriIndices[curTriIdx][0]);
			}
			else
			{
			newPathTriangleIdxVec.push_back(pTriIndices[curTriIdx][1]);
			}*/

			// An advanced scheme to add vertex index
			if (pointIdx == 0) {
				int i;
				newPathTriangleIdxVec.push_back(pTriIndices[curTriIdx][0]);
				newPathTriangleIdxVec.push_back(pTriIndices[curTriIdx][1]);
				chosenVerIdxSet.insert(pTriIndices[curTriIdx][0]);
				chosenVerIdxSet.insert(pTriIndices[curTriIdx][1]);
				for (i = 2; i < pTriIndices[curTriIdx].length(); i++) {
					bannedVerIdxSet.insert(pTriIndices[curTriIdx][i]);
				}

				cout << "Triangle " << curTriIdx << " Size " << pTriIndices[curTriIdx].length() << endl;
				cout << "Vertices are " << pTriIndices[curTriIdx][0] << " " << pTriIndices[curTriIdx][1] << " " << pTriIndices[curTriIdx][2] << endl;
				cout << "Adding vertex " << pTriIndices[curTriIdx][0] << ", " << pTriIndices[curTriIdx][1] << endl;
				cout << endl;
			}
			else {
				int pointStatus[3] = { 0 };
				int chosenCounter = 2;
				int i;
				for (i = 0; i < pTriIndices[curTriIdx].length(); i++) {
					if (chosenVerIdxSet.find(pTriIndices[curTriIdx][i]) != chosenVerIdxSet.end()) {
						pointStatus[i] = 1;
						chosenCounter--;
					}
					if (bannedVerIdxSet.find(pTriIndices[curTriIdx][i]) != bannedVerIdxSet.end()) {
						pointStatus[i] = -1;
					}
				}
				for (i = 0; i < pTriIndices[curTriIdx].length() && chosenCounter > 0; i++) {
					if (pointStatus[i] == 0) {
						cout << "Triangle " << curTriIdx << " Size " << pTriIndices[curTriIdx].length() << endl;
						cout << "Vertices are "
							<< pTriIndices[curTriIdx][0] << "(" << pointStatus[0] << ") "
							<< pTriIndices[curTriIdx][1] << "(" << pointStatus[1] << ") "
							<< pTriIndices[curTriIdx][2] << "(" << pointStatus[2] << ") " << endl;
						cout << "Adding vertex " << pTriIndices[curTriIdx][i] << endl;
						cout << endl;

						pointStatus[i] = 1;
						newPathTriangleIdxVec.push_back(pTriIndices[curTriIdx][i]);
						chosenVerIdxSet.insert(pTriIndices[curTriIdx][i]);
						chosenCounter = 0;
					}
				}
				for (i = 0; i < pTriIndices[curTriIdx].length(); i++) {
					if (pointStatus[i] != 1)
						bannedVerIdxSet.insert(pTriIndices[curTriIdx][i]);
				}
			}

			pTriMarkData[curTriIdx * 3 + 0] = pTriMarkData[curTriIdx * 3 + 1] = pTriMarkData[curTriIdx * 3 + 2] = 1.0f;

		}
	}

	float *pDisData = new float[CBrushGlobalRes::s_pSmoothMesh->getVerNum()];

	// Collect path segment between each two consecutive vertices
	m_zeroOrderPathIdxVec.clear();
	m_firstOrderPathIdxVec.clear();

	for (int verIdx = 1; verIdx < newPathTriangleIdxVec.size(); ++verIdx)
	{
		CBrushGlobalRes::s_pGeodesicMesh->computePath(
			newPathTriangleIdxVec[verIdx - 1], newPathTriangleIdxVec[verIdx]);

		for (int pointIdx = 0; pointIdx < CBrushGlobalRes::s_pGeodesicMesh->getZeroOrderPathIdxVec().size(); ++pointIdx)
		{
			m_zeroOrderPathIdxVec.push_back(CBrushGlobalRes::s_pGeodesicMesh->getZeroOrderPathIdxVec()[pointIdx]);
		}
		for (int pointIdx = 0; pointIdx < CBrushGlobalRes::s_pGeodesicMesh->getFirstOrderPathIdxVec().size(); ++pointIdx)
		{
			m_firstOrderPathIdxVec.push_back(CBrushGlobalRes::s_pGeodesicMesh->getFirstOrderPathIdxVec()[pointIdx]);
		}
	}

	//*********************************************************************************
	// Todo-1:	Refine seed point set which satisfies that 
	//			at most two vertices on a triangle can be added as seed points
	// Input:	Triangle index set (variable curveTriangleIdxSet) on curve;
	//			Triangle mesh data (triangle vertex corresponding data)
	// Output:	Selected seed vertex index vector, 
	//			addSeeds function will use this vector as parameter to set seed points
	//			pls modify correspondingly.
	//*********************************************************************************

	/*cout << "Altogeter " << curveTriangleIdxSet.size() << " vertices" << endl;

	for (int verIdx = 0; verIdx < newPathTriangleIdxVec.size(); ++verIdx)
	{
	cout << verIdx << " " << newPathTriangleIdxVec[verIdx] << endl;
	}*/

	CBrushGlobalRes::s_pGeodesicMesh->resetGeoMesh();
	CBrushGlobalRes::s_pGeodesicMesh->addSeeds(newPathTriangleIdxVec);
	CBrushGlobalRes::s_pGeodesicMesh->computeGeodesics(pDisData);

	collectVertexVectors();

	calculateEquidisLineSegments();

	assignLocalTexcoords();

	//*********************************************************************************
	// Todo-5:	Update vertex buffer (s_pSmoothMesh) texcoord data
	//*********************************************************************************

	CBrushGlobalRes::s_pSmoothMesh->setPropFloat(pDisData, CBrushGlobalRes::s_pSmoothMesh->getVerNum());
	CBrushGlobalRes::s_pSmoothMeshVBO->updateBuffer(VBOBM_FLOAT_PROP);
	CBrushGlobalRes::s_pFlatMeshVBO->updateBuffer(VBOBM_FLOAT_PROP);

	SAFE_DELETE_ARRAY(pDisData);

	m_pathVec.clear();
}

//**************************************************************************
//	Todo-2:	Collect vertices(might be subdivided vertices) with equal geodesic distance
//  Input:	Vertex geodesic distance vector
//			Triangle mesh data(triangle vertex corresponding data)
//	Output: One vertex vector with equal geodesic distance for each distance value
//			(such as 0.1, 0.2, ... , 1.0)

//	Hint :	If a specific distance value is in the range of the distance values of two vertices 
//			on one edge, the point on the edge with same interpolated distance value 
//			should be added to the vertex vector
//			Subdivision data should be maintained for further process
//**************************************************************************
void CPaintPathes::collectVertexVectors()
{
	vector<int> equalGeoDisanceVertices[10];
	vector<int> subdivision;
}

//**************************************************************************
//	Todo-3:	Remove points surrounding curve ends to generate two curve segments
//			with equal geodesic distance
//  Input:	Vertex vector with equal geodesic distance
//	Output: Two vertex vectors

//	Hint:	Remove those points with projection points located 
//			on the extension of the sketch curve
//**************************************************************************
void CPaintPathes::calculateEquidisLineSegments()
{

}

//**************************************************************************
//	Todo-4:	Assign texcoords to the vertices surrounding the sketch curve
//			according to the geodesic distance and ratio on the previously 
//			computed equidistance curve
//  Input:	Vertex geodesic distance data and equidistance curves
//	Output: Vertex texcoord data

//	Hint:	The process Only process vertices with distance in a range
//			Other vertices will be assigned with invalid texcoords
//**************************************************************************
void CPaintPathes::assignLocalTexcoords()
{

}

/*
void AddPointFunc(int TriNum)
{
	if TriNum > pTriIndices[TriNum].length()
		return 0;
	else
		for (int i = 0; i < 3; i++)
		{
			if (chosenVerIdxSet.find(pTriIndices[curTriIdx][i]) == chosenVerIdxSet.end())//the point i is not be chosen 
				CompareSet.insert(pTriIndices[curTriIdx][i]);// put the point i into the compared set and then find the point with the minimum distance.
		}
	Find the point with the minimum distance between the point and the line draw by user in the set "CompareSet"..I don't know how to realize it..= = 
		Let's assume the chosen point(minimum distance point) is  MinP.
		chosenVerIdxSet.insert(MinP);// insert the chosen point
		if (MinP == pTriIndices[TriNum + 1][0] || [1] || [2])
			AddPointFunc(TriNum + 1);
		else if (Minp == pTriIndices[TriNum + 2][0] || [1] || [2])
			AddPointFunc(TriNum + 2);
	return 0;
}

int TriNum = 0;// in the first triangle, choose the minimum distance point .
assume the chosen point(minimum distance point) is  MinP.
chosenVerIdxSet.insert(MinP);// insert the chosen point
if (MinP == pTriIndices[TriNum + 1][0] || [1] || [2])
AddPointFunc(TriNum + 1);
else if (Minp == pTriIndices[TriNum + 2][0] || [1] || [2])
AddPointFunc(TriNum + 2);
else
AddPointFunc(TriNum);
*/
