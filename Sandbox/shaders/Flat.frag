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
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 vPosition;             // fragment position in world coordinates
in vec3 vNormal;

out vec4 fColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec4 ambient = light.ambient * material.ambient;

    vec3 lightDir = normalize(light.position - vPosition);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // diffuse
    vec3 norm = normalize(vNormal);
    vec4 diffuse = light.diffuse
        * max(dot(norm, lightDir), 0.0)
        * material.diffuse
        * intensity;

    // specular
    vec3 viewDir = normalize(viewPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * material.specular
        * intensity;

    float distance = length(light.position - vPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    fColor = (ambient + diffuse + specular) * attenuation;
}
