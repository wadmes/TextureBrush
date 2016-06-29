#include "commonUtility.h"

using namespace TextureSynthesis;

char* CCommonUtility::loadFile(const char* filename, int& filesize)
{
	FILE *fptr; 
	char *buf;

	fptr = fopen(filename, "rb");
	if (!fptr)
	{
		printf("Fail to load the file %s\n", filename);
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	filesize = ftell(fptr);
	buf = (char*)malloc(filesize+1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, filesize, 1, fptr);
	fclose(fptr);
	buf[filesize] = '\0';

	return buf;
}

char* CCommonUtility::loadFile(const char* filename)
{
	int len;
	return loadFile(filename, len);
}