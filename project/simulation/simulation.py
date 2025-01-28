import time
from typing import Tuple

import cupy as cp

from simulation.fabric import Fabric


class Simulation:
    def __init__(self,
                 fabric: Fabric,
                 gravity: float = 0.2,
                 wind_speed: Tuple[float, float, float] = (0, 0, 1),
                 wind_constant: float = 0.02,
                 dampening: float = 1,
                 epsilon: float = 1e-10,
                 speed: float = 1.
                 ):
        self.fabric = fabric
        self._gravity = gravity
        self._old_delta_time = 1.

        self._dampening = dampening
        self._epsilon = epsilon

        self._wind_speed = cp.array(wind_speed)
        self._wind_constant = wind_constant

        self._speed = speed

    def update(self, delta_time: float):
        delta_time *= self._speed
        self.fabric.forces = cp.zeros_like(self.fabric.points)
        self.fabric.forces[..., 1] = self.fabric.forces[..., 1] + self._gravity * self.fabric.masses

        directions, distances = self.fabric.connection_direction_and_distances()
        distance_differences = self.fabric.rest_lengths - distances

        broken_connections = distances > (self.fabric.break_ratio * self.fabric.rest_lengths)
        self.fabric.connections[broken_connections] = 0

        self.fabric.forces += cp.sum(
            (self.fabric.connections * self.fabric.connection_strengths)[..., None] * distance_differences[..., None] * directions / (distances[..., None] + self._epsilon),
            axis=-2
        )

        self.fabric.forces += self._wind_constant * (cp.sum(self.fabric.normals * (self._wind_speed - self.fabric.velocities), axis=-1))[..., None] * self.fabric.normals

        self.fabric.forces = self.fabric.forces - self.fabric.velocities * self._dampening

        self.fabric.forces = self.fabric.forces * self.fabric.not_static[..., None] / self.fabric.points_num
        self.fabric.accelerations = self.fabric.forces * self.fabric.inv_masses[..., None]
        self._integrate(delta_time)
        self.fabric.normals = self.fabric.calculate_normals()

    def _integrate(self, delta_time: float):
        delta_points = self.fabric.points - self.fabric.old_points
        self.fabric.old_points = self.fabric.points.copy()
        dt = delta_time / self._old_delta_time
        self.fabric.points = self.fabric.points + dt * delta_points + self.fabric.accelerations * (delta_time * (delta_time + self._old_delta_time) / 2)
        self._old_delta_time = delta_time

        self.fabric.accelerations = cp.zeros_like(self.fabric.accelerations)
        self.fabric.velocities = delta_points / delta_time
        self.fabric.forces = cp.zeros_like(self.fabric.forces)

    def change_speed(self, speed: float):
        self._speed *= speed

if __name__ == '__main__':
    iters = 1000
    cloth = Fabric((50, 100))
    sim = Simulation(cloth)
    cloth.set_static(0, 0)
    t0 = time.time()
    for i in range(iters):
        sim.update(delta_time=0.1)
    t1 = time.time()
    dur = (t1 - t0)/iters
    print(f'1000 iters took {t1-t0:.2f} seconds, {dur:.5f} seconds per iter, {1/dur:.2f} iterations/second')
