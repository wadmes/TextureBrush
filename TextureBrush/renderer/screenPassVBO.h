#pragma once

#include "../preHeader.h"
#include "vertexBufferObject.h"

namespace TextureSynthesis
{

class CScreenPassVBO : public CVertexBufferObject
{
public:
	CScreenPassVBO();
	virtual ~CScreenPassVBO();

public:
	void display();

private:
	void display(VBORenderMode mode){}
};

}