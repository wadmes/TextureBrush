#version 430 core

uniform sampler2D u_sourceTex;
//LIQILIQI
uniform int is_line;

in vec3 f_origPos;
in vec4 f_posInEye;
in vec4 f_normal;
in float f_geoDis;

vec3 lightPos = vec3(0.0, 0.0, 100.0);
vec3 lightAmbi = vec3(0.2, 0.2, 0.2);
vec3 lightDiff = vec3(0.8, 0.8, 0.8);
vec3 lightSpec = vec3(0.8, 0.8, 0.8);

vec3 myColor = vec3(0.9, 0.9, 0.9);

layout(location = 0) out vec4 out_Color;

void main()
{
	vec3 normal = f_normal.xyz;
	normal = normalize(normal);

	vec3 lightDir = normalize(lightPos - f_posInEye.xyz);
	vec3 eyeDir = normalize(-f_posInEye.xyz);
	vec3 halfDir = normalize(eyeDir + lightDir);

	vec3 finalColor = myColor * lightAmbi;

	float diffuseTerm = dot(normal, lightDir);
	if (diffuseTerm > 0.0)
	{
		finalColor += diffuseTerm * lightDiff * myColor;
	}

	float specFactor = dot(halfDir, normal);
	if (specFactor > 0.0)
	{
		specFactor = pow(specFactor, 150);
		finalColor += specFactor * lightSpec * vec3(0.8, 0.8, 0.8);
	}

	//out_Color = vec4(normal, 1.0);
	float scale = 0.25;
	//out_Color = vec4(f_triIdx * scale, f_triIdx * scale, f_triIdx * scale, 1.0);
	float chColor = fract(f_geoDis * 30.0) < 0.1 ? (1.0) : f_geoDis * scale;
	
	out_Color = vec4(chColor, 0, 0, 1.0);
	//out_Color = vec4(finalColor.xyz, 1.0);

	if (is_line == 1) {
		out_Color = vec4(0, 0.5, 0, 0.5);
	}
}
