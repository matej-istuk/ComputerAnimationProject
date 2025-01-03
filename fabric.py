from typing import Tuple

import cupy as cp

class Fabric:
    def __init__(self,
                 dims: Tuple[int, int],
                 mass: float = 0.1,
                 connection_strengths: Tuple[float, float, float] = (10., 2., 2.),
                 break_ratio: float = 200,
                 center = True
                 ):
        height = dims[0] / max(dims)
        width = dims[1] / max(dims)
        self.connection_strengths = cp.array(
            [4 * [connection_strengths[0]] + 4 * [connection_strengths[1]] + 4 * [connection_strengths[2]]])
        self.break_ratio = break_ratio
        self.points = cp.dstack([
            cp.broadcast_to(cp.linspace(0, width, dims[1], dtype=cp.float32)[None, :], dims),
            cp.broadcast_to(cp.linspace(0, height, dims[0], dtype=cp.float32)[:, None], dims),
            cp.zeros(dims, dtype=cp.float32)
        ])
        if center:
            self.points[..., 0] -= width / 2
            self.points[..., 1] -= height / 2
        self.old_points = self.points.copy()
        self.velocities = cp.zeros_like(self.points)
        self.accelerations = cp.zeros_like(self.points)
        self.forces = cp.zeros_like(self.points)
        self.masses = cp.ones_like(self.points[..., 0]) * mass
        self.inv_masses = 1. / self.masses
        self.not_static = cp.ones_like(self.points[..., 0], dtype=cp.int8)
        self.connections = self._create_connections(dims)
        self.rest_lengths = self.connection_distances()
        self.normals = self.calculate_normals()

    @property
    def static(self):
        return 1 - self.not_static

    def set_static(self, i, j):
        self.not_static[i, j] = 0

    def remove_static(self, i, j):
        self.not_static[i, j] = 1

    def set_position(self, i, j, x, y, z):
        self.points[i, j, 0], self.points[i, j, 1], self.points[i, j, 2] = x, y, z

    # connection legend:
    #     0 - 3 structural
    #         0 - north
    #         1 - west
    #         2 - south
    #         3 - east
    #     4 - 7 shear
    #         4 - north-east
    #         5 - south-east
    #         6 - south-west
    #         7 - north-west
    #     8 - 11 flexion
    #         8 - north
    #         9 - west
    #         10 - east
    #         11 - south
    @staticmethod
    def _create_connections(dims):
        connections = cp.ones((*dims, 12), dtype=cp.int8)
        connections[0, :, 0] = 0
        connections[-1, :, 2] = 0
        connections[:, 0, 1] = 0
        connections[:, -1, 3] = 0

        connections[0, :, 4] = 0
        connections[0, :, 7] = 0
        connections[-1, :, 5] = 0
        connections[-1, :, 6] = 0
        connections[:, 0, 4] = 0
        connections[:, 0, 5] = 0
        connections[:, -1, 6] = 0
        connections[:, -1, 7] = 0

        connections[:2, :, 8] = 0
        connections[-2:, :, 11] = 0
        connections[:, :2, 9] = 0
        connections[:, -2:, 10] = 0

        return connections

    def connection_direction_and_distances(self):
        directions = self.connection_directions()
        return directions, cp.sqrt(cp.sum(directions ** 2, axis=-1))

    def connection_distances(self):
        return cp.sqrt(cp.sum(self.connection_directions() ** 2, axis=-1))

    def connection_directions(self):
        directions = cp.zeros((*self.connections.shape, 3), dtype=cp.float32)
        directions[1:, :, 0] = self.points[1:, :] - self.points[:-1, :]
        directions[:-1, :, 2] = self.points[:-1, :] - self.points[1:, :]
        directions[:, 1:, 1] = self.points[:, 1:] - self.points[:, :-1]
        directions[:, :-1, 3] = self.points[:, :-1] - self.points[:, 1:]

        directions[1:, :-1, 4] = self.points[1:, :-1] - self.points[:-1, 1:]
        directions[:-1, :-1, 5] = self.points[:-1, :-1] - self.points[1:, 1:]
        directions[:-1, 1:, 6] = self.points[:-1, 1:] - self.points[1:, :-1]
        directions[1:, 1:, 7] = self.points[1:, 1:] - self.points[:-1, :-1]

        directions[2:, :, 8] = self.points[2:, :] - self.points[:-2, :]
        directions[:-2, :, 11] = self.points[:-2, :] - self.points[2:, :]
        directions[:, 2:, 9] = self.points[:, 2:] - self.points[:, :-2]
        directions[:, :-2, 10] = self.points[:, :-2] - self.points[:, 2:]

        return directions

    def calculate_normals(self, directions=None):
        if directions is None:
            directions = self.connection_directions()
        normals = cp.zeros_like(self.points)
        normals[1:, 1:] += cp.cross(directions[1:, 1:, 0], directions[1:, 1:, 1])
        normals[:-1, 1:] += cp.cross(directions[:-1, 1:, 1], directions[:-1, 1:, 2])
        normals[:-1, :-1] += cp.cross(directions[:-1, :-1, 2], directions[:-1, :-1, 3])
        normals[1:, :-1] += cp.cross(directions[1:, :-1, 3], directions[1:, :-1, 0])
        normals = normals / cp.linalg.norm(normals, axis=-1, keepdims=True)
        return normals
