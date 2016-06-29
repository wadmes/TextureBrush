#include "renderUtilities.h"

#include "renderSystemConfig.h"
#include "renderSystem.h"
#include "image2D.h"

#include "../renderer/Geo2D.h"

using namespace TextureSynthesis;

double CRenderUtilities::getTime()
{
	return glfwGetTime();
}

void CRenderUtilities::drawAxis()
{
	//draw axis.
	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);
	GLint width = viewport[2] / 16;
	GLint height = viewport[3] / 16;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//get the camera position and up vector from the modelview matrix.
	double campos[3] = {0.0 + 2.0f * modelview[2], 0.0 + 2.0f * modelview[6], 0.0 + 2.0f * modelview[10]};
	double up[3] = {modelview[1], modelview[5], modelview[9]};
	//set up the view matrix.
	gluLookAt(campos[X], campos[Y], campos[Z], 
		0.0, 0.0, 0.0, up[X], up[Y], up[Z]);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glPopMatrix();
}

void CRenderUtilities::computePixelSize(CImage2D* pImg, float scale, vec2& pixelSize, vec2& imgCenter)
{
	int imgWidth, imgHeight;
	pImg->getImgSize(imgHeight, imgWidth);

	int winWidth, winHeight;
	glfwGetWindowSize(CRenderSystem::Instance()->getGLFWWindow(), &winWidth, &winHeight);

	float imgWHRatio = (float)imgWidth / imgHeight;
	float winWHRatio = (float)winWidth / winHeight;
	float recWidth, recHeight;
	recWidth = recHeight = 2.0f * scale;
	if (imgWHRatio > winWHRatio)
	{
		recHeight /= imgWHRatio;
		recHeight *= winWHRatio;
	}
	else
	{
		recWidth *= imgWHRatio;
		recWidth /= winWHRatio;
	}

	pixelSize = vec2(recWidth / imgWidth, recHeight / imgHeight);
	imgCenter = vec2(imgWidth * 0.5f, imgHeight * 0.5f);
}

void CRenderUtilities::showImage2D(CImage2D* pImg, float scale)
{
	int imgWidth, imgHeight;
	pImg->getImgSize(imgHeight, imgWidth);

	vec2 pixelSize, imgCenter;
	computePixelSize(pImg, scale, pixelSize, imgCenter);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	for (int xPos = 0; xPos < imgWidth; ++xPos)
	{
		for (int yPos = 0; yPos < imgHeight; ++yPos)
		{
			vec3 pixelColor;
			pImg->getPixelData(xPos, yPos, pixelColor);
			vec2 pixelPos = vec2((xPos - imgCenter[0]) * pixelSize[0], (yPos - imgCenter[1]) * pixelSize[1]) + vec2(pixelSize[0] * 0.5f, pixelSize[1] * 0.5);

			glColor3f(pixelColor[0], pixelColor[1], pixelColor[2]);
			CRenderUtilities::drawSolidRectangle(pixelPos, pixelSize, 0);
		}
	}

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void CRenderUtilities::drawWireCube(const vec3 &boxMin, const vec3 &boxMax)
{
	float xstart = boxMin.x;
	float ystart = boxMin.y;
	float zstart = boxMin.z;
	float xend = boxMax.x;
	float yend = boxMax.y;
	float zend = boxMax.z;

	glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_LIGHTING);

	glLineWidth(2.0); 
	glColor3f(0.25f, 0.25f, 0.25f);

	glBegin(GL_LINE_LOOP);
	glVertex3f(xstart, ystart, zstart);
	glVertex3f(xstart, yend, zstart);
	glVertex3f(xstart, yend, zend);
	glVertex3f(xstart, ystart, zend);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(xend, ystart, zstart);
	glVertex3f(xend, yend, zstart);
	glVertex3f(xend, yend, zend);
	glVertex3f(xend, ystart, zend);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(xstart, ystart, zstart);
	glVertex3f(xend, ystart, zstart);
	glVertex3f(xstart, yend, zstart);
	glVertex3f(xend, yend, zstart);
	glVertex3f(xstart, yend, zend);
	glVertex3f(xend, yend, zend);
	glVertex3f(xstart, ystart, zend);
	glVertex3f(xend, ystart, zend);
	glEnd();

	glPopAttrib();
}

