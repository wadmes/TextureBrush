#include "renderSystemConfig.h"

using namespace TextureSynthesis;

CRenderSystemConfig::CRenderSystemConfig()
{
	m_parameterTypeMap["WindowName"] = RSPT_WINDOWNAME;
	m_parameterTypeMap["WindowSize"] = RSPT_WINDOWSIZE;
	m_parameterTypeMap["CameraMView"] = RSPT_CAMERA_MVIEW;
	m_parameterTypeMap["CameraProj"] = RSPT_CAMERA_PROJ;
	m_parameterTypeMap["CameraAdjust"] = RSPT_CAMERA_ADJUST;
	m_parameterTypeMap["ModelName"] = RSPT_MODEL_NAME;

	initConfig();
	loadConfig();
}

CRenderSystemConfig::~CRenderSystemConfig()
{
	std::cout<<"Release: System config object released!"<<std::endl;
}

void CRenderSystemConfig::loadConfig()
{
	std::fstream cfgFile;
	cfgFile.open("system.cfg");
	if (cfgFile.is_open())
	{
		char lineBuf[256];
		while (cfgFile.getline(lineBuf, 256))
		{
			parseConfig(lineBuf);
		}
	}
	else
	{
		std::cout<<"ERROR: Can't load config file!"<<std::endl;
		exit(EXIT_FAILURE);
	}

	cfgFile.close();
}

void CRenderSystemConfig::initConfig()
{
	m_winWidth = 1024; m_winHeight = 800;

	m_head = 0.0f; m_pitch = 0.0f; m_radius = 10.0f;
	m_lookAtX = m_lookAtY = m_lookAtZ = 0.0f;
	m_fov = 60.0f; m_nearPlane = 0.0001f; m_farPlane = 10000.0f;
	m_tumblingSpeed = 0.5f; m_zoomSpeed = 0.2f;	m_moveSpeed = 0.05f;
}

void CRenderSystemConfig::parseConfig(const std::string &cfgLine)
{
	namespace qi = boost::spirit::qi;

	// get parameter type
	int itrParaType = cfgLine.find('=');
	if (std::string::npos == itrParaType)
	{
		std::cout<<"WARNING: Invalid config syntax!"<<std::endl;
	}
	else
	{
		std::string::const_iterator beginItr = cfgLine.begin() + itrParaType + 2;
		std::string::const_iterator endItr = cfgLine.end();
		switch (m_parameterTypeMap[std::string(cfgLine.c_str(),  cfgLine.c_str() + itrParaType - 1)])
		{
		case RSPT_WINDOWNAME:
			{
				m_windowName = string(beginItr, endItr);
			}
			break;
		case RSPT_WINDOWSIZE:
			{
				qi::parse(beginItr, endItr, qi::int_>>' '>>qi::int_, m_winWidth, m_winHeight);
			}
			break;
		case RSPT_CAMERA_MVIEW:
			{
				qi::parse(beginItr, endItr, 
					qi::double_>>' '>>qi::double_>>' '>>qi::double_>>' '>>qi::double_>>' '>>qi::double_>>' '>>qi::double_,
					m_lookAtX, m_lookAtY, m_lookAtZ, m_head, m_pitch, m_radius);
			}
			break;
		case RSPT_CAMERA_PROJ:
			{
				qi::parse(beginItr, endItr, qi::double_>>' '>>qi::double_>>' '>>qi::double_, m_fov, m_nearPlane, m_farPlane);
			}
			break;
		case RSPT_CAMERA_ADJUST:
			{
				qi::parse(beginItr, endItr, qi::double_>>' '>>qi::double_>>' '>>qi::double_, m_tumblingSpeed, m_zoomSpeed, m_moveSpeed);
			}
			break;
		case RSPT_MODEL_NAME:
			{
				m_modelName = string(beginItr, endItr);
			}
			break;
		default:
			std::cout<<"WARNING: Not existing parameter!"<<std::endl;
			break;
		}
	}
}

CRenderSystemConfig* CRenderSystemConfig::getSysCfgInstance()
{
	static CRenderSystemConfig s_sysCfg;

	return &s_sysCfg;
}

void CRenderSystemConfig::getWindowName(string& winName)
{
	winName = m_windowName;
}

void CRenderSystemConfig::getWinSize(int &width, int &height)
{
	width = m_winWidth;
	height = m_winHeight;
}

void CRenderSystemConfig::getCameraMView(float &lookatX, float &lookatY, float &lookatZ, float &head, float &pitch, float &radius)
{
	lookatX = m_lookAtX;
	lookatY = m_lookAtY;
	lookatZ = m_lookAtZ;
	head = m_head;
	pitch = m_pitch;
	radius = m_radius;
}

void CRenderSystemConfig::getCameraProj(float &fov, float &nearPlane, float &farPlane)
{
	fov = m_fov;
	nearPlane = m_nearPlane;
	farPlane = m_farPlane;
}

void CRenderSystemConfig::getCameraAdjust(float &tumblingSpeed, float &zoomSpeed, float &moveSpeed)
{
	tumblingSpeed = m_tumblingSpeed;
	zoomSpeed = m_zoomSpeed;
	moveSpeed = m_moveSpeed;
}

void CRenderSystemConfig::getModelName(string& modelName)
{
	modelName = m_modelName;
}