#version 450 core

layout (location = 0) out vec4 f_Color;

uniform vec4 u_LightColor;
uniform vec4 u_ObjectColor;

void main()
{
    f_Color = u_LightColor * u_ObjectColor;
}
