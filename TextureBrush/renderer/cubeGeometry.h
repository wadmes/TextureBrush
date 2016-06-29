#pragma once

#include "../preHeader.h"
#include "triangleMesh.h"
#include "sceneElementDefs.h"

namespace TextureSynthesis
{

	class CCubeGeometry : public CTriangleMesh
	{
	public:
		CCubeGeometry();
		virtual ~CCubeGeometry();

	private:
		void initGeometry();
	};

}