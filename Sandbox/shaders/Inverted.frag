#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D screenTexture;
uniform bool wireframe;

void main()
{
    if (wireframe)
        fColor = vec4(1.0);
    else {
        fColor = vec4(vec3(1.0 - texture(screenTexture, vTexCoord)), 1.0);
    }
}
