#pragma once

#include "preHeader.h"

namespace TextureSynthesis
{

class CCommonUtility
{
public:
	static char* loadFile(const char* filename);
	static char* loadFile(const char* filename, int& filesize);
};

} // end namespace 