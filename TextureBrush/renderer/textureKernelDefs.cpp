#include "textureKernelDefs.h"

using namespace TextureSynthesis;

std::map<std::string, TextureType>::value_type str2TexTypeValues[] =
{
	std::map<std::string, TextureType>::value_type("TEXTYPE_1D", TEXTYPE_1D),
	std::map<std::string, TextureType>::value_type("TEXTYPE_2D", TEXTYPE_2D),
	std::map<std::string, TextureType>::value_type("TEXTYPE_3D", TEXTYPE_3D),
	
	std::map<std::string, TextureType>::value_type("TEXTYPE_RECTANGLE", TEXTYPE_RECTANGLE),
	std::map<std::string, TextureType>::value_type("TEXTYPE_TEXTURE_BUFFER", TEXTYPE_TEXTURE_BUFFER),
	std::map<std::string, TextureType>::value_type("TEXTYPE_CUBE_MAP", TEXTYPE_CUBE_MAP),
	
	std::map<std::string, TextureType>::value_type("TEXTYPE_1D_ARRAY", TEXTYPE_1D_ARRAY),
	std::map<std::string, TextureType>::value_type("TEXTYPE_2D_ARRAY", TEXTYPE_2D_ARRAY),
	std::map<std::string, TextureType>::value_type("TEXTYPE_CUBE_MAP_ARRAY", TEXTYPE_CUBE_MAP_ARRAY),

	std::map<std::string, TextureType>::value_type("TEXTYPE_2D_MULTISAMPLE", TEXTYPE_2D_MULTISAMPLE),
	std::map<std::string, TextureType>::value_type("TEXTYPE_2D_MULTISAMPLE_ARRAY", TEXTYPE_2D_MULTISAMPLE_ARRAY)
};

std::map<std::string, TexelFormat>::value_type str2TexelFmtValues[] =
{
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_COLOR_INDEX", TEXELFMT_COLOR_INDEX),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_ALPHA", TEXELFMT_ALPHA),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_RGB", TEXELFMT_RGB),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_BGR", TEXELFMT_BGR),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_RGBA", TEXELFMT_RGBA),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_BGRA", TEXELFMT_BGRA),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_LUMILLANCE", TEXELFMT_LUMILLANCE),
	std::map<std::string, TexelFormat>::value_type("TEXELFMT_LUMILLANCE_ALPHA", TEXELFMT_LUMILLANCE_ALPHA)
};

std::map<std::string, TexelType>::value_type str2TexelTypeValues[] =
{
	std::map<std::string, TexelType>::value_type("TEXELTYPE_BYTE", TEXELTYPE_BYTE),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_USIGNED_BYTE", TEXELTYPE_USIGNED_BYTE),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_SHORT", TEXELTYPE_SHORT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_USIGNED_SHORT", TEXELTYPE_USIGNED_SHORT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_INT", TEXELTYPE_INT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_USIGNED_INT", TEXELTYPE_USIGNED_INT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_HALF_FLOAT", TEXELTYPE_HALF_FLOAT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_FLOAT", TEXELTYPE_FLOAT),
	std::map<std::string, TexelType>::value_type("TEXELTYPE_DOUBLE", TEXELTYPE_DOUBLE)
};

std::map<TexelFormat, int>::value_type texFmt2ChannelNumValues[] =
{
	std::map<TexelFormat, int>::value_type(TEXELFMT_COLOR_INDEX, 1),
	std::map<TexelFormat, int>::value_type(TEXELFMT_ALPHA, 1),
	std::map<TexelFormat, int>::value_type(TEXELFMT_RGB, 3),
	std::map<TexelFormat, int>::value_type(TEXELFMT_BGR, 3),
	std::map<TexelFormat, int>::value_type(TEXELFMT_RGBA, 4),
	std::map<TexelFormat, int>::value_type(TEXELFMT_BGRA, 4),
	std::map<TexelFormat, int>::value_type(TEXELFMT_LUMILLANCE, 1),
	std::map<TexelFormat, int>::value_type(TEXELFMT_LUMILLANCE_ALPHA, 1)
};

std::map<TexelType, int>::value_type str2TexelByteNumValues[] =
{
	std::map<TexelType, int>::value_type(TEXELTYPE_BYTE, 1),
	std::map<TexelType, int>::value_type(TEXELTYPE_USIGNED_BYTE, 1),
	std::map<TexelType, int>::value_type(TEXELTYPE_SHORT, 2),
	std::map<TexelType, int>::value_type(TEXELTYPE_USIGNED_SHORT, 2),
	std::map<TexelType, int>::value_type(TEXELTYPE_INT, 4),
	std::map<TexelType, int>::value_type(TEXELTYPE_USIGNED_INT, 4),
	std::map<TexelType, int>::value_type(TEXELTYPE_HALF_FLOAT, 2),
	std::map<TexelType, int>::value_type(TEXELTYPE_FLOAT, 4),
	std::map<TexelType, int>::value_type(TEXELTYPE_DOUBLE, 8)
};

std::map<std::string, TextureType> CTexturePropMap::s_str2TexType(str2TexTypeValues, str2TexTypeValues + 11);
std::map<std::string, TexelFormat> CTexturePropMap::s_str2TexelFmt(str2TexelFmtValues, str2TexelFmtValues + 8);
std::map<TexelFormat, int> CTexturePropMap::s_fmt2ChannelNum(texFmt2ChannelNumValues, texFmt2ChannelNumValues + 8);
std::map<std::string, TexelType> CTexturePropMap::s_str2TexelType(str2TexelTypeValues, str2TexelTypeValues + 9);
std::map<TexelType, int> CTexturePropMap::s_type2ByteNum(str2TexelByteNumValues, str2TexelByteNumValues + 9);