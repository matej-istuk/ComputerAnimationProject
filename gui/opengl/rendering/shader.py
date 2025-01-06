from ctypes import create_string_buffer

from OpenGL.GL import glUniformMatrix4fv, glUniform1i, glUniform1f, glUniform3f, glUniform4f, glUniform3fv, glUniform4fv
from OpenGL.GL.shaders import compileShader, compileProgram, glDeleteShader, GL_TRUE
from pyglet.gl import GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, glUseProgram, glGetUniformLocation


def read_file(file):
    with open(file, 'rb') as f:
        content = f.read()
    return content

class Shader:
    def __init__(self, vertex_shader_location: str, fragment_shader_location: str, mode: int):
        vertex_shader = compileShader(read_file(vertex_shader_location), GL_VERTEX_SHADER)
        fragment_shader = compileShader(read_file(fragment_shader_location), GL_FRAGMENT_SHADER)
        self._shader = compileProgram(vertex_shader,fragment_shader)
        glDeleteShader(vertex_shader)
        glDeleteShader(fragment_shader)
        self.mode = mode

    def use(self):
        glUseProgram(self._shader)

    def set_uniform_int(self, name: str, value: int):
        glUniform1i(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), value)

    def set_uniform_float(self, name: str, value: float):
        glUniform1f(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), value)

    def set_uniform_mat4(self, name: str, value):
        glUniformMatrix4fv(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), 1, GL_TRUE, value)

    def set_uniform_vec3(self, name: str, value):
        glUniform3f(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), *value)

    def set_uniform_vec4(self, name: str, value):
        glUniform4f(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), *value)

    def set_uniform_vec3s(self, name: str, value, count: int):
        glUniform3fv(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), count, value)

    def set_uniform_vec4s(self, name: str, value, count: int):
        glUniform4fv(glGetUniformLocation(self._shader, create_string_buffer(name.encode('ascii'))), count, value)
