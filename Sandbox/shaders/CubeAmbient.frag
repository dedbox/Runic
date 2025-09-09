#version 450 core

in vec3 v_Normal;

out vec4 f_Color;

uniform vec4 u_LightColor;
uniform vec4 u_ObjectColor;

void main()
{
    float ambientStrength = 0.1;
    vec4 ambientColor = ambientStrength * u_LightColor;

    f_Color = ambientColor * u_ObjectColor;
}
