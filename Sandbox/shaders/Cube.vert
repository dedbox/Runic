#version 450 core

in vec3 a_Position;
in vec3 a_Normal;

out vec3 v_Position;            // vertex position in world coordinates
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = a_Normal;
}
