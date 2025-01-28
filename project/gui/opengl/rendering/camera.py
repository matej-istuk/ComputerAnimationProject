from typing import Tuple

from gui.opengl.rendering.object3d import Object3D
from gui.opengl.rendering.utils import look_at, create_perspective_matrix_2


class Camera(Object3D):
    def __init__(self,
                 front: Tuple[float, float, float, float],
                 up: Tuple[float, float, float, float],
                 right: Tuple[float, float, float, float],
                 position: Tuple[float, float, float, float],
                 scale: Tuple[float, float, float, float],
                 center, fov, aspect_ratio, near, far):
        super().__init__(front, up, right, position, scale)
        self._center = center
        self._perspective_matrix = create_perspective_matrix_2(fov, aspect_ratio, near, far)
        self._rx = 0
        self._rz = 0

    @property
    def view_matrix(self):
        return look_at(
            self._position[:3],
            self._center,
            self._up[:3]
        )

    @property
    def perspective_matrix(self):
        return self._perspective_matrix
