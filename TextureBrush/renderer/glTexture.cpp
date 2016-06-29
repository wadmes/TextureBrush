#include "glTexture.h"
#include "image2D.h"

using namespace TextureSynthesis;

CGLTexture::CGLTexture(CImage2D* pImg) : m_textureType(TEXTYPE_2D), m_pImg(pImg), m_dataOwned(false)
{
	m_isGLTexReady = false;
	m_isDataUpdated = false;

	m_texelFmt = m_pImg->getTexelFmt();
	m_texelType = m_pImg->getTexelType();

	m_texWidth = m_pImg->getColNum();
	m_texHeight = m_pImg->getRowNum();
	m_texDepth = 1;

	m_channelNum = m_pImg->getChannelNum();

	printTextureInfo();
}

CGLTexture::CGLTexture(TextureType vTexType, TexelFormat vTexelFmt, TexelType vTexelType, int vWidth, int vHeight, int vDepth/* = 1*/, bool genData/* = false*/) :
	m_textureType(vTexType), m_texelFmt(m_texelFmt), m_texelType(vTexelType),
	m_texWidth(vWidth), m_texHeight(vHeight), m_texDepth(vDepth), m_dataOwned(false), m_pImg(NULL)
{
	if (vTexType != TEXTYPE_2D)
	{
		cout << "ERROR: Only 2D texture is supported now!" << endl;
		exit(-1);
	}

	m_isGLTexReady = false;
	m_isDataUpdated = false;

	if (genData)
	{
		m_dataOwned = true;
		m_pImg = new CImage2D(vHeight, vWidth, vTexelFmt, vTexelType);
	}

	m_channelNum = CTexturePropMap::s_fmt2ChannelNum[vTexelFmt];

	printTextureInfo();
}

CGLTexture::~CGLTexture()
{
	releaseGLTexResource();
}

GLuint CGLTexture::getGLTexHandle()
{
	if (!m_isGLTexReady)
	{
		createGLTexture();
		m_isGLTexReady = true;
	}
	
	return m_glTexHandle;
}

void CGLTexture::getTexSize(int &vTexWidth, int &vTexHeight)
{
	vTexWidth = m_texWidth;
	vTexHeight = m_texHeight;
}

void CGLTexture::getTexSize(int &vTexWidth, int &vTexHeight, int &vTexDepth)
{
	vTexWidth = m_texWidth;
	vTexHeight = m_texHeight;
	vTexDepth = m_texDepth;
}

void CGLTexture::printTextureInfo()
{
	std::cout<<"SUCCESS: New OGLTexture generated"<<std::endl
		<<"\tSize = "<<m_texWidth<<" * "<<m_texHeight<<" * "<<m_texDepth<<std::endl
		<<"\tChannel = "<<m_channelNum<<std::endl;
}

void CGLTexture::createGLTexture()
{
	// ARTHUR_FIX: Only process 2D texture.
	if (TEXTYPE_2D == m_textureType)
	{
		glGenTextures(1, &m_glTexHandle);
		glBindTexture(GL_TEXTURE_2D, m_glTexHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (m_pImg != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_texelFmt, m_texWidth, m_texHeight, 0, m_texelFmt, m_texelType, m_pImg->getData());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_texelFmt, m_texWidth, m_texHeight, 0, m_texelFmt, m_texelType, NULL);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (TEXTYPE_3D == m_textureType)
	{
		glGenTextures(1, &m_glTexHandle);
		glBindTexture(GL_TEXTURE_3D, m_glTexHandle);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (m_pImg != NULL)
		{
			glTexImage2D(GL_TEXTURE_3D, 0, m_texelFmt, m_texWidth, m_texHeight, m_texDepth, m_texelFmt, m_texelType, m_pImg->getData());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_3D, 0, m_texelFmt, m_texWidth, m_texHeight, m_texDepth, m_texelFmt, m_texelType, NULL);
		}

		glBindTexture(GL_TEXTURE_3D, 0);
	}
	else
	{
		std::cout<<"ERROR: GL texture generation failed. Only support 2D/3D texture!"<<std::endl;
		exit(-1);
	}

	std::cout<<"SUCCESS: OpenGL texture created (texture handle = "<<m_glTexHandle<<")."<<std::endl;
}

void CGLTexture::releaseGLTexResource()
{
	if (m_dataOwned)
	{
		SAFE_DELETE(m_pImg);
	}

	if (glIsTexture(m_glTexHandle))
	{
		glDeleteTextures(1, &m_glTexHandle);
	}
}