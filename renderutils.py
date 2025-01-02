import numpy as np
from numpy.ma.core import indices

from renderer import Renderer
from simulation import Simulation


def draw_cloth(points, static_mask, connections_mask, color, renderer: Renderer):
    for i in range(points.shape[0]):
        for j in range(points.shape[1]):
            if i != (points.shape[0] - 1) and connections_mask[i, j, 2] == 1:
                renderer.draw_line(points[i, j, 0], points[i, j, 1], points[i + 1, j, 0], points[i + 1, j, 1], color, 1)
            if j != (points.shape[1] - 1) and connections_mask[i, j, 3] == 1:
                renderer.draw_line(points[i, j, 0], points[i, j, 1], points[i, j + 1, 0], points[i, j + 1, 1], color, 1)
            size = 4 if static_mask[i, j] == 0 else 8
            renderer.draw_point(points[i, j, 0], points[i, j, 1], color, size)


def indices_for_simulation(simulation: Simulation):
    indices = []
    height = simulation.points.shape[0]
    width = simulation.points.shape[1]
    for i in range(height - 1):
        for j in range(width - 1):
            indices.extend([
                i*width+j,
                i*width+j+1,
                (i+1)*width+j,
            ])
            indices.extend([
                i*width+j+1,
                (i+1)*width+j+1,
                (i+1)*width+j,
            ])
    return indices

def export_to_obj(simulation: Simulation):
    vertices = simulation.points.get().reshape((-1, 3))
    normals = simulation.normals.get().reshape((-1, 3))
    inds = indices_for_simulation(simulation)

    for vertex in vertices:
        print(f'v {vertex[0]:.3f} {vertex[1]:.3f} {vertex[2]:.3f}')

    print()

    for normal in normals:
        print(f'vn {normal[0]:.3f} {normal[1]:.3f} {normal[2]:.3f}')

    print()

    for i in range(0, len(inds), 3):
        print(f'f {inds[i] + 1} {inds[i+1] + 1} {inds[i+2] + 1}')
