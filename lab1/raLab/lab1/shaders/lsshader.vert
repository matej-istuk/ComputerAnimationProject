#version 330 core
layout (location = 0) in vec4 aPos;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 perspective_matrix;
uniform vec3 color;

out vec3 vert_color;

void main()
{
    vert_color = color;
    gl_Position = perspective_matrix * view_matrix * model_matrix * aPos;
}