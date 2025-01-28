from typing import Iterable, List, Tuple

from project.simulation.fabric import Fabric


class StaticPointSetter:
    def __init__(self, height_iterable: Iterable = None, width_iterable: Iterable = None, step: int = None, specific_points: List[Tuple[int, int]] = None):
        if step is None:
            step = 1
        if specific_points is None:
            specific_points = []
        self.height_iterable = height_iterable
        self.width_iterable = width_iterable
        self.step = step
        self.specific_points = specific_points

    def set_statics(self, fabric: Fabric):
        height_iterable = self.height_iterable if self.height_iterable is not None else range(0, fabric.points.shape[0], self.step)
        width_iterable = self.width_iterable if self.width_iterable is not None else range(0, fabric.points.shape[1], self.step)

        for i in height_iterable:
            for j in width_iterable:
                if i == 0 or j == 0 or i == (fabric.points.shape[0] - 1) or i == -1 or j == (fabric.points.shape[1] - 1) or j == -1:
                    fabric.set_static(i, j)

        for point in self.specific_points:
            fabric.set_static(point[0], point[1])