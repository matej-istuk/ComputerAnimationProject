import ctypes

import numpy as np
from pyglet.gl import *
from typing import Tuple, List

import pyglet

from gui.opengl.rendering.camera import Camera
from gui.opengl.rendering.light import Light
from gui.opengl.rendering.material import Material
from gui.opengl.rendering.object3d import Object3D
from gui.opengl.rendering.renderutils import gen_indices, export_to_obj
from gui.opengl.rendering.shader import Shader
from simulation.simulation import Simulation, Fabric

CAPTION = "Fabric Simulator"


class SimulationWindow(pyglet.window.Window):
    def __init__(self,
                 simulation: Simulation,
                 window_height: int = 900,
                 window_width: int = 1200,
                 ups: int = 60,
                 speedup: float = 1.,
                 background_color: Tuple[int, int, int] = (0.8, 0.8, 0.8),
                 fabric_material: Material = None,
                 constant_sim_update: bool = True,
                 app_config = None,
                 *args, **kwargs
                 ):
        if fabric_material is None:
            fabric_material = Material()
        self._window_height = window_height
        self._window_width = window_width
        super().__init__(self._window_width, self._window_height, CAPTION, resizable=True, config=app_config, *args, **kwargs)
        self._simulation = simulation
        self._background_color = background_color
        self._speedup = speedup
        self._inv_ups = 1.0 / ups
        self._constant_sim_update = constant_sim_update

        self._renderer = FabricSceneRenderer(self._simulation.fabric,
                                             [('shaders/shader_mesh.vert', 'shaders/shader_mesh.frag', GL_LINE),
                                              ('shaders/shader_material.vert', 'shaders/shader_material.frag', GL_FILL)],
                                             background_color, fabric_material=fabric_material)

        pyglet.clock.schedule_interval(self.update, self._inv_ups)

    def update(self, delta_time: float):
        if self._constant_sim_update:
            delta_time = self._inv_ups
        self._simulation.update(delta_time * self._speedup)

    def on_draw(self):
        self._renderer.draw()

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifiers):
        self._renderer.fabric_model.local_rotate((0, dx * 0.01, 0))

    def on_mouse_scroll(self, x: int, y: int, scroll_x: float, scroll_y: float) -> None:
        self._renderer.fabric_model.do_scale((1 + scroll_y / 100, 1 + scroll_y / 100, 1 + scroll_y / 100))

    def on_key_press(self, symbol: int, modifiers: int) -> None:
        if symbol == pyglet.window.key.M:
            self._renderer.change_shader()
        if symbol == pyglet.window.key.F:
            self._speedup *= 1.1
        if symbol == pyglet.window.key.S:
            self._speedup *= 0.9
        if symbol == pyglet.window.key.E:
            export_to_obj(self._simulation.fabric)


class FabricSceneRenderer:
    def __init__(self,
                 fabric: Fabric,
                 shader_descriptors: List[Tuple[str, str, int]],
                 background_color: Tuple[int, int, int],
                 camera: Camera = None,
                 fabric_model: Object3D = None,
                 fabric_material: Material = None,
                 lights: List[Light] = None,
                 ):
        if camera is None:
            camera = Camera((0, 0, -1, 0),
                            (0, -1, 0, 0),
                            (1, 0, 0, 0),
                            (0, 0, 2, 0),
                            (1, 1, 1, 0),
                            (0, 0, 0),
                            60, 1, 0.1, 10)
        if fabric_model is None:
            fabric_model = Object3D(
                (0, 0, 1, 0),
                (0, 1, 0, 0),
                (1, 0, 0, 0),
                (0, 0, 0, 0),
                (1, 1, 1, 0),
            )
        if lights is None:
            lights = [
                Light(position=(0, 1, -2, 1)),
                Light(position=(0, 1, 2, 1)),

            ]
        if fabric_material is None:
            fabric_material = Material()

        self.camera = camera
        self.fabric_model = fabric_model
        self.lights = lights
        self.fabric_material = fabric_material

        self._background_color = background_color

        self._shader_mode = 1
        self._shaders = []
        for shader_descriptor in shader_descriptors:
            self._shaders.append(Shader(shader_descriptor[0], shader_descriptor[1], shader_descriptor[2]))

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

    def change_shader(self):
        self._shader_mode = (self._shader_mode + 1) % len(self._shaders)

    @property
    def shader(self):
        return self._shaders[self._shader_mode]

    def draw(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        self.shader.use()
        self.shader.set_uniform_mat4('view_matrix', self.camera.view_matrix)
        self.shader.set_uniform_mat4('perspective_matrix', self.camera.perspective_matrix)
        self.shader.set_uniform_mat4('model_matrix', self.fabric_model.model_matrix)

        self.shader.set_uniform_vec4("eye_pos", self.camera.position)
        self.shader.set_uniform_vec3("material_ambient", self.fabric_material.ambient_coefficient)
        self.shader.set_uniform_vec3("material_diffuse", self.fabric_material.diffuse_coefficient)
        self.shader.set_uniform_vec3("material_specular", self.fabric_material.specular_coefficient)
        self.shader.set_uniform_float("material_reflective", self.fabric_material.shininess_exponent)

        light_positions = []
        light_ambient = []
        light_diffuse = []
        light_specular = []

        for light in self.lights:
            light_positions.extend(light.position)
            light_ambient.extend(light.ambient_intensity)
            light_diffuse.extend(light.diffuse_intensity)
            light_specular.extend(light.specular_intensity)

        light_positions = np.array(light_positions)
        light_ambient = np.array(light_ambient)
        light_diffuse = np.array(light_diffuse)
        light_specular = np.array(light_specular)

        self.shader.set_uniform_int("lights_count", len(self.lights))
        self.shader.set_uniform_vec4s("lights_pos", light_positions, len(self.lights))
        self.shader.set_uniform_vec3s("lights_ambient", light_ambient, len(self.lights))
        self.shader.set_uniform_vec3s("lights_diffuse", light_diffuse, len(self.lights))
        self.shader.set_uniform_vec3s("lights_specular", light_specular, len(self.lights))

        self._update_vao()
        glBindVertexArray(self._vao)
        glPolygonMode(GL_FRONT_AND_BACK, self.shader.mode)
        glDrawElements(GL_TRIANGLES, len(self._indicesGl), GL_UNSIGNED_INT, 0)
        glBindVertexArray(0)
