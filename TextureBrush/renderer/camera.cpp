#include "camera.h"

#include "renderSystemConfig.h"

using namespace TextureSynthesis;

CCamera::CCamera()
{
	CRenderSystemConfig::getSysCfgInstance()->getCameraMView(m_lookX, m_lookY, m_lookZ, m_head, m_pitch, m_radius);
	CRenderSystemConfig::getSysCfgInstance()->getCameraProj(m_fov, m_nearPlane, m_farPlane);

	m_focal.x = m_lookX;
	m_focal.y = m_lookY;
	m_focal.z = m_lookZ;
};