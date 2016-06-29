#include "image2D.h"

using namespace TextureSynthesis;

CImage2D::CImage2D() : m_rows(0), m_cols(0), m_channels(3), m_texelFmt(TEXELFMT_RGB), m_texelType(TEXELTYPE_USIGNED_BYTE)
{
	m_channels = CTexturePropMap::s_fmt2ChannelNum[m_texelFmt];
	m_dataPointer.pDataUC = NULL;
}

CImage2D::CImage2D(int vRow, int vCol, TexelFormat vFmt, TexelType vType, void* pData) : 
m_rows(vRow), m_cols(vCol), m_texelFmt(vFmt), m_texelType(vType)
{
	m_channels = CTexturePropMap::s_fmt2ChannelNum[vFmt];
	m_dataPointer.pDataUC = NULL;

	allocateMemory();
}

CImage2D::~CImage2D()
{
	releaseData();
}

void CImage2D::load(const std::string& fileName, TexelFormat vFmt, TexelType vType)
{
	ILuint imgHandle;

	ILenum Error;
	while ((Error = ilGetError()) != IL_NO_ERROR) {
		printf_s("%d: %s\n", Error, iluErrorString(Error));
	}

	ilGenImages(1, &imgHandle);
	ilBindImage(imgHandle);

	ILboolean loaded = ilLoadImage(fileName.c_str());

	printf("IL_INVALID_EXTENSION=%d IL_COULD_NOT_OPEN_FILE=%d\n", IL_INVALID_EXTENSION, IL_COULD_NOT_OPEN_FILE);
	while ((Error = ilGetError()) != IL_NO_ERROR) {
		printf_s("%d: %s\n", Error, iluErrorString(Error));
	}

	if (!loaded)
	{
		cout << "ERROR: Image loading error with name " << fileName << "." << endl;
		exit(-1);
	}

	copyImage(imgHandle);

	ilDeleteImage(imgHandle);

	internalProcess();
}

void CImage2D::getPixelData(int xPos, int yPos, vec3 &pixelColor)
{
	assert(xPos >= 0 && xPos < m_cols && yPos >= 0 && yPos < m_rows);

	int offset = pixelOffset(xPos, yPos);

	pixelColor = vec3(m_dataPointer.pDataSF[offset], m_dataPointer.pDataSF[offset + 1], m_dataPointer.pDataSF[offset + 2]);
}

void CImage2D::setPixelData(int xPos, int yPos, const vec3 pixel)
{
	assert(xPos >= 0 && xPos < m_cols && yPos >= 0 && yPos < m_rows);

	int offset = pixelOffset(xPos, yPos);

	m_dataPointer.pDataSF[offset] = pixel[0];
	m_dataPointer.pDataSF[offset + 1] = pixel[1];
	m_dataPointer.pDataSF[offset + 2] = pixel[2];
}

void CImage2D::getPixelData(const ivec2& vPos, vec3 &pixelColor)
{
	getPixelData(vPos[0], vPos[1], pixelColor);
}

void CImage2D::setPixelData(const ivec2& vPos, const vec3 pixel)
{
	setPixelData(vPos[0], vPos[1], pixel);
}

void CImage2D::allocateMemory()
{
	switch (m_texelType)
	{
	case TEXELTYPE_BYTE:
		m_dataPointer.pDataSC = new char[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_USIGNED_BYTE:
		m_dataPointer.pDataUC = new unsigned char[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_SHORT:
		m_dataPointer.pDataSS = new short[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_USIGNED_SHORT:
		m_dataPointer.pDataUS = new unsigned short[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_INT:
		m_dataPointer.pDataSI = new int[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_USIGNED_INT:
		m_dataPointer.pDataUI = new unsigned int[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_HALF_FLOAT:
		cout << "WARNING: Half float texel is not supported! It has been cast to float!" << endl;
		m_dataPointer.pDataSF = new float[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_FLOAT:
		m_dataPointer.pDataSF = new float[m_rows * m_cols * m_channels];
		break;
	case TEXELTYPE_DOUBLE:
		m_dataPointer.pDataDF = new double[m_rows * m_cols * m_channels];
		break;
	}
}

void CImage2D::copyImage(int imgHandle)
{
	ilBindImage(imgHandle);

	m_rows = ilGetInteger(IL_IMAGE_HEIGHT);
	m_cols = ilGetInteger(IL_IMAGE_WIDTH);

	allocateMemory();

	ilCopyPixels(0, 0, 0, m_cols, m_rows, 1, m_texelFmt, m_texelType, m_dataPointer.pDataSF);
}

void CImage2D::releaseData()
{
	switch (m_texelType)
	{
	case TEXELTYPE_BYTE:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataSC);
		break;
	case TEXELTYPE_USIGNED_BYTE:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataUC);
		break;
	case TEXELTYPE_SHORT:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataSS);
		break;
	case TEXELTYPE_USIGNED_SHORT:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataUS);
		break;
	case TEXELTYPE_INT:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataSI);
		break;
	case TEXELTYPE_USIGNED_INT:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataUI);
		break;
	case TEXELTYPE_HALF_FLOAT:
		cout << "WARNING: Half float texel is not supported! It has been cast to float!" << endl;
		SAFE_DELETE_ARRAY(m_dataPointer.pDataSF);
		break;
	case TEXELTYPE_FLOAT:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataSF);
		break;
	case TEXELTYPE_DOUBLE:
		SAFE_DELETE_ARRAY(m_dataPointer.pDataDF);
		break;
	}
}

void CImage2D::getILImgProps(int imgHandle)
{
	ilBindImage(imgHandle);

	m_rows = ilGetInteger(IL_IMAGE_HEIGHT);
	m_cols = ilGetInteger(IL_IMAGE_WIDTH);
}

void CImage2D::convert2ILTexcoord(int xPos, int yPos, int& rowIdx, int& colIdx)
{
	rowIdx = yPos;
	colIdx = xPos;
}

int CImage2D::pixelOffset(int xPos, int yPos)
{
	return (yPos * m_cols + xPos) * m_channels;
}