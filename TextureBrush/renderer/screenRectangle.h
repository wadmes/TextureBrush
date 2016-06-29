#pragma once

#include "../preHeader.h"
#include "triangleMesh.h"

namespace TextureSynthesis
{

class CScreenRectangle : public CTriangleMesh
{
public:
	static CScreenRectangle* getInstance();
	virtual ~CScreenRectangle();

	void display();

protected:
	CScreenRectangle();
	void initData();
};

}