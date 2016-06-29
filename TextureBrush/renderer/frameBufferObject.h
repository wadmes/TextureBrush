#pragma once

#include "../preHeader.h"
#include "textureKernelDefs.h"

namespace TextureSynthesis
{

class CFrameBufferObject
{
public:
	CFrameBufferObject(int width, int height, int colorTexNum);
	virtual ~CFrameBufferObject();

	GLuint getDepthBuffer(){ return m_depthBuffer; }
	GLuint getColorBuffer(int idx){ return m_colorTex[idx]; }

	void attachRenderTexture(GLuint texId, int attachId);

	void genTextureAndAttach(TexelFormat vFmt, TexelType vType);

	void activate();
	void deactivate();

private:
	void genFrameBufferObject();
	void genColorTextures();

	void checkFBOStatus();

private:
	GLuint m_frameBuffer;
	GLuint m_depthBuffer;
	GLuint m_colorTex[16];

	int m_curAttachIdx;
	int m_colorTextureNum;
	int m_bufWidth, m_bufHeight;
};

}