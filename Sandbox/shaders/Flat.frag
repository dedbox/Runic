#version 450 core

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight
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

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

in vec3 vPosition;
in vec3 vNormal;

out vec4 fColor;

uniform vec3 viewPosition;
uniform Material material;

uniform DirectionalLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

void main()
{
    vec3 normal  = normalize(vNormal);
    vec3 viewDir = normalize(viewPosition - vPosition);

    fColor = calculateDirectionalLight(dirLight, normal, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        fColor += calculatePointLight(pointLights[i], normal, viewDir);

    fColor += calculateSpotLight(spotLight, normal, viewDir);
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir   = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec4 ambient  = light.ambient * material.ambient;
        vec4 diffuse  = light.diffuse * max(dot(normal, lightDir), 0.0) * material.diffuse;
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * material.specular;

    return ambient + diffuse + specular;
}

vec4 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir   = normalize(light.position - vPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec4 ambient  = light.ambient * material.diffuse;
    vec4 diffuse  = light.diffuse * max(dot(normal, lightDir), 0.0) * material.diffuse;
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * material.specular;

    float distance    = length(light.position - vPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return vec4(vec3(ambient + diffuse + specular) * attenuation, 1.0);
}

vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir   = normalize(light.position - vPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec4 ambient = light.ambient * material.diffuse;
    vec4 diffuse = light.diffuse
        * max(dot(normal, lightDir), 0.0)
        * material.diffuse
        * intensity;
    vec4 specular = light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
        * material.specular
        * intensity;

    float distance    = length(light.position - vPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}
