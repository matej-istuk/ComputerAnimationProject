import ctypes

import numpy as np
from pyglet.gl import *
from typing import Tuple, List

import pyglet

from gui.opengl.rendering.camera import Camera
from renderutils import gen_indices
from gui.opengl.rendering.shader import Shader
from simulation import Simulation, Fabric

CAPTION = "Fabric Simulator"


class SimulationWindow(pyglet.window.Window):
    def __init__(self,
                 simulation: Simulation,
                 window_height: int = 900,
                 window_width: int = 1200,
                 ups: int = 60,
                 background_color: Tuple[int, int, int] = (0.8, 0.8, 0.8),
                 cloth_color: Tuple[int, int, int] = (0, 0, 255),
                 *args, **kwargs
                 ):
        self._window_height = window_height
        self._window_width = window_width
        super().__init__(self._window_width, self._window_height, CAPTION, resizable=True, *args, **kwargs)
        self._simulation = simulation
        self._background_color = background_color
        self._cloth_color = cloth_color

        self._renderer = FabricSceneRenderer(self._simulation.fabric, 'shaders/shader_mesh.vert', 'shaders/shader_mesh.frag', background_color)

        pyglet.clock.schedule_interval(self.update, 1.0 / ups)

    def update(self, delta_time: float):
        self._simulation.update(delta_time)

    def on_draw(self):
        self._renderer.draw()

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifiers):
        self._renderer.camera.rotate_x(dy * 0.5)
        self._renderer.camera.rotate_z(dx * 0.5)

class FabricSceneRenderer:
    def __init__(self,
                 fabric: Fabric,
                 vertex_shader_location: str,
                 fragment_shader_location: str,
                 background_color: Tuple[int, int, int],
                 camera: Camera = None,
                 model_matrix: List[List[float]] = None,
                 ):
        if model_matrix is None:
            model_matrix = np.identity(4, dtype=np.float32)
        if camera is None:
            camera = Camera(np.array([2, 0, 2]), np.array([0, 0, 0]), np.array([0, -1, 0]), 60, 1, 0.1, 10)
        self.camera = camera
        self._model_matrix = np.array(model_matrix)

        self._background_color = background_color

        self._shader = Shader(vertex_shader_location, fragment_shader_location)
        self._fabric = fabric
        indices = gen_indices(self._fabric)
        self._indicesGl = (GLint * len(indices))(*indices)

        self._vao = GLuint()
        self._vert_vbo = GLuint()
        self._norm_vbo = GLuint()
        self._ebo = GLuint()
        glGenVertexArrays(1, self._vao)
        glGenBuffers(1, self._vert_vbo)
        glGenBuffers(1, self._norm_vbo)
        glGenBuffers(1, self._ebo)

        self._update_vao()

        self._opengl_setup()

    def _opengl_setup(self):
        glEnable(GL_DEPTH_TEST)
        glClearColor(*self._background_color, 1)

    def _update_vao(self):
        glBindVertexArray(self._vao)

        glBindBuffer(GL_ARRAY_BUFFER, self._vert_vbo)
        points = self._fabric.points.get().flatten()
        pointsGl = (GLfloat * len(points))(*points)
        glBufferData(GL_ARRAY_BUFFER, 4 * len(pointsGl), pointsGl, GL_STATIC_DRAW)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, ctypes.c_void_p(0))

        glBindBuffer(GL_ARRAY_BUFFER, self._norm_vbo)
        normals = self._fabric.normals.get().flatten()
        normalsGl = (GLfloat * len(normals))(*normals)
        glBufferData(GL_ARRAY_BUFFER, 4 * len(normalsGl), normalsGl, GL_STATIC_DRAW)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 4, ctypes.c_void_p(0))

        glEnableVertexAttribArray(0)
        glEnableVertexAttribArray(1)

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self._ebo)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * len(self._indicesGl), self._indicesGl, GL_STATIC_DRAW)

        glBindVertexArray(0)


    def draw(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        self._shader.use()
        self._shader.set_uniform_mat4('view_matrix', self.camera.view_matrix)
        self._shader.set_uniform_mat4('perspective_matrix', self.camera.perspective_matrix)
        self._shader.set_uniform_mat4('model_matrix', self._model_matrix)

        self._update_vao()
        glBindVertexArray(self._vao)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glDrawElements(GL_TRIANGLES, len(self._indicesGl), GL_UNSIGNED_INT, 0)
        glBindVertexArray(0)
