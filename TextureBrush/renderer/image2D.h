#pragma once

#include "../preHeader.h"
#include "textureKernelDefs.h"

namespace TextureSynthesis
{

class CImage2D
{
	union ImageDataPointer
	{
		unsigned char *pDataUC;
		unsigned short *pDataUS;
		unsigned int *pDataUI;

		char *pDataSC;
		short *pDataSS;
		int *pDataSI;

		float *pDataSF;
		double *pDataDF;
	};

public:
	CImage2D();
	CImage2D(int vRow, int vCol, TexelFormat vFmt, TexelType vType, void* pData = NULL);
	virtual ~CImage2D();

	ImageDataPointer getDataPointer(){ return m_dataPointer; }

	void load(const std::string& fileName, TexelFormat vFmt = TEXELFMT_RGB, TexelType vType = TEXELTYPE_USIGNED_BYTE);
	void reload();

	// Getter functions
	TexelFormat getTexelFmt(){ return m_texelFmt; }
	TexelType getTexelType(){ return m_texelType; }

	int getRowNum(){ return m_rows; }
	int getColNum(){ return m_cols; }
	void getImgSize(int& rows, int& cols){ rows = m_rows; cols = m_cols; }
	ivec2 getImgSize(){ return ivec2(m_cols, m_rows); }
	int getChannelNum(){ return m_channels; }

	void* getData(){ return m_dataPointer.pDataSF; }

	// Here the coordinate increase from down-left to up-right, which agree with OpenGL texture coordinate
	template<typename T> T getPixelData(int xPos, int yPos)
	{
		assert(xPos >= 0 && xPos < m_cols && yPos >= 0 && yPos < m_rows);

		T* pData = (T*)(m_dataPointer.pDataSF);
		return pData[yPos * m_cols + xPos];
	}

	template<typename T> T getPixelData(const ivec2& vPos)
	{
		assert(vPos[0] >= 0 && vPos[0] < m_cols && vPos[1] >= 0 && vPos[1] < m_rows);

		T* pData = (T*)(m_dataPointer.pDataSF);
		return pData[vPos[1] * m_cols + vPos[0]];
	}

	template<typename T> void setPixelData(int xPos, int yPos, T value)
	{
		assert(xPos >= 0 && xPos < m_cols && yPos >= 0 && yPos < m_rows);

		T* pData = (T*)(m_dataPointer.pDataSF);
		pData[yPos * m_cols + xPos] = value;
	}

	template<typename T> void setPixelData(const ivec2& vPos, int yPos, T value)
	{
		assert(vPos[0] >= 0 && vPos[0] < m_cols && vPos[1] >= 0 && vPos[1] < m_rows);

		T* pData = (T*)(m_dataPointer.pDataSF);
		pData[vPos[1] * m_cols + vPos[0]] = value;
	}

	void getPixelData(int xPos, int yPos, vec3 &pixelColor);
	void setPixelData(int xPos, int yPos, const vec3 pixel);

	void getPixelData(const ivec2& vPos, vec3 &pixelColor);
	void setPixelData(const ivec2& vPos, const vec3 pixel);

protected:
	void allocateMemory();
	virtual void copyImage(int imgHandle);
	virtual void releaseData();

	virtual void internalProcess(){}

	void getILImgProps(int imgHandle);
	void convert2ILTexcoord(int xPos, int yPos, int& rowIdx, int& colIdx);

	int pixelOffset(int xPos, int yPos);

protected:
	TexelFormat m_texelFmt;
	TexelType m_texelType;

	int m_rows;
	int m_cols;
	int m_channels;

	ImageDataPointer m_dataPointer;
};

// vec3 CImage2D::getPixelData<vec3>(int xPos, int yPos);
// vec3 CImage2D::getPixelData<vec3>(const ivec2& vPos);

} // End of namespace