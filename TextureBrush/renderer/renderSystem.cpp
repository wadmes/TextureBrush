#include "renderSystem.h"

#include "renderUtilities.h"
#include "renderSystemConfig.h"
#include "viewer.h"

#include "../renderer/triangleMesh.h"
#include "../renderer/sphereGeometry.h"
#include "../renderer/vertexBufferObject.h"
#include "../renderer/frameBufferObject.h"
#include "../renderer/pixelBufferObject.h"
#include "../renderer/screenPassVBO.h"
#include "../renderer/shaderProgram.h"
#include "../renderer/shaderManager.h"
#include "../renderer/glTexture.h"

#include "../renderer/paintPathes.h"
#include "../renderer/geodesicMesh.h"
#include "../renderer/brushGlobalRes.h"

using namespace TextureSynthesis;

CRenderSystem* CRenderSystem::Instance()
{
	static CRenderSystem *s_pRenderSystem = NULL;

	if (s_pRenderSystem == NULL)
	{
		s_pRenderSystem = new CRenderSystem();
	}

	return s_pRenderSystem;
}

CRenderSystem::CRenderSystem() :m_pMainWindow(NULL)
{
	CEventManager::getEventManagerInstance()->registerKeyListener(GLFW_KEY_UP, this);
	CEventManager::getEventManagerInstance()->registerKeyListener(GLFW_KEY_DOWN, this);
	CEventManager::getEventManagerInstance()->registerKeyListener(GLFW_KEY_LEFT, this);
	CEventManager::getEventManagerInstance()->registerKeyListener(GLFW_KEY_RIGHT, this);
}

CRenderSystem::~CRenderSystem()
{
}

void CRenderSystem::initRenderSystem()
{
	// Must be initialized by this order!!
	initGlfw();
	createRenderWindow();

	initGlew();

	initDevIL();

	setEventCallbackFuncs();

	CBrushGlobalRes::initGlobalResource();
}

void CRenderSystem::initGlew()
{
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0 "
		"GL_ARB_pixel_buffer_object"
		)) {
		cout << "ERROR: Support for necessary OpenGL extensions missing." << endl;

		exit(-1);
	}

	char *GL_version = (char *)glGetString(GL_VERSION);
	cout << "Info: supported OpenGL version:" << GL_version << std::endl;
}

void CRenderSystem::initGlfw()
{
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Info: Glfw initialized!" << endl;
	}

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void CRenderSystem::initDevIL()
{
	ilInit();

	iluInit();

	ilutRenderer(ILUT_OPENGL);

	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);
	
	cout << "Info: DevIL initialized!" << endl;
}

void CRenderSystem::setEventCallbackFuncs()
{
	glfwSetKeyCallback(m_pMainWindow, CEventManager::keyCallback);
	glfwSetMouseButtonCallback(m_pMainWindow, CEventManager::mouseButtonCallback);
	glfwSetCursorPosCallback(m_pMainWindow, CEventManager::mousePosCallback);
	glfwSetScrollCallback(m_pMainWindow, CEventManager::mouseWheelCallback);
}

void CRenderSystem::createRenderWindow()
{
	int winWidth, winHeight;

	CRenderSystemConfig::getSysCfgInstance()->getWindowName(m_winTitle);
	CRenderSystemConfig::getSysCfgInstance()->getWinSize(winWidth, winHeight);

	m_pMainWindow = glfwCreateWindow(winWidth, winHeight, m_winTitle.c_str(), NULL, NULL);

	if (!m_pMainWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_pMainWindow);
}

void CRenderSystem::cleanSystem()
{
// 	CViewer *pViewer = CViewer::getViewerInstance();
// 	SAFE_DELETE(pViewer);
// 
// 	CEventManager *pEventManager = CEventManager::getEventManagerInstance();
// 	SAFE_DELETE(pEventManager);
}

void CRenderSystem::render()
{
	int frame = 0;

	float totalTriangleNum = 1.0f * CBrushGlobalRes::s_totalTriangleNum;

	while (!glfwGetKey(m_pMainWindow, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(m_pMainWindow))
	{
		CViewer::getViewerInstance()->aim();

		CRenderUtilities::drawAxis();
		CRenderUtilities::drawWireCube(vec3(-1.0), vec3(1.0));

		// Render triangle index
		if (CBrushGlobalRes::s_newSketch)
		{
			CBrushGlobalRes::s_newSketch = false;

			// Render triangle index into frame buffer and extract triangle index with screen space coordinate
			CBrushGlobalRes::s_pFrameBuffer->activate();

			CBrushGlobalRes::s_pTrackProgram->activate();

			CBrushGlobalRes::s_pTrackProgram->updateModelViewMat("u_modelviewMatrix");
			CBrushGlobalRes::s_pTrackProgram->updateProjMat("u_projMatrix");
			CBrushGlobalRes::s_pFlatMeshVBO->display(VBORM_TRIANGLES);

			CBrushGlobalRes::s_pTrackProgram->deactivate();

			CPaintPathes::Instance()->extractTriangleIndexTexture(CBrushGlobalRes::s_pFrameBuffer->getColorBuffer(0));

			CBrushGlobalRes::s_pFrameBuffer->deactivate();
		}

		// Render marked triangles on the curve
		/*CBrushGlobalRes::s_pShowMarkProgram->activate();

		CBrushGlobalRes::s_pShowMarkProgram->updateModelViewMat("u_modelviewMatrix");
		CBrushGlobalRes::s_pShowMarkProgram->updateProjMat("u_projMatrix");

		CBrushGlobalRes::s_pFlatMeshVBO->display(VBORM_TRIANGLES);

		CBrushGlobalRes::s_pShowMarkProgram->deactivate();*/

		// Render final result
		CBrushGlobalRes::s_pFinalRenderProgram->activate();

		CBrushGlobalRes::s_pFinalRenderProgram->updateModelViewMat("u_modelviewMatrix");
		CBrushGlobalRes::s_pFinalRenderProgram->updateProjMat("u_projMatrix");
		CBrushGlobalRes::s_pFinalRenderProgram->updateTexture2D("u_sourceTex", CBrushGlobalRes::s_pGLTexture->getGLTexHandle(), 0);

			//LIQILIQI
		int tmp;
		tmp = 1;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		CBrushGlobalRes::s_pFinalRenderProgram->updateUniformiv("is_line", &tmp, 1, 1);
		CBrushGlobalRes::s_pSmoothMeshVBO->display(VBORM_TRIANGLES);
		tmp = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		CBrushGlobalRes::s_pFinalRenderProgram->updateUniformiv("is_line", &tmp, 1, 1);
		CBrushGlobalRes::s_pSmoothMeshVBO->display(VBORM_TRIANGLES);

		CBrushGlobalRes::s_pFinalRenderProgram->deactivate();

		glfwSwapBuffers(m_pMainWindow);
		glfwPollEvents();
		++frame;
	}

	glfwTerminate();
}

bool CRenderSystem::keyPressed(const KeyEvent &arg)
{
	switch (arg.key)
	{
	case GLFW_KEY_UP:
		break;
	case GLFW_KEY_DOWN:
		break;
	case GLFW_KEY_LEFT:
		break;
	case GLFW_KEY_RIGHT:
		break;
	default:
		break;
	}

	return true;
}

bool CRenderSystem::keyReleased(const KeyEvent &arg)
{
	return true;
}