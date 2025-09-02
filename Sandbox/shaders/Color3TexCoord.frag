#version 450 core

layout (location = 0) in vec3 v_Color;
layout (location = 1) in vec2 v_TexCoord;

layout (location = 0) out vec4 color;

uniform sampler2D u_Texture0;

void main()
{
    color = texture(u_Texture0, v_TexCoord);
}
