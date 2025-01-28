from typing import Tuple


class Renderer:
    def draw_line(self, x1: int, y1: int, x2: int, y2: int, color: Tuple[int, int, int], width: int):
        pass

    def draw_point(self, x: int, y: int, color: Tuple[int, int, int], size: int):
        pass

def draw_cloth(points, static_mask, connections_mask, color, renderer: Renderer):
    for i in range(points.shape[0]):
        for j in range(points.shape[1]):
            if i != (points.shape[0] - 1) and connections_mask[i, j, 2] == 1:
                renderer.draw_line(points[i, j, 0], points[i, j, 1], points[i + 1, j, 0], points[i + 1, j, 1], color, 1)
            if j != (points.shape[1] - 1) and connections_mask[i, j, 3] == 1:
                renderer.draw_line(points[i, j, 0], points[i, j, 1], points[i, j + 1, 0], points[i, j + 1, 1], color, 1)
            size = 4 if static_mask[i, j] == 0 else 8
            renderer.draw_point(points[i, j, 0], points[i, j, 1], color, size)
