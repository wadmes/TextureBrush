#include "brushGlobalRes.h"

#include "renderSystemConfig.h"
#include "viewer.h"

#include "image2D.h"
#include "glTexture.h"
#include "triangleMesh.h"
#include "vertexBufferObject.h"
#include "frameBufferObject.h"
#include "pixelBufferObject.h"
#include "screenPassVBO.h"
#include "shaderProgram.h"
#include "shaderManager.h"
#include "geodesicMesh.h"

using namespace TextureSynthesis;

int CBrushGlobalRes::s_totalTriangleNum = 0;
CTriangleMesh* CBrushGlobalRes::s_pFlatMesh = NULL;
CTriangleMesh* CBrushGlobalRes::s_pSmoothMesh = NULL;

CVertexBufferObject* CBrushGlobalRes::s_pFlatMeshVBO = NULL;
CVertexBufferObject* CBrushGlobalRes::s_pSmoothMeshVBO = NULL;

CFrameBufferObject* CBrushGlobalRes::s_pFrameBuffer = NULL;
CPixelBufferObject* CBrushGlobalRes::s_pPixelBuffer = NULL;

CShaderProgram* CBrushGlobalRes::s_pTrackProgram = NULL;
CShaderProgram* CBrushGlobalRes::s_pShowMarkProgram = NULL;
CShaderProgram* CBrushGlobalRes::s_pFinalRenderProgram = NULL;

CScreenPassVBO* CBrushGlobalRes::s_pScreenRenderPassVBO = NULL;

CGeodesicMesh* CBrushGlobalRes::s_pGeodesicMesh = NULL;

CImage2D * CBrushGlobalRes::s_pSourceImg = NULL;
CGLTexture* CBrushGlobalRes::s_pGLTexture = NULL;

bool CBrushGlobalRes::s_newSketch = false;
bool CBrushGlobalRes::s_endSketch = false;

void CBrushGlobalRes::initGlobalResource()
{
	int winWidth, winHeight;
	string modelName;

	CRenderSystemConfig::getSysCfgInstance()->getModelName(modelName);
	CRenderSystemConfig::getSysCfgInstance()->getWinSize(winWidth, winHeight);

	// Load source image
	s_pSourceImg = new CImage2D();
	s_pSourceImg->load("Image/Example_2.png", TEXELFMT_RGBA, TEXELTYPE_USIGNED_BYTE);

	s_pGLTexture = new CGLTexture(s_pSourceImg);

	// Load model
	s_pFlatMesh = new CTriangleMesh(TMSM_FLAT);
	s_pSmoothMesh = new CTriangleMesh(TMSM_SMOOTH);
	s_pFlatMesh->load(modelName.c_str());
	s_pSmoothMesh->load(modelName.c_str());

	// Setup geodesic mesh
	s_pGeodesicMesh = new CGeodesicMesh(s_pSmoothMesh);

	s_totalTriangleNum = s_pFlatMesh->getTriNum();

	float *pMarkedTriIdx = new float[s_pFlatMesh->getTriNum() * 3];

	// Set all triangle as not marked
	for (unsigned int idx = 0; idx < s_pFlatMesh->getTriNum(); ++idx)
	{
		pMarkedTriIdx[idx * 3 + 0] = pMarkedTriIdx[idx * 3 + 1] = pMarkedTriIdx[idx * 3 + 2] = 0.0f;
	}
	s_pFlatMesh->setPropFloat(pMarkedTriIdx, s_pFlatMesh->getTriNum() * 3);
	
	SAFE_DELETE_ARRAY(pMarkedTriIdx);

	s_pFlatMeshVBO = new CVertexBufferObject(s_pFlatMesh, VBOBM_BASICTRIANGLE | VBOBM_FLOAT_PROP);
	s_pSmoothMeshVBO = new CVertexBufferObject(s_pSmoothMesh);
	s_pScreenRenderPassVBO = new CScreenPassVBO();

	s_pFrameBuffer = new CFrameBufferObject(winWidth, winHeight, 0);
	s_pFrameBuffer->genTextureAndAttach(TEXELFMT_RGBA, TEXELTYPE_USIGNED_BYTE);

	// Init shader programs
	s_pTrackProgram = new CShaderProgram();
	s_pTrackProgram->attachShader(CShaderManager::ST_VERTEX, "shaders/triangleTrack.vert");
	s_pTrackProgram->attachShader(CShaderManager::ST_FRAGMENT, "shaders/triangleTrack.frag");
	s_pTrackProgram->linkProgram();

	s_pFinalRenderProgram = new CShaderProgram();
	s_pFinalRenderProgram->attachShader(CShaderManager::ST_VERTEX, "shaders/basicPerpixelShading.vert");
	s_pFinalRenderProgram->attachShader(CShaderManager::ST_FRAGMENT, "shaders/basicPerpixelShading.frag");
	s_pFinalRenderProgram->linkProgram();

	s_pShowMarkProgram = new CShaderProgram();
	s_pShowMarkProgram->attachShader(CShaderManager::ST_VERTEX, "shaders/triangleMark.vert");
	s_pShowMarkProgram->attachShader(CShaderManager::ST_FRAGMENT, "shaders/triangleMark.frag");
	s_pShowMarkProgram->linkProgram();
}

void CBrushGlobalRes::releaseGlobalResource()
{
	SAFE_DELETE(s_pFlatMesh);
	SAFE_DELETE(s_pSmoothMesh);

	SAFE_DELETE(s_pFlatMeshVBO);
	SAFE_DELETE(s_pSmoothMeshVBO);
	SAFE_DELETE(s_pScreenRenderPassVBO);

	SAFE_DELETE(s_pFrameBuffer);
	SAFE_DELETE(s_pPixelBuffer);

	SAFE_DELETE(s_pTrackProgram);
	SAFE_DELETE(s_pShowMarkProgram);
	SAFE_DELETE(s_pFinalRenderProgram);

	SAFE_DELETE(s_pGeodesicMesh);

	SAFE_DELETE(s_pSourceImg);
	SAFE_DELETE(s_pGLTexture);
}