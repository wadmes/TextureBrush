#version 430 core

layout(location = 0) out uvec4 out_Color;

void main()
{
	out_Color = uvec4(gl_PrimitiveID + 1, 0, 0, 0);
}
