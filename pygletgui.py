from pyglet.gl import *
from typing import Tuple, List

import pyglet

from renderutils import gen_indices
from simulation import Simulation

CAPTION = "ClothSim"


class SimulationWindow(pyglet.window.Window):
    def __init__(self,
                 simulation: Simulation,
                 window_height: int = 900,
                 window_width: int = 1200,
                 ups: int = 60,
                 background_color: Tuple[int, int, int] = (0.2, 0.2, 0.2),
                 cloth_color: Tuple[int, int, int] = (0, 0, 255),
                 static_points: List[Tuple[int, int]] = None,
                 *args, **kwargs
                 ):
        if static_points is None:
            static_points = [(0, 0), (4, 0), (9, 0), (0, -1), (4, -1), (9, -1)]
        self._window_height = window_height
        self._window_width = window_width
        super().__init__(self._window_width, self._window_height, CAPTION, resizable=True, *args, **kwargs)
        self._simulation = simulation
        for static_point in static_points:
            self._simulation.fabric.set_static(*static_point)
        self._background_color = background_color
        self._cloth_color = cloth_color
        self._background_color = background_color
        self.fps_display = pyglet.window.FPSDisplay(window=self)
        self._rx = 0
        self._ry = 180
        self._rz = 180

        indices = gen_indices(self._simulation.fabric)
        self._indices = (GLuint * len(indices))(*indices)
        self._opengl_setup()

        pyglet.clock.schedule_interval(self.update, 1.0 / ups)

    def _opengl_setup(self):
        glClearColor(*self._background_color, 1)
        glColor3f(1, 0, 0)
        glEnable(GL_DEPTH_TEST)

        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glEnable(GL_LIGHT1)

        def vec(*args):
            return (GLfloat * len(args))(*args)

        glLightfv(GL_LIGHT0, GL_POSITION, vec(0, 0, 5, 0))
        glLightfv(GL_LIGHT0, GL_SPECULAR, vec(.5, .5, 1, 1))
        glLightfv(GL_LIGHT0, GL_DIFFUSE, vec(1, 1, 1, 1))
        glLightfv(GL_LIGHT1, GL_POSITION, vec(0, 0, -5, 0))
        glLightfv(GL_LIGHT1, GL_DIFFUSE, vec(.5, .5, .5, 1))
        glLightfv(GL_LIGHT1, GL_SPECULAR, vec(1, 1, 1, 1))

        glMaterialfv(
            GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, vec(*self._cloth_color, 1))
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec(1, 1, 1, 1))
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50)

    def on_resize(self, width, height):
        # Override the default on_resize handler to create a 3D projection
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(60., width / float(height), .1, 1000.)
        glMatrixMode(GL_MODELVIEW)
        return pyglet.event.EVENT_HANDLED

    def update(self, delta_time: float):
        self._simulation.update(delta_time)

    def on_draw(self):
        points_cpu = self._simulation.fabric.points.get().flatten()
        normals_cpu = self._simulation.fabric.normals.get().flatten()
        vertices = (GLfloat * points_cpu.shape[0])(*points_cpu)
        normals = (GLfloat * normals_cpu.shape[0])(*normals_cpu)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glRotatef(self._rz, 0, 0, 1)
        glRotatef(self._ry, 0, 1, 0)
        glRotatef(self._rx, 1, 0, 0)
        glTranslatef(-0.5, 0.5, -2.5)

        l = glGenLists(1)
        glNewList(l, GL_COMPILE)

        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
        glEnableClientState(GL_VERTEX_ARRAY)
        glEnableClientState(GL_NORMAL_ARRAY)
        glVertexPointer(3, GL_FLOAT, 0, vertices)
        glNormalPointer(GL_FLOAT, 0, normals)
        glDrawElements(GL_TRIANGLES, len(self._indices), GL_UNSIGNED_INT, self._indices)
        glPopClientAttrib()

        glEndList()

        glCallList(l)
        self.fps_display.draw()

    def on_mouse_motion(self, x, y, dx, dy):
        self._rx += dy
        self._ry += dx
        self._rx = self._rx % 360
        self._ry = self._ry % 360