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
        fColor = texture(screenTexture, vTexCoord);
        float avg = (fColor.r + fColor.g + fColor.b) / 3.0;
        fColor = vec4(vec3(avg), 1.0);
    }
}
