from typing import Tuple, List

import numpy as np
import pyglet

from renderer import Renderer, draw_cloth
from gui.opengl.rendering.renderutils import export_to_obj
from simulation.simulation import Simulation

CAPTION = "ClothSim"


class SimulationWindow(pyglet.window.Window):
    def __init__(self,
                 simulation: Simulation,
                 window_height: int = 900,
                 window_width: int = 1200,
                 ups: int = 60,
                 background_color: Tuple[int, int, int] = (20, 20, 20),
                 cloth_color: Tuple[int, int, int] = (255, 255, 255),
                 static_points: List[Tuple[int, int]] = None,
                 minimal_margin: int = 100,
                 app_config = None,
                 *args, **kwargs
                 ):
        if static_points is None:
            static_points = [(0, 0), (4, 0), (9, 0), (0, -1), (4, -1), (9, -1)]
        self._minimal_margin = minimal_margin
        self._window_height = window_height
        self._window_width = window_width
        super().__init__(self._window_width, self._window_height, CAPTION, resizable=True, config=app_config, *args, **kwargs)
        self._initial_size = min(window_width, window_height) - 2 * self._minimal_margin
        self._margins = ((self._window_height - self._initial_size) // 2, (self._window_width - self._initial_size) // 2)
        self._simulation = simulation
        for static_point in static_points:
            self._simulation.fabric.set_static(*static_point)
        self._batch = pyglet.graphics.Batch()
        self._background = pyglet.shapes.Rectangle(0, 0, self._window_width, self._window_height, batch=self._batch)
        self._background.color = background_color
        self._cloth_color = cloth_color
        self.renderer = PygletRenderer(self._batch)
        self.fps_display = pyglet.window.FPSDisplay(window=self)

        pyglet.clock.schedule_interval(self.update, 1.0 / ups)

    def update(self, delta_time: float):
            self._simulation.update(delta_time)

    def on_draw(self):
        self.clear()
        points_cpu = self._simulation.fabric.points.get()
        points_cpu = points_cpu
        points_cpu[..., 0] = self._margins[1] + points_cpu[..., 0] * self._initial_size
        points_cpu[..., 1] = self._window_height - (self._margins[0] + points_cpu[..., 1] * self._initial_size)
        draw_cloth(points_cpu.astype(np.int32), self._simulation.fabric.static, self._simulation.fabric.connections, self._cloth_color, self.renderer)
        self._batch.draw()
        self.fps_display.draw()
        self.renderer.clear()

    def on_close(self):
        export_to_obj(self._simulation.fabric)
        self.close()

class PygletRenderer(Renderer):
    def __init__(self, batch: pyglet.graphics.Batch):
        self._batch = batch
        self._objects = []

    def draw_point(self, x: int, y: int, color: Tuple[int, int, int], size: int):
        self._objects.append(
            pyglet.shapes.Circle(x, y, size, color=color, batch=self._batch)
        )

    def draw_line(self, x1: int, y1: int, x2: int, y2: int, color: Tuple[int, int, int], width: int):
        self._objects.append(
            pyglet.shapes.Line(x1, y1, x2, y2, width=width, color=color, batch=self._batch)
        )

    def clear(self):
        self._objects.clear()
