#pragma once

#include "../preHeader.h"
#include "textureKernelDefs.h"

namespace TextureSynthesis
{

class CImage2D;
class CGLTexture
{
public:
	CGLTexture(CImage2D* pImg);
	CGLTexture(TextureType vTexType, TexelFormat vTexelFmt, TexelType vTexelType, int vWidth, int vHeight, int vDepth = 1, bool genData = false);

	virtual ~CGLTexture();

	GLuint getGLTexHandle();
	void getTexSize(int &vTexWidth, int &vTexHeight);
	void getTexSize(int &vTexWidth, int &vTexHeight, int &vTexDepth);

	bool isDataOwned(){ return m_dataOwned; }
	
	void saveToImage(const std::string& vImgName);

	void printTextureInfo();

private:
	void createGLTexture();
	void releaseGLTexResource();

private:
	TextureType m_textureType;
	TexelFormat m_texelFmt;
	TexelType m_texelType;
	int m_texWidth, m_texHeight, m_texDepth;
	int m_channelNum;

	CImage2D* m_pImg;
	bool m_dataOwned;

	bool m_isGLTexReady;
	bool m_isDataUpdated;
	GLuint m_glTexHandle;
};

}