#pragma once

#include "../preHeader.h"
#include "sceneElementDefs.h"

namespace TextureSynthesis
{

enum TriMeshShadeMode
{
	TMSM_FLAT = GL_FLAT,
	TMSM_SMOOTH = GL_SMOOTH
};

class CTriangleMesh
{
public:
	// Default constructor. Use Load() to load a scene.
	CTriangleMesh(TriMeshShadeMode shadeMode = TMSM_SMOOTH);
	virtual ~CTriangleMesh(void);

	void setVertex(vec3* pVertices, int vNum);
	void setIndices(ivec3* pIndices, int vNum);

	void setVerNormal(vec3* pNormals, int vNum);
	void setColors(vec3* pColors, int vNum);
	void setTexcoords(vec3* pTexcoords, int vNum);

	void setPropFloat(float *pPropData, int vNum);
	void setPropFloat2(vec2 *pPropData, int vNum);
	void setPropFloat3(vec3 *pPropData, int vNum);
	void setPropFloat4(vec4 *pPropData, int vNum);

	void setPropInt(int *pPropData, int vNum);
	void setPropUInt(uint *pPropData, int vNum);

	void setTangent(vec3* pTangents, int vNum);
	void setBiTangent(vec3* pBiTangents, int vNum);

public:
	void load(const std::string& strFile);
	void unload();

	const bool isLoaded() { return m_numVers != 0; }
	const std::string getSourceFile() { return m_strSceneFile; }

	int getMatNum() { return m_vecMaterials.size(); }
	const BasicMaterial& getMaterial(size_t idx) { return m_vecMaterials.at(idx); }
	int getVerNum() { return m_numVers; }
	int getTriNum() { return m_numTris; }
	vec3 getBoundMin() { return m_boundMin; }
	vec3 getBoundMax() { return m_boundMax; }
	vec3* getVertices()  { return m_v; }
	vec3* getNormals() { return m_n; }
	vec3* getTangent() { return m_tangent; }
	vec3* getBiTangent() { return m_biTangent; }
	ivec3* getTriIdx() { return m_i; }
	int* getMaterialIndices() { return m_idxMaterial; }
	vec3* getTextureCoords() { return m_texCoords; }

	float* getPropFloatData(){ return m_propFloat; }
	vec2* getPropFloat2Data(){ return m_propFloat2; }
	vec3* getPropFloat3Data(){ return m_propFloat3; }
	vec4* getPropFloat4Data(){ return m_propFloat4; }

	int* getPropIntData(){ return m_propInt; }
	uint* getPropUIntData(){ return m_propUInt; }

	void windTriVertexIdxOrder();
	void normalize();
	void computeBoundingBox();
	void computeFlatNormal();
	void computeSmoothNormal();

private:
	void checkMatch(int vNum);
	void postProcess();

	void copyData(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum);
	void copyData4FlatMesh(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum);
	void copyData4SmoothMesh(const vec3 *pVertexData, int verNum, const int *pFacetVerNumData, int facetNum, int **pFacetVerIdxData, int triNum);
	void copyVertexData();
	void copyFacetData();

// Attributes
protected:
	std::string m_strSceneFile;

	const TriMeshShadeMode m_shadeMode;

	int m_numVers;
	int m_numTris;

	vec3 m_boundMin;
	vec3 m_boundMax;

	std::vector<BasicMaterial> m_vecMaterials;

	// Vertex array
	vec3* m_v;
	// Vertex normal array, smooth or flat
	vec3* m_n;
	// Vertex's indices array of triangles
	ivec3* m_i;
	// Material index array. One per triangle.
	int* m_idxMaterial;
	// Texture coordinate arrays
	// So at most three-dimensional textures are supported. 
	vec3* m_texCoords;

	// Tangent and Bitangent
	vec3* m_tangent;
	vec3* m_biTangent;

	// Attached properties
	float *m_propFloat;
	vec2 *m_propFloat2;
	vec3 *m_propFloat3;
	vec4 *m_propFloat4;

	int *m_propInt;
	uint *m_propUInt;
};

} // end namespace