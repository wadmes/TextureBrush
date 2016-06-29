#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

//////////////////////////////////////////////////////////////////////////
// Recently, I just take vertex, normal and facet idx into consideration
//////////////////////////////////////////////////////////////////////////

class CTriangleMesh;

enum VBOBufferMask
{
	VBOBM_VERTEX = 0x1,
	VBOBM_TRIIDX = 0x2,
	VBOBM_NORMAL = 0x4,

	VBOBM_COLOR = 0x8,
	VBOBM_TEXCOORD = 0x10,
	VBOBM_TANGENT = 0x20,

	VBOBM_FLOAT_PROP = 0x40,
	VBOBM_FLOAT2_PROP = 0x80,
	VBOBM_FLOAT3_PROP = 0x100,
	VBOBM_FLOAT4_PROP = 0x200,

	VBOBM_INT_PROP = 0x400,
	VBOBM_UINT_PROP = 0x800,

	VBOBM_BASICTRIANGLE = VBOBM_VERTEX | VBOBM_TRIIDX,
	VBOBM_SCREENRECTANGLE = VBOBM_BASICTRIANGLE | VBOBM_TEXCOORD,
	VBOBM_NORMALTRIANGLE = VBOBM_BASICTRIANGLE | VBOBM_NORMAL,
	VBOBM_NORMALTEXCOORDTRIANGLE = VBOBM_NORMALTRIANGLE | VBOBM_TEXCOORD
};

enum VBOIndex
{
	VBOIDX_VERTEX = 0,
	VBOIDX_NORMAL,
	VBOIDX_COLOR,
	VBOIDX_TEXCOORD,
	VBOIDX_TRIIDX,
	VBOIDX_TANGENT,
	VBOIDX_PROPFLOAT,
	VBOIDX_PROPFLOAT2,
	VBOIDX_PROPFLOAT3,
	VBOIDX_PROPFLOAT4,
	VBOIDX_PROPINT,
	VBOIDX_PROPUINT,
	VBOIDX_TOTALIDXNUM
};

enum VBORenderMode
{
	VBORM_POINTS = GL_POINTS,
	VBORM_LINES = GL_LINE_LOOP,
	VBO_LINE_LOOP = GL_LINE_LOOP,
	VBORM_LINE_STRIP = GL_LINE_STRIP,
	VBORM_TRIANGLES = GL_TRIANGLES,
	VBORM_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	VBORM_TRIANGLE_FAN = GL_TRIANGLE_FAN,
	VBORM_QUADS = GL_QUADS,
	VBORM_QUAD_STRIP = GL_QUAD_STRIP,
	VBORM_POLYGON = GL_POLYGON
};

class CVertexBufferObject
{
public:
	CVertexBufferObject(CTriangleMesh* pScene, GLuint bufMask = VBOBM_NORMALTRIANGLE);
	virtual ~CVertexBufferObject();

public:
	void activeBuffer(int vIdx);
	void deactiveBuffer(int vIdx);

	void display(VBORenderMode mode);
	void resetScene(CTriangleMesh* pScene);

	void updateBuffer(int bufMask);

protected:
	virtual void setup();
	void clean();

protected:
	CTriangleMesh* m_pGeometry;
	int m_verNum;
	int m_triNum;

	GLuint m_totalBufNumber;
	GLuint m_bufferMask;
	GLuint m_VAO;
	GLuint m_buffers[VBOIDX_TOTALIDXNUM];
	int m_bufferAttachPoints[VBOIDX_TOTALIDXNUM];
	GLboolean m_activeState[VBOIDX_TOTALIDXNUM];
};

} // end namespace