#pragma once

#include "../preHeader.h"

namespace TextureSynthesis
{

#define MAX_MATERIALS			64
/// Number of texture types. It is restricted by material flag array. See MaterialProperties.
#define NUM_TEX_TYPES			2

enum LightType
{
	Light_Point = 0,
	Light_Directional,
	Light_AreaDisc,
	Light_AreaRect
};

// enum TextureType
// {
// 	Tex_Diffuse = 0,
// 	Tex_Bump
// };

struct BasicLight
{
	BasicLight(): type(Light_Point), position(0.0f), color(0.0f), direction(0.0f){}

	LightType type;
	vec3 position;
	vec3 color;
	vec3 direction;
};

struct BasicMaterial
{
	/// Whether this is an area light material.
	bool isAreaLight;
	/// Diffuse material color.
	vec3 colorDiffuse;
	/// Specular material color.
	vec3 colorSpec;
	/// Specular exponent (shininess).
	float specExp;
	/// Transparency alpha (opaque = 1, completely transparent = 0).
	float transAlpha;
	/// Index of refraction of the material.
	float indexRefrac;
	/// Whether this material has textures.
	bool hasTex[NUM_TEX_TYPES];
	/// Texture paths.
	std::string strTexPath[NUM_TEX_TYPES];
};

} // end namespace