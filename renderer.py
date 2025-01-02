from typing import Tuple


class Renderer:
    def draw_line(self, x1: int, y1: int, x2: int, y2: int, color: Tuple[int, int, int], width: int):
        pass

    def draw_point(self, x: int, y: int, color: Tuple[int, int, int], size: int):
        pass
