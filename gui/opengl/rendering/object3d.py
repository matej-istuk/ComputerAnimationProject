from typing import Tuple

import numpy as np

from gui.opengl.rendering.utils import rotation_matrix, translation_matrix


class Object3D:
    def __init__(self,
                 front: Tuple[float, float, float, float],
                 up: Tuple[float, float, float, float],
                 right: Tuple[float, float, float, float],
                 position: Tuple[float, float, float, float],
                 scale: Tuple[float, float, float, float]):
        self._front = np.array(front)
        self._up = np.array(up)
        self._right = np.array(right)
        self._position = np.array(position)
        self._scale = np.array(scale)

    @property
    def front(self):
        return self._front

    @property
    def up(self):
        return self._up

    @property
    def right(self):
        return self._right

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, value: Tuple[float, float, float, float]):
        self._position = np.array(value)

    @property
    def scale(self):
        return self._scale

    @property
    def model_matrix(self):
        mat = np.array([self._right, self._up, self._front, self._position])
        mat[3, :3] = 0
        mat[3, 3] = 1
        return mat @ np.diag(np.array([*self._scale[:3], 1]))

    def local_rotate(self, angle: Tuple[float, float, float]):
        mat = np.eye(4)
        mat = mat @ rotation_matrix(angle[0], self._right[:3])
        mat = mat @ rotation_matrix(angle[1], self._up[:3])
        mat = mat @ rotation_matrix(angle[2], self._front[:3])
        self._front = self._front @ mat
        self._up = self._up @ mat
        self._right = self._right @ mat

    def global_rotate(self, angle: Tuple[float, float, float]):
        mat = np.eye(4)
        mat = mat @ rotation_matrix(angle[0], np.array([1, 0, 0]))
        mat = mat @ rotation_matrix(angle[1], np.array([0, 1, 0]))
        mat = mat @ rotation_matrix(angle[2], np.array([0, 0, 1]))
        self._front = mat @self._front
        self._up = mat @self._up
        self._right = mat @self._right
        self._position = mat @self._position

    def local_move(self, move: Tuple[float, float, float]):
        temp_position = np.linalg.inv(self.model_matrix) @ self._position
        temp_position = translation_matrix(np.array(move)) @ temp_position
        self._position = self.model_matrix @ temp_position

    def global_move(self, move: Tuple[float, float, float]):
        self._position = translation_matrix(np.array(move)) @ self._position

    def do_scale(self, scale: Tuple[float, float, float]):
        self._scale = np.diag(np.array([*scale, 1])) * self._scale