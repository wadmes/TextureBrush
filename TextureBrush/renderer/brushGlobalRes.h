#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

class CImage2D;
class CGLTexture;
class CTriangleMesh;
class CVertexBufferObject;
class CScreenPassVBO;
class CFrameBufferObject;
class CPixelBufferObject;
class CShaderProgram;
class CGeodesicMesh;

class CBrushGlobalRes
{
public:
	static void initGlobalResource();
	static void releaseGlobalResource();

	static int s_totalTriangleNum;
	static CTriangleMesh* s_pFlatMesh;
	static CTriangleMesh* s_pSmoothMesh;

	static CVertexBufferObject* s_pFlatMeshVBO;
	static CVertexBufferObject* s_pSmoothMeshVBO;
	static CScreenPassVBO* s_pScreenRenderPassVBO;

	static CFrameBufferObject* s_pFrameBuffer;
	static CPixelBufferObject* s_pPixelBuffer;

	static CShaderProgram* s_pTrackProgram;
	static CShaderProgram* s_pShowMarkProgram;
	static CShaderProgram* s_pFinalRenderProgram;

	static CGeodesicMesh* s_pGeodesicMesh;

	static CImage2D* s_pSourceImg;
	static CGLTexture* s_pGLTexture;

	// State for texture brush
	static bool s_newSketch;
	static bool s_endSketch;
};

}