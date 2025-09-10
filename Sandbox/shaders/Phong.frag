#version 450 core

in vec3 v_Position;             // fragment position in world coordinates
in vec3 v_Normal;

out vec4 f_Color;

uniform vec4 u_LightColor;
uniform vec4 u_ObjectColor;

uniform vec3 u_LightPosition;   // light position in world coordinates
uniform vec3 u_ViewPosition;    // camera position in world coordinates

void main()
{
    float ambientStrength = 0.1;
    vec4 ambientColor = ambientStrength * u_LightColor;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    vec4 diffuseColor = max(dot(norm, lightDir), 0.0) * u_LightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec4 specularColor = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32) * u_LightColor;

    f_Color = (ambientColor + diffuseColor + specularColor) * u_ObjectColor;
}
