#include "pixelBufferObject.h"
#include "glTexture.h"

using namespace TextureSynthesis;

CPixelBufferObject::CPixelBufferObject(TexelFormat vTexelFmt, TexelType vTexelType, int vWidth, int vHeight) : 
m_texelFmt(vTexelFmt), m_texelType(vTexelType), m_width(vWidth), m_height(vHeight)
{
	glGenBuffers(1, &m_pboId);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pboId);
	glBufferData(GL_PIXEL_PACK_BUFFER, vWidth * vHeight * CTexturePropMap::s_fmt2ChannelNum[vTexelFmt]
		* CTexturePropMap::s_type2ByteNum[vTexelType], NULL, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	m_pData = new uint[vWidth * vHeight];
}

CPixelBufferObject::~CPixelBufferObject()
{
	glDeleteBuffers(1, &m_pboId);
}

uint* CPixelBufferObject::getDataPointer(GLuint texId)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBindTexture(GL_TEXTURE_2D, texId);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pboId);
	// FIX: Fixed texture format and texel type!
	glReadPixels(0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

	void *retPointer = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

	copyData(retPointer);

	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	return m_pData;
}

void CPixelBufferObject::copyData(void* pPboData)
{
	uint* pData = (uint*)(pPboData);

	//cout << endl;
	for (int hIdx = 0; hIdx < m_height; ++hIdx)
	{
		//cout << endl;
		for (int vIdx = 0; vIdx < m_width; ++vIdx)
		{
			m_pData[hIdx * m_width + vIdx] = pData[hIdx * m_width + vIdx];
			/*if (m_pData[hIdx * m_width + vIdx] > 0)
			{
			cout << "0";
			}
			else
			cout << "1";*/
		}
	}
	//cout << endl;
}