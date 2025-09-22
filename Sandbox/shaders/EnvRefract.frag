#version 450 core

in vec3 vPosition;
in vec3 vNormal;

out vec4 fColor;

uniform vec3 cameraPosition;
uniform samplerCube skyBox;

void main()
{
    float ratio = 1.00 / 1.52;  // air to glass
    vec3 I = normalize(vPosition - cameraPosition);
    vec3 R = refract(I, normalize(vNormal), ratio);
    fColor = vec4(texture(skyBox, R).rgb, 1.0);
}
