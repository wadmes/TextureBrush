#version 430 core

uniform mat4 u_modelviewMatrix;
uniform mat4 u_projMatrix;

layout(location = 0) in vec3 v_position;
layout(location = 1) in float v_triMark;

out vec3 f_origPos;
out float f_triMark;

void main()
{
	f_triMark = v_triMark;
	f_origPos = v_position;

    gl_Position = u_projMatrix * u_modelviewMatrix * vec4(v_position, 1.0);
}
