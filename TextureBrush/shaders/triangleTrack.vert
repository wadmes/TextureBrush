#version 430 core

uniform mat4 u_modelviewMatrix;
uniform mat4 u_projMatrix;

layout(location = 0) in vec3 v_position;
layout(location = 1) in float v_mark;

void main()
{
    gl_Position = u_projMatrix * u_modelviewMatrix * vec4(v_position, 1.0);
}
