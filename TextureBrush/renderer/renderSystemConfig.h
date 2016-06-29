#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

enum RenderSystemParameterType
{
	RSPT_WINDOWNAME = 0,
	RSPT_WINDOWSIZE,
	RSPT_CAMERA_MVIEW,
	RSPT_CAMERA_PROJ,
	RSPT_CAMERA_ADJUST,
	RSPT_MODEL_NAME,
	RSPT_TOTAL_NUMBER
};

class CRenderSystemConfig
{
public:
	static CRenderSystemConfig* getSysCfgInstance();
	virtual ~CRenderSystemConfig();

	void getWindowName(string& winName);
	void getWinSize(int &width, int &height);
	void getCameraMView(float &lookatX, float &lookatY, float &lookatZ, float &head, float &pitch, float &radius);
	void getCameraProj(float &fov, float &nearPlane, float &farPlane);
	void getCameraAdjust(float &tumblingSpeed, float &zoomSpeed, float &moveSpeed);

	void getModelName(string& modelName);

protected:
	CRenderSystemConfig();

	void loadConfig();
	void initConfig();
	void parseConfig(const std::string &cfgLine);

	string m_windowName;
	int m_winWidth, m_winHeight;

	float m_head, m_pitch, m_radius;
	float m_lookAtX, m_lookAtY, m_lookAtZ;
	float m_fov, m_nearPlane, m_farPlane;
	float m_tumblingSpeed, m_zoomSpeed, m_moveSpeed;

	string m_modelName;

	map<std::string, int> m_parameterTypeMap;
};

} // end namespace