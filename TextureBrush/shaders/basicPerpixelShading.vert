#version 430 core

uniform mat4 u_modelviewMatrix;
uniform mat4 u_projMatrix;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in float v_geoDis;

out vec3 f_origPos;
out vec4 f_posInEye;
out vec4 f_normal;
out float f_geoDis;

void main()
{
	f_geoDis = v_geoDis;
	f_origPos = v_position;
	f_posInEye = u_modelviewMatrix * vec4(v_position, 1.0);
    f_normal = transpose(inverse(u_modelviewMatrix)) * vec4(v_normal, 0.0); // Inverse normal only for Bunny & Horse

    gl_Position = u_projMatrix * u_modelviewMatrix * vec4(v_position, 1.0);
}
