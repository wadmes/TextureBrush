#pragma once

#include "../preHeader.h"
#include "textureKernelDefs.h"

namespace TextureSynthesis
{

class CGLTexture;
class CPixelBufferObject
{
public:
	CPixelBufferObject(TexelFormat vTexelFmt, TexelType vTexelType, int vWidth, int vHeight);
	virtual ~CPixelBufferObject();

	uint* getDataPointer(GLuint texId);

private:
	void copyData(void* pPboData);

private:
	TexelFormat m_texelFmt;
	TexelType m_texelType;
	int m_width, m_height;
	GLuint m_pboId;
	uint* m_pData;
};

}