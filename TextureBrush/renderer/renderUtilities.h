#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{
	
enum RenderAxis
{
	X = 0, Y, Z
};

class CImage2D;
class CMaskImage2D;
class CCrossSectionBoundary;
class CBezierCircle;
class CSphereHarmonicCircle;

class CRenderUtilities
{
public:
	static double getTime();
	static void drawAxis();

	static void computePixelSize(CImage2D* pImg, float scale, vec2& pixelSize, vec2& imgCenter);

	static void showImage2D(CImage2D* pImg, float scale);

// 	static void showTextureByName(const std::string& vName, float scale = 1.0f);
// 	static void showTripleByName(const std::string& vName1, const std::string& vName2, const std::string& vName3); 
	static void grabScreen(unsigned int width, unsigned int height, unsigned int frame_num);

	static void drawWireRectangle(const vec2 &vCenter, const vec2 &vSize, int vPlane);
	static void drawSolidRectangle(const vec2 &vCenter, const vec2 &vSize, int vPlane);
	
	static void drawWireCircle(const vec2 &vCenter, const float vRadius, int vSegNum, int vPlane);
	static void drawSolidCircle(const vec2 &vCenter, const float vRadius, int vSegNum, int vPlane);

	static void drawWireCube(const vec3 &boxMin, const vec3 &boxMax);
	static void drawSolidCude(const vec3 &vCenter, const vec3 &vSize);

	static void drawWireSphere(int longReso, int letiReso, float redius);
	static void drawSolidSphere(int longReso, int letiReso, float redius);
};

};