void CRenderUtilities::drawSolidCude(const vec3 &vCenter, const vec3 &vSize)
{
	float xstart = vCenter.x - 0.5f * vSize.x;
	float ystart = vCenter.y - 0.5f * vSize.y;
	float zstart = vCenter.z - 0.5f * vSize.z;
	float xend = vCenter.x + 0.5f * vSize.x;
	float yend = vCenter.y + 0.5f * vSize.y;
	float zend = vCenter.z + 0.5f * vSize.z;

	glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLES);
	
	glVertex3f(xend, yend, zend);
	glVertex3f(xend, yend, zstart);
	glVertex3f(xstart, yend, zstart);

	glVertex3f(xend, yend, zend);
	glVertex3f(xstart, yend, zstart);
	glVertex3f(xstart, yend, zend);

	glVertex3f(xend, ystart, zend);
	glVertex3f(xend, ystart, zstart);
	glVertex3f(xstart, ystart, zstart);

	glVertex3f(xend, ystart, zend);
	glVertex3f(xstart, ystart, zstart);
	glVertex3f(xstart, ystart, zend);

	glVertex3f(xend, yend, zend);
	glVertex3f(xstart, yend, zend);
	glVertex3f(xstart, ystart, zend);

	glVertex3f(xend, yend, zend);
	glVertex3f(xstart, ystart, zend);
	glVertex3f(xend, ystart, zend);

	glVertex3f(xend, yend, zstart);
	glVertex3f(xstart, yend, zstart);
	glVertex3f(xstart, ystart, zstart);

	glVertex3f(xend, yend, zstart);
	glVertex3f(xstart, ystart, zstart);
	glVertex3f(xend, ystart, zstart);

	glVertex3f(xend, yend, zstart);
	glVertex3f(xend, yend, zend);
	glVertex3f(xend, ystart, zstart);

	glVertex3f(xend, ystart, zstart);
	glVertex3f(xend, yend, zend);
	glVertex3f(xend, ystart, zend);

	glVertex3f(xstart, yend, zstart);
	glVertex3f(xstart, yend, zend);
	glVertex3f(xstart, ystart, zstart);

	glVertex3f(xstart, ystart, zstart);
	glVertex3f(xstart, yend, zend);
	glVertex3f(xstart, ystart, zend);

	glEnd();

	glPopAttrib();
}

// void CRenderUtilities::showTextureByName(const std::string& vName, float scale)
// {
// 	int imgWidth, imgHeight;
// 	COGLTexture* pOGLTex = CTextureManager::getTexManagerInstance()->getTextureByName(vName);
// 	pOGLTex->getTexSize(imgWidth, imgHeight);
// 
// 	int winWidth, winHeight;
// 	CSystemConfig::getSysCfgInstance()->getWinSize(winWidth, winHeight);
// 	
// 	float imgWHRatio = (float)imgWidth / imgHeight;
// 	float winWHRatio = (float)winWidth / winHeight;
// 	float recWidth, recHeight;
// 	recWidth = recHeight = 1.0;
// 	if (imgWHRatio > winWHRatio)
// 	{
// 		recHeight /= imgWHRatio;
// 		recHeight *= winWHRatio;
// 	}
// 	else
// 	{
// 		recWidth *= imgWHRatio;
// 		recWidth /= winWHRatio;
// 	}
// 
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	int width, height;
// 	glfwGetWindowSize(&width, &height);
// 	height = height < 1 ? 1 : height;
// 	glViewport(0, 0, width, height);
// 
// 	glMatrixMode(GL_PROJECTION);
// 	glPushMatrix();
// 	glLoadIdentity();
// 
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// 	glLoadIdentity();
// 
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE0, pOGLTex->getGLTexHandle());
// 
// 	glBegin(GL_QUADS);
// 	glTexCoord2f(0.0, 0.0);
// 	glVertex2f(-recWidth, -recHeight);
// 
// 	glTexCoord2f(1.0, 0.0);
// 	glVertex2f(recWidth, -recHeight);
// 
// 	glTexCoord2f(1.0, 1.0);
// 	glVertex2f(recWidth, recHeight);
// 
// 	glTexCoord2f(0.0, 1.0);
// 	glVertex2f(-recWidth, recHeight);
// 	glEnd();
// 
// 	glBindTexture(GL_TEXTURE0, 0);
// 	glDisable(GL_TEXTURE_2D);
// 
// 	glPopMatrix();
// 
// 	glMatrixMode(GL_MODELVIEW);
// 	glPopMatrix();
// }
// 
// void CRenderUtilities::showTripleByName(const std::string& vName1, const std::string& vName2, const std::string& vName3)
// {
// 	int imgWidth1, imgHeight1;
// 	COGLTexture* pOGLTex1 = CTextureManager::getTexManagerInstance()->getTextureByName(vName1);
// 	pOGLTex1->getTexSize(imgWidth1, imgHeight1);
// 
// 	int imgWidth2, imgHeight2;
// 	COGLTexture* pOGLTex2 = CTextureManager::getTexManagerInstance()->getTextureByName(vName2);
// 	pOGLTex2->getTexSize(imgWidth2, imgHeight2);
// 
// 	int imgWidth3, imgHeight3;
// 	COGLTexture* pOGLTex3 = CTextureManager::getTexManagerInstance()->getTextureByName(vName3);
// 	pOGLTex3->getTexSize(imgWidth3, imgHeight3);
// 
// 	glEnable(GL_TEXTURE_2D);
// 	
// 	float halfWidth = 1.0, halfHeight = halfWidth * imgHeight1 / imgWidth1;
// 	glBindTexture(GL_TEXTURE0, pOGLTex1->getGLTexHandle());
// 	glBegin(GL_QUADS);
// 	glTexCoord2f(0.0, 0.0);
// 	glVertex3f(-halfWidth, -halfHeight, 0.0);
// 
// 	glTexCoord2f(1.0, 0.0);
// 	glVertex3f(halfWidth, -halfHeight, 0.0);
// 
// 	glTexCoord2f(1.0, 1.0);
// 	glVertex3f(halfWidth, halfHeight, 0.0);
// 
// 	glTexCoord2f(0.0, 1.0);
// 	glVertex3f(-halfWidth, halfHeight, 0.0);
// 	glEnd();
// 
// 	/*halfWidth = imgWidth2 / imgWidth1;
// 	halfHeight = halfWidth * imgHeight2 / imgWidth2;
// 
// 	glBindTexture(GL_TEXTURE0, pOGLTex1->getGLTexHandle());*/
// 	glBegin(GL_QUADS);
// 	glTexCoord2f(0.0, 0.0);
// 	glVertex3f(-halfWidth, 0.0, -halfHeight);
// 
// 	glTexCoord2f(1.0, 0.0);
// 	glVertex3f(halfWidth, 0.0, -halfHeight);
// 
// 	glTexCoord2f(1.0, 1.0);
// 	glVertex3f(halfWidth, 0.0, halfHeight);
// 
// 	glTexCoord2f(0.0, 1.0);
// 	glVertex3f(-halfWidth, 0.0, halfHeight);
// 	glEnd();
// 
// 	/*halfWidth = imgWidth3 / imgWidth1;
// 	halfHeight = halfWidth * imgHeight3 / imgWidth3;
// 
// 	glBindTexture(GL_TEXTURE0, pOGLTex3->getGLTexHandle());*/
// 	glBegin(GL_QUADS);
// 	glTexCoord2f(0.0, 0.0);
// 	glVertex3f(0.0, -halfWidth, -halfHeight);
// 
// 	glTexCoord2f(1.0, 0.0);
// 	glVertex3f(0.0, halfWidth, -halfHeight);
// 
// 	glTexCoord2f(1.0, 1.0);
// 	glVertex3f(0.0, halfWidth, halfHeight);
// 
// 	glTexCoord2f(0.0, 1.0);
// 	glVertex3f(0.0, -halfWidth, halfHeight);
// 	glEnd();
// 
// 	glBindTexture(GL_TEXTURE0, 0);
// 	glDisable(GL_TEXTURE_2D);
// }

