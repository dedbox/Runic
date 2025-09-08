#version 450 core

out vec4 f_Color;

uniform vec4 u_LightColor;

void main()
{
    f_Color = u_LightColor;
}
