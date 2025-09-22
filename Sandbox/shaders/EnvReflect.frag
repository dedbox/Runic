#version 450 core

in vec3 vPosition;
in vec3 vNormal;

out vec4 fColor;

uniform vec3 cameraPosition;
uniform samplerCube skyBox;

void main()
{
    vec3 I = normalize(vPosition - cameraPosition);
    vec3 R = reflect(I, normalize(vNormal));
    fColor = vec4(texture(skyBox, R).rgb, 1.0);
}
