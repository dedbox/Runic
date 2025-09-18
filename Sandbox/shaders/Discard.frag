#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, vTexCoord);

    if (texColor.a < 0.1)
        discard;

    fColor = texColor;
}
