#include "vertexBufferObject.h"
#include "triangleMesh.h"

using namespace TextureSynthesis;

CVertexBufferObject::CVertexBufferObject(CTriangleMesh* pScene, GLuint bufMask) : m_pGeometry(pScene), m_bufferMask(bufMask), m_totalBufNumber(0)
{
	setup();
}

CVertexBufferObject::~CVertexBufferObject()
{
	clean();
}

void CVertexBufferObject::setup()
{
	for (int vIdx = 0; vIdx < VBOIDX_TOTALIDXNUM; ++vIdx)
	{
		m_buffers[vIdx] = 0;
		m_bufferAttachPoints[vIdx] = -1;
		m_activeState[vIdx] = false;
	}

	m_verNum = m_pGeometry->getVerNum();
	m_triNum = m_pGeometry->getTriNum();

	assert(m_verNum != 0 && m_triNum != 0);

	m_VAO = 0;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(VBOIDX_TOTALIDXNUM, m_buffers);

	cout << "Info: New vertex buffer array generated with Id " << m_VAO << endl;

	//NEW

	//glm::vec3 v = glm::vec3(1, 1, 1); 
	//m_pGeometry->setVertex(&v, 1);

	updateBuffer(m_bufferMask);

	glBindVertexArray(0);
}

void CVertexBufferObject::updateBuffer(int bufMask)
{
	glBindVertexArray(m_VAO);

	if (bufMask & VBOBM_VERTEX)
	{
		if (m_pGeometry->getVertices() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_VERTEX] < 0)
			{
				cout << "\t New vertex buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_VERTEX] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_VERTEX]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * 3 * sizeof(float), m_pGeometry->getVertices(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_VERTEX]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_VERTEX], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "ERROR: Buffer setup error, no vertex exists!" << endl;
			exit(-1);
		}
	}

	if (bufMask & VBOBM_NORMAL)
	{
		if (m_pGeometry->getNormals() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_NORMAL] < 0)
			{
				cout << "\t New normal buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_NORMAL] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_NORMAL]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * 3 * sizeof(float), m_pGeometry->getNormals(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_NORMAL]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_NORMAL], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no normal data exist!" << endl;
		}
	}

	if (m_bufferMask & VBOBM_TEXCOORD)
	{
		if (m_pGeometry->getTextureCoords() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_TEXCOORD] < 0)
			{
				cout << "\t New texcoord buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_TEXCOORD] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_TEXCOORD]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * 3 * sizeof(float), m_pGeometry->getTextureCoords(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_TEXCOORD]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_TEXCOORD], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no normal data exist!" << endl;
		}
	}

	if (m_bufferMask & VBOBM_TANGENT)
	{
		if (m_pGeometry->getTangent() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_TANGENT] < 0)
			{
				cout << "\t New tangent buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_TANGENT] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_TANGENT]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * 3 * sizeof(float), m_pGeometry->getTangent(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_TANGENT]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_TANGENT], 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no tangent data exist!" << endl;
		}
	}

	if (bufMask & VBOBM_FLOAT_PROP)
	{
		if (m_pGeometry->getPropFloatData() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_PROPFLOAT] < 0)
			{
				cout << "\t New float property buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_PROPFLOAT] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_PROPFLOAT]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * sizeof(float), m_pGeometry->getPropFloatData(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_PROPFLOAT]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_PROPFLOAT], 1, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no triangle indices data exist!" << endl;
		}
	}

	if (bufMask & VBOBM_FLOAT2_PROP)
	{
		if (m_pGeometry->getPropFloatData() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_PROPFLOAT2] < 0)
			{
				cout << "\t New float2 property buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_PROPFLOAT2] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_PROPFLOAT2]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * sizeof(float), m_pGeometry->getPropFloat2Data(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_PROPFLOAT2]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_PROPFLOAT2], 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no triangle indices data exist!" << endl;
		}
	}

	if (bufMask & VBOBM_INT_PROP)
	{
		if (m_pGeometry->getPropIntData() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_PROPINT] < 0)
			{
				cout << "\t New int property buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_PROPINT] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_PROPINT]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * sizeof(int), m_pGeometry->getPropIntData(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_PROPINT]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_PROPINT], 1, GL_INT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no triangle indices data exist!" << endl;
		}
	}

	if (bufMask & VBOBM_UINT_PROP)
	{
		if (m_pGeometry->getPropUIntData() != NULL)
		{
			if (m_bufferAttachPoints[VBOIDX_PROPUINT] < 0)
			{
				cout << "\t New unsigned int property buffer bounded to attachment point " << m_totalBufNumber << endl;
				m_bufferAttachPoints[VBOIDX_PROPUINT] = m_totalBufNumber;
				m_totalBufNumber++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBOIDX_PROPUINT]);
			glBufferData(GL_ARRAY_BUFFER, m_verNum * sizeof(uint), m_pGeometry->getPropUIntData(), GL_STATIC_DRAW_ARB);
			glEnableVertexAttribArray(m_bufferAttachPoints[VBOIDX_PROPUINT]);
			glVertexAttribPointer(m_bufferAttachPoints[VBOIDX_PROPUINT], 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no triangle indices data exist!" << endl;
		}
	}

	if (bufMask & VBOBM_TRIIDX)
	{
		if (m_pGeometry->getTriIdx() != NULL)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VBOIDX_TRIIDX]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triNum * 3 * sizeof(int), m_pGeometry->getTriIdx(), GL_STATIC_DRAW);
		}
		else
		{
			cout << "WARNING: Buffer setup error, no triangle indices data exist!" << endl;
		}
	}

	glBindVertexArray(0);
}

void CVertexBufferObject::clean()
{
	m_verNum = m_triNum = 0;
	m_pGeometry = NULL;
	for (int vIdx = 0; vIdx < VBOIDX_TOTALIDXNUM; ++vIdx)
	{
		if (m_buffers[vIdx] != 0)
		{
			glDeleteBuffers(1, &m_buffers[vIdx]);
			m_buffers[vIdx] = 0;
		}
		m_activeState[vIdx] = false;
	}
	glDeleteVertexArrays(1, &m_VAO);
}

void CVertexBufferObject::display(VBORenderMode mode)
{
	glBindVertexArray(m_VAO);

	glDrawElementsBaseVertex(mode, m_triNum * 3, GL_UNSIGNED_INT, (void*)0, 0);

	glBindVertexArray(0);
}

void CVertexBufferObject::resetScene(CTriangleMesh* pScene)
{
	clean();

	m_pGeometry = pScene;

	setup();
}

void CVertexBufferObject::activeBuffer(int vIdx)
{
	if (m_buffers[vIdx] != 0)
	{
		m_activeState[vIdx] = true;
	}
	else
	{
		std::cout << "WARNING: Activation failed. Target buffer hasn't be initialized!" << std::endl;
	}
}

void CVertexBufferObject::deactiveBuffer(int vIdx)
{
	if (m_buffers[vIdx] != 0)
	{
		m_activeState[vIdx] = false;
	}
	else
	{
		std::cout << "WARNING: Deactivation failed. Target buffer hasn't be initialized!" << std::endl;
	}
}