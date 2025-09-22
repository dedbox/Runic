#version 450 core

in vec3 vTexCoord;

out vec4 fColor;

uniform samplerCube skyBox;

void main()
{
    fColor = texture(skyBox, vTexCoord);
}
