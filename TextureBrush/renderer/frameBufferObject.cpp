#include "frameBufferObject.h"

using namespace TextureSynthesis;

CFrameBufferObject::CFrameBufferObject(int width, int height, int colorTexNum): m_bufWidth(width), 
m_bufHeight(height), m_colorTextureNum(colorTexNum), m_curAttachIdx(0)
{
	genFrameBufferObject();

	checkFBOStatus();
}

CFrameBufferObject::~CFrameBufferObject()
{
	glDeleteTextures(m_colorTextureNum, m_colorTex);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteFramebuffers(1, &m_frameBuffer);
}

void CFrameBufferObject::attachRenderTexture(GLuint texId, int attachId)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachId, GL_TEXTURE_2D, texId, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferObject::genTextureAndAttach(TexelFormat vFmt, TexelType vType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenTextures(1, &m_colorTex[m_curAttachIdx]);
	glBindTexture(GL_TEXTURE_2D, m_colorTex[m_curAttachIdx]);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_bufWidth, m_bufHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_curAttachIdx, GL_TEXTURE_2D, m_colorTex[m_curAttachIdx], 0);

	cout << "\t New texture with ID " << m_colorTex[m_curAttachIdx] << " attached to " << m_curAttachIdx << " of frame buffer " << m_frameBuffer << endl;
	
	++m_curAttachIdx;
	++m_colorTextureNum;

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferObject::checkFBOStatus()
{
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR: Frame buffer incomplete!" << endl;
		exit(-1);
	}
}

void CFrameBufferObject::activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CFrameBufferObject::deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferObject::genFrameBufferObject()
{
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	cout << "Info: New frame buffer generated with ID " << m_frameBuffer << endl;

	// Generate depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_bufWidth, m_bufHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	// Generate texture objects
	if (m_colorTextureNum != 0)
	{
		glGenTextures(m_colorTextureNum, m_colorTex);
		for (int idx = 0; idx < m_colorTextureNum; ++idx)
		{
			//glActiveTexture(GL_TEXTURE0 + idx);
			glBindTexture(GL_TEXTURE_2D, m_colorTex[idx]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bufWidth, m_bufHeight, 0, GL_RGBA, GL_FLOAT, NULL);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_curAttachIdx, GL_TEXTURE_2D, m_colorTex[idx], 0);
			++m_curAttachIdx;
			cout << "\t Default texture with ID " << m_colorTex[idx] << " attached to frame buffer " << m_frameBuffer << endl;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferObject::genColorTextures()
{
	glGenTextures(m_colorTextureNum, m_colorTex);

	for (int idx = 0; idx < m_colorTextureNum; ++idx)
	{
		glBindTexture(GL_TEXTURE_2D, m_colorTex[idx]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bufWidth, m_bufHeight, 0, GL_RGBA, GL_FLOAT, NULL);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, m_colorTex[idx]);
	}
}