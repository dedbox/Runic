#version 450 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float linearize(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near  - z * (far - near));
}

void main()
{
    // fColor = texture(texture1, vTexCoord);

    float depth = linearize(gl_FragCoord.z) / far;

    fColor = vec4(vec3(depth), 1.0);
}
