#version 450 core

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Light
{
    vec3 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec3 vPosition;             // fragment position in world coordinates
in vec3 vNormal;

out vec4 fColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

void main()
{
    // ambient
    vec4 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(-light.direction);
    vec4 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * material.specular;

    fColor = ambient + diffuse + specular;
}
