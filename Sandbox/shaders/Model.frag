#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D texture_diffuse1;

void main()
{
    fColor = texture(texture_diffuse1, vTexCoord);
}
