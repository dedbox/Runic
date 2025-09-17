#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D texture1;

void main()
{
    fColor = texture(texture1, vTexCoord);
    // fColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
