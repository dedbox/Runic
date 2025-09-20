#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D screenTexture;
uniform bool wireframe;

void main()
{
    if (wireframe)
        fColor = vec4(1.0);
    else
        fColor = texture(screenTexture, vTexCoord);
}
