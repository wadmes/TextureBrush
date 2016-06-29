#pragma once

#include "../preHeader.h"
#include "triangleMesh.h"
#include "sceneElementDefs.h"

namespace TextureSynthesis
{

class CSphereGeometry : public CTriangleMesh
{
public:
	CSphereGeometry(int vLongReso, int vLatiReso);
	virtual ~CSphereGeometry();

private:
	void initGeometry();

private:
	int m_longReso;
	int m_latiReso;
};

}