#version 450 core

out vec4 Color;

uniform vec4 light_color;

void main()
{
    Color = light_color;
}
