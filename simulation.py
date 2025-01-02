import time
from typing import Tuple

import cupy as cp


class Simulation:
    def __init__(self,
                 dims: Tuple[int, int],
                 gravity: float = 0.1,
                 mass: float = 0.1,
                 connection_strengths: Tuple[float, float, float] = (10., 2., 2.),
                 wind_speed: Tuple[float, float, float] = (0, 0, 1),
                 wind_constant: float = 0.01,
                 dampening: float = 1,
                 break_ratio: float = 2,
                 epsilon: float = 1e-10
                 ):
        height = dims[0] / max(dims)
        width = dims[1] / max(dims)
        self._gravity = gravity
        self._old_delta_time = 1.
        self._connection_strengths = cp.array([4 * [connection_strengths[0]] + 4 * [connection_strengths[1]]+ 4 * [connection_strengths[2]]])
        self._dampening = dampening
        self._epsilon = epsilon
        self._break_ratio = break_ratio
        self._wind_speed = cp.array(wind_speed)
        self._wind_constant = wind_constant

        self._points = cp.dstack([
            cp.broadcast_to(cp.linspace(0, width, dims[1])[None, :], dims),
            cp.broadcast_to(cp.linspace(0, height, dims[0])[:, None], dims),
            cp.zeros(dims)
        ])
        self._old_points = self._points.copy()
        self._velocities = cp.zeros_like(self._points)
        self._accelerations = cp.zeros_like(self._points)
        self._forces = cp.zeros_like(self._points)

        self._masses = cp.ones_like(self._points[..., 0]) * mass
        self._inv_masses = 1. / self._masses

        self._not_static = cp.ones_like(self._points[..., 0], dtype=cp.int8)

        self._connections = self._create_connections(dims)
        self._rest_lengths = self._connection_distances()

        self._normals = self._calculate_normals()

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

    def _connection_direction_and_distances(self):
        directions = self._connection_directions()
        return directions, cp.sqrt(cp.sum(directions ** 2, axis=-1))

    def _connection_distances(self):
        return cp.sqrt(cp.sum(self._connection_directions() ** 2, axis=-1))

    def _connection_directions(self):
        directions = cp.zeros((*self._connections.shape, 3), dtype=cp.float32)
        directions[1:, :, 0] = self._points[1:, :] - self._points[:-1, :]
        directions[:-1, :, 2] = self._points[:-1, :] - self._points[1:, :]
        directions[:, 1:, 1] = self._points[:, 1:] - self._points[:, :-1]
        directions[:, :-1, 3] = self._points[:, :-1] - self._points[:, 1:]

        directions[1:, :-1, 4] = self._points[1:, :-1] - self._points[:-1, 1:]
        directions[:-1, :-1, 5] = self._points[:-1, :-1] - self._points[1:, 1:]
        directions[:-1, 1:, 6] = self._points[:-1, 1:] - self._points[1:, :-1]
        directions[1:, 1:, 7] = self._points[1:, 1:] - self._points[:-1, :-1]

        directions[2:, :, 8] = self._points[2:, :] - self._points[:-2, :]
        directions[:-2, :, 11] = self._points[:-2, :] - self._points[2:, :]
        directions[:, 2:, 9] = self._points[:, 2:] - self._points[:, :-2]
        directions[:, :-2, 10] = self._points[:, :-2] - self._points[:, 2:]

        return directions

    def _calculate_normals(self, directions = None):
        if directions is None:
            directions = self._connection_directions()
        normals = cp.zeros_like(self._points)
        normals[1:, 1:] += cp.cross(directions[1:, 1:, 0], directions[1:, 1:, 1])
        normals[:-1, 1:] += cp.cross(directions[:-1, 1:, 1], directions[:-1, 1:, 2])
        normals[:-1, :-1] += cp.cross(directions[:-1, :-1, 2], directions[:-1, :-1, 3])
        normals[1:, :-1] += cp.cross(directions[1:, :-1, 3], directions[1:, :-1, 0])
        normals = normals / cp.linalg.norm(normals, axis=-1, keepdims=True)
        return normals

    @property
    def points(self):
        return self._points

    @property
    def normals(self):
        return self._normals

    @property
    def static(self):
        return 1 - self._not_static

    @property
    def connections(self):
        return self._connections

    def set_static(self, i, j):
        self._not_static[i, j] = 0

    def remove_static(self, i, j):
        self._not_static[i, j] = 1

    def set_position(self, i, j, x, y, z):
        self._points[i, j, 0], self._points[i, j, 1], self._points[i, j, 2] = x, y, z

    def update(self, delta_time: float):
        self._forces = cp.zeros_like(self._points)
        self._forces[..., 1] = self._forces[..., 1] + self._gravity * self._masses

        directions, distances = self._connection_direction_and_distances()
        distance_differences = self._rest_lengths - distances

        broken_connections = distances > (self._break_ratio * self._rest_lengths)
        self._connections[broken_connections] = 0

        self._forces += cp.sum((self._connections * self._connection_strengths)[..., None] * distance_differences[..., None] * directions / (distances[..., None] + self._epsilon), axis=-2)

        self._forces += self._wind_constant * (cp.sum(self._normals * (self._wind_speed - self._velocities), axis=-1))[..., None] * self._normals

        self._forces += -self._velocities * self._dampening

        self._forces = self._forces * self._not_static[..., None]
        self._accelerations = self._forces * self._inv_masses[..., None]
        self._improved_integrate(delta_time)
        self._normals = self._calculate_normals()

    def _integrate(self, delta_time: float):
        new_points = 2 * self._points - self._old_points + self._accelerations * delta_time * delta_time
        self._old_points = self._points
        self._points = new_points

    def _improved_integrate(self, delta_time: float):
        self._velocities = self._points - self._old_points
        self._old_points = self._points.copy()
        dt = delta_time / self._old_delta_time
        self._points = self._points + dt * self._velocities + self._accelerations * (delta_time * (delta_time + self._old_delta_time) / 2)
        self._old_delta_time = delta_time

        self._accelerations = cp.zeros_like(self._accelerations)
        self._forces = cp.zeros_like(self._forces)

if __name__ == '__main__':
    iters = 1000
    sim = Simulation(dims=(50, 100))
    sim.set_static(0, 0)
    t0 = time.time()
    for i in range(iters):
        sim.update(delta_time=0.1)
    t1 = time.time()
    dur = (t1 - t0)/iters
    print(f'1000 iters took {t1-t0:.2f} seconds, {dur:.5f} seconds per iter, {1/dur:.2f} iterations/second')