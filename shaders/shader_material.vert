#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNorm;
out vec3 normal;
out vec3 position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 perspective_matrix;

void main()
{
    vec4 vert4 = model_matrix * aPos;
    vec3 vert = vert4.xyz / vert4.w;

    normal = normalize(mat3(transpose(inverse(model_matrix))) * aNorm.xyz);
    position = vert;

    gl_Position = perspective_matrix * view_matrix * vert4;
}