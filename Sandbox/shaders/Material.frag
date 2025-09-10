#version 450 core

in vec3 v_Position;             // fragment position in world coordinates
in vec3 v_Normal;

out vec4 f_Color;

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
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_ViewPosition;

void main()
{
    // // ambient
    vec4 ambient = u_Light.ambient * u_Material.ambient;

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_Position);
    vec4 diffuse = u_Light.diffuse * max(dot(norm, lightDir), 0.0) * u_Material.diffuse;

    // specular
    vec3 viewDir = normalize(u_ViewPosition - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec4 specular = u_Light.specular
        * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess)
        * u_Material.specular;

    f_Color = ambient + diffuse + specular;
}
