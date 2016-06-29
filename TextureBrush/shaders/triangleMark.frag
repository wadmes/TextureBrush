#version 430 core

in vec3 f_origPos;
in float f_triMark;

layout(location = 0) out vec4 out_Color;

void main()
{
	if (f_triMark < 0.5)
		out_Color = vec4(1.0, 0.0, 0.0, 1.0);
	else
		out_Color = vec4(0.0, 0.0, 1.0, 1.0);
}
