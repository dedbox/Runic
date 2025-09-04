#version 450 core

layout (location = 0) in vec2 v_TexCoord;

layout (location = 0) out vec4 color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
    color = mix(texture(u_Texture0, v_TexCoord), texture(u_Texture1, v_TexCoord), 0.2);
}
