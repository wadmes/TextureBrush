#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

class CCamera
{
public:
	CCamera();

	void setLookAt(const float& x, const float& y, const float& z) {m_lookX = x; m_lookY = y; m_lookZ = z;}
	void setRadius(const float& r) {m_radius = r;}

	void setNearAndFar(const float& n, const float& f) { m_nearPlane = n; m_farPlane = f; }

	void setFieldOfView(const float& fov) { m_fov = fov; }

	void resetLookAt(){m_lookX = m_focal.x; m_lookY = m_focal.y; m_lookZ = m_focal.z;}
	void setFocal(const float& x, const float& y, const float& z) {m_focal.x = x; m_focal.y = y; m_focal.z = z;}

public:
	float m_fov, m_nearPlane, m_farPlane;

	float m_head, m_pitch, m_radius,
		  m_lookX, m_lookY, m_lookZ;

	vec3 m_focal;
};

} // end namespace