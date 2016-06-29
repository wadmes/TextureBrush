#include "viewer.h"
#include "camera.h"
#include "renderSystemConfig.h"
#include "renderSystem.h"
#include "paintPathes.h"
#include "brushGlobalRes.h"

using namespace TextureSynthesis;

CViewer::CViewer() : m_mouseDown(false), m_preWheelPos(0), m_startTumbling(false), m_zoomMode(false), m_isPaintingMode(false)
{
	m_pCamera = new CCamera();

	CRenderSystemConfig::getSysCfgInstance()->getCameraAdjust(m_tumblingSpeed, m_zoomSpeed, m_moveSpeed);

	CEventManager::getEventManagerInstance()->registerKeyListener(GLFW_KEY_LEFT_SHIFT, this);
	CEventManager::getEventManagerInstance()->registerMouseListener(GLFW_MOUSE_BUTTON_LEFT, this);
	CEventManager::getEventManagerInstance()->registerMouseListener(GLFW_MOUSE_BUTTON_RIGHT, this);
}

CViewer::~CViewer()
{
	std::cout << "Release: Viewer object released!" << std::endl;
	SAFE_DELETE(m_pCamera);
}

CViewer* CViewer::getViewerInstance()
{
	static CViewer* s_pViewer = NULL;

	if (s_pViewer == NULL)
	{
		s_pViewer = new CViewer();
	}

	return s_pViewer;
}

void CViewer::aim()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int width, height;
	glfwGetWindowSize(CRenderSystem::Instance()->getGLFWWindow(), &width, &height);
	height = height < 1 ? 1 : height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Window aspect (assumes square pixels)
	float aspectRatio = (float)width / (float)height;
	// Set perspective view
	gluPerspective(m_pCamera->m_fov, aspectRatio, m_pCamera->m_nearPlane, m_pCamera->m_farPlane);

	vec3 camPos(m_pCamera->m_lookX + m_pCamera->m_radius * std::cos(glm::radians(m_pCamera->m_head)) * std::sin(glm::radians(m_pCamera->m_pitch)),
		m_pCamera->m_lookY + m_pCamera->m_radius * std::sin(glm::radians(m_pCamera->m_head)),
		m_pCamera->m_lookZ + m_pCamera->m_radius * std::cos(glm::radians(m_pCamera->m_head)) * std::cos(glm::radians(m_pCamera->m_pitch)));

	m_forward = vec3(m_pCamera->m_lookX - camPos.x, m_pCamera->m_lookY - camPos.y, m_pCamera->m_lookZ - camPos.z);
	m_forward = glm::normalize(m_forward);
	vec3 up = vec3(0.0f, (std::cos(glm::radians(m_pCamera->m_head)) > 0.0f) ? 1.0f : -1.0f, 0.0f);
	m_side = glm::cross(m_forward, up);
	m_up = glm::cross(m_side, m_forward);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Set up model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos.x, camPos.y, camPos.z,
		m_pCamera->m_lookX, m_pCamera->m_lookY, m_pCamera->m_lookZ,
		up.x, up.y, up.z);
}

bool CViewer::keyPressed(const KeyEvent &arg)
{
	if (arg.key == GLFW_KEY_LEFT_SHIFT)
	{
		CBrushGlobalRes::s_newSketch = true;
		m_isPaintingMode = true;
	}

	return true;
}

bool CViewer::keyReleased(const KeyEvent &arg)
{
	if (arg.key == GLFW_KEY_LEFT_SHIFT)
	{
		m_isPaintingMode = false;
		CPaintPathes::Instance()->compute3dPath();
	}

	return true;
}

bool CViewer::mouseMoved(const MouseEvent &arg)
{
	if (m_isPaintingMode)
	{
		if (m_mouseDown)
		{
			CPaintPathes::Instance()->addPointToPath(ivec2(arg.state.xPos, arg.state.yPos));
		}
	}
	else
	{
		if (m_startTumbling) {
			m_preMouseXPos = arg.state.xPos;
			m_preMouseYPos = arg.state.yPos;
			m_startTumbling = false;
		}

		float dx, dy;
		dx = (float)arg.state.xPos - m_preMouseXPos;
		dy = (float)arg.state.yPos - m_preMouseYPos;

		if (m_mouseDown && !m_zoomMode)
		{
			m_pCamera->m_head += dy * m_tumblingSpeed;
			m_pCamera->m_pitch += dx * m_tumblingSpeed;
		}
		else if (m_mouseDown && m_zoomMode)
		{
			m_pCamera->m_lookX += (dy * m_up.x - dx * m_side.x) * m_moveSpeed;
			m_pCamera->m_lookY += (dy * m_up.y - dx * m_side.y) * m_moveSpeed;
			m_pCamera->m_lookZ += (dy * m_up.z - dx * m_side.z) * m_moveSpeed;
		}
	}

	m_preMouseXPos = arg.state.xPos;
	m_preMouseYPos = arg.state.yPos;

	return true;
}

bool CViewer::mousePressed(const MouseEvent &arg, int id)
{
	if (m_isPaintingMode)
	{
		CPaintPathes::Instance()->startNewPath();

		if (arg.state.button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouseDown = true;
		}
	}
	else
	{
		if (arg.state.button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouseDown = true;
		}
		else if (arg.state.button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			m_mouseDown = true;
			m_zoomMode = true;
		}

		m_startTumbling = true;
	}

	return true;
}

bool CViewer::mouseReleased(const MouseEvent &arg, int id)
{
	if (m_isPaintingMode)
	{
		CPaintPathes::Instance()->endPath();

		if (arg.state.button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouseDown = false;
		}
	}
	else
	{
		if (arg.state.button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_mouseDown = false;
		}
		else if (arg.state.button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			m_mouseDown = false;
			m_zoomMode = false;
		}

		m_mouseDown = false;

		m_startTumbling = true;
	}

	return true;
}

bool CViewer::mouseWheel(int vOffset)
{
	if (!m_isPaintingMode)
	{
		if (vOffset > 0)
		{
			m_pCamera->m_radius += vOffset * m_zoomSpeed;
			setSpeed(m_pCamera->m_radius * 0.1f, m_pCamera->m_radius * 0.002f, m_pCamera->m_radius * 0.02f);
		}
		else
		{
			float temp = m_pCamera->m_radius + vOffset * m_zoomSpeed;
			if (temp > 0.0f)
				m_pCamera->m_radius = temp;
			setSpeed(m_pCamera->m_radius * 0.1f, m_pCamera->m_radius * 0.002f, m_pCamera->m_radius * 0.02f);
		}
	}

	m_preWheelPos += vOffset;

	return true;
}