void CRenderUtilities::grabScreen(unsigned int width, unsigned int height, unsigned int frame_num)
{
// 	unsigned char* bitmapData = new unsigned char[3 * width * height];
// 
// 	glPixelStorei(GL_PACK_ALIGNMENT, 1);
// 	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapData);
// 	
// 	char anim_filename[256];
// 	sprintf_s(anim_filename, 256, "output/result_%04d.jpg", frame_num);
// 
// 	cv::Mat TempMat = cv::Mat(height, width, CV_8UC3, bitmapData);
// 	cv::flip(TempMat, TempMat, 0);
// 	cv::imwrite(anim_filename, TempMat);
// 
// 	delete [] bitmapData;
}

// FIX : only considered XOY plane.
void CRenderUtilities::drawWireRectangle(const vec2 &vCenter, const vec2 &vSize, int vPlane)
{
	glm::vec2 halfSize = vSize * 0.5f;

	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] + halfSize[1], 0.0f);
	glVertex3f(vCenter[0] - halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	glVertex3f(vCenter[0] - halfSize[0], vCenter[1] + halfSize[1], 0.0f);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	glEnd();
}
void CRenderUtilities::drawSolidRectangle(const vec2 &vCenter, const vec2 &vSize, int vPlane)
{
	glm::vec2 halfSize = vSize * 0.5f;
	//std::cout << "Rectangle: [" << vCenter[0] << " " << vCenter[1] << "; " << vSize[0] << " " << vSize[0] << "]" << std::endl;
	glBegin(GL_TRIANGLES);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] + halfSize[1], 0.0f);
	glVertex3f(vCenter[0] - halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	
	glVertex3f(vCenter[0] - halfSize[0], vCenter[1] - halfSize[1], 0.0f);
	glVertex3f(vCenter[0] + halfSize[0], vCenter[1] + halfSize[1], 0.0f);
	glVertex3f(vCenter[0] - halfSize[0], vCenter[1] + halfSize[1], 0.0f);
	glEnd();
}

void CRenderUtilities::drawWireCircle(const vec2 &vCenter, const float vRadius, int vSegNum, int vPlane)
{
	float angleStep = 3.14159265359f * 2.0f / vSegNum;
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	for (int idx = 0; idx <= vSegNum; ++idx)
	{
		glVertex3f(vCenter[0] + vRadius * cos(angleStep * idx), vCenter[1] + vRadius * sin(angleStep * idx), 0.0f);
	}
	glEnd();
}
void CRenderUtilities::drawSolidCircle(const vec2 &vCenter, const float vRadius, int vSegNum, int vPlane)
{
	float angleStep = 3.14159265359f * 2.0f / vSegNum;
	glLineWidth(2.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(vCenter[0], vCenter[1], 0.0);
	for (int idx = 0; idx <= vSegNum; ++idx)
	{
		glVertex3f(vCenter[0] + vRadius * cos(angleStep * idx), vCenter[1] + vRadius * sin(angleStep * idx), 0.0f);
	}
	glEnd();
}