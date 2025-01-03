import numpy as np

from gui.opengl.rendering.utils import look_at, create_perspective_matrix_2, rotate_x, rotate_y


class Camera:
    def __init__(self, eye, center, up, fov, aspect_ratio, near, far):
        self._view_matrix = look_at(eye, center, up)
        self._perspective_matrix = create_perspective_matrix_2(fov, aspect_ratio, near, far)
        self._rx = 0
        self._rz = 0

    def rotate_x(self, angle):
        self._rx = (self._rx + angle) % 360

    def rotate_z(self, angle):
        self._rz = (self._rz + angle) % 360

    @property
    def view_matrix(self):
        return self._view_matrix @ rotate_x(self._rx / 180 * np.pi) @ rotate_y(self._rz / 180 * np.pi)

    @property
    def perspective_matrix(self):
        return self._perspective_matrix
