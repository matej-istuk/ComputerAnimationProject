from simulation.simulation import Fabric

def gen_indices(fabric: Fabric):
    inds = []
    height = fabric.points.shape[0]
    width = fabric.points.shape[1]
    for i in range(height - 1):
        for j in range(width - 1):
            inds.extend([
                i*width+j,
                i*width+j+1,
                (i+1)*width+j,
            ])
            inds.extend([
                i*width+j+1,
                (i+1)*width+j+1,
                (i+1)*width+j,
            ])
    return inds

def export_to_obj(fabric: Fabric):
    vertices = fabric.points.get().reshape((-1, 3))
    normals = fabric.normals.get().reshape((-1, 3))
    inds = gen_indices(fabric)

    for vertex in vertices:
        print(f'v {vertex[0]:.3f} {vertex[1]:.3f} {vertex[2]:.3f}')

    print()

    for normal in normals:
        print(f'vn {normal[0]:.3f} {normal[1]:.3f} {normal[2]:.3f}')

    print()

    for i in range(0, len(inds), 3):
        print(f'f {inds[i] + 1} {inds[i+1] + 1} {inds[i+2] + 1}')
