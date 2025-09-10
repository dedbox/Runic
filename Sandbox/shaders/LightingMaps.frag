#version 450 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

void main()
{
    // ambient
    vec4 ambient = light.ambient * texture(material.diffuse, vTexCoord);

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vPosition);
    vec4 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * texture(material.diffuse, vTexCoord);

    // specular
    vec3 viewDir = normalize(viewPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * texture(material.specular, vTexCoord);

    fColor = ambient + diffuse + specular;
}
