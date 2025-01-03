#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 perspective_matrix;

out vec3 out_color;
out vec3 normal;

void main()
{
    out_color = vec3(0, 0, 0);
    gl_Position = perspective_matrix * view_matrix * model_matrix * vec4(aPos, 1);
}