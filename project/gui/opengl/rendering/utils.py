import numpy as np


def look_at(eye: np.ndarray, center: np.ndarray, up: np.ndarray) -> np.ndarray:
    z_axis = (center - eye)
    z_axis = z_axis/np.linalg.norm(z_axis)
    x_axis = np.cross(up, z_axis)
    x_axis = x_axis/np.linalg.norm(x_axis)
    y_axis = np.cross(z_axis, x_axis)
    y_axis = y_axis/np.linalg.norm(y_axis)

    m = np.zeros((4, 4), dtype=np.float32)
    m[0, :-1] = x_axis
    m[1, :-1] = y_axis
    m[2, :-1] = z_axis
    m[0][3] = np.dot(x_axis, eye)
    m[1][3] = np.dot(y_axis, eye)
    m[2][3] = np.dot(z_axis, eye)
    m[-1, -1] = 1.0

    return m

def create_perspective_matrix(left: float, right: float, bottom: float, top: float, near: float, far: float) -> np.ndarray:
   return np.array([
       [2*near/(right-left), 0, (right+left)/(right-left), 0],
       [0, 2*near/(top-bottom), (top+bottom)/(top-bottom), 0],
       [0, 0, (near+far)/(near-far), 2*near*far/(near-far)],
       [0, 0, -1, 0]
   ], dtype=np.float32)

def create_perspective_matrix_2(fov_deg: float, aspect_ratio: float, near: float, far: float) -> np.ndarray:
   y_max = near*np.tan(fov_deg * np.pi / 360)
   x_max = y_max * aspect_ratio
   return create_perspective_matrix(-x_max, x_max, -y_max, y_max, near, far)

def rotation_matrix(angle: float, axis: np.ndarray) -> np.ndarray:
    axis = axis
    axis = axis / np.sqrt(np.dot(axis, axis))
    a = np.cos(angle / 2.0)
    b, c, d = -axis * np.sin(angle / 2.0)
    aa, bb, cc, dd = a * a, b * b, c * c, d * d
    bc, ad, ac, ab, bd, cd = b * c, a * d, a * c, a * b, b * d, c * d
    return np.array([[aa + bb - cc - dd, 2 * (bc + ad), 2 * (bd - ac), 0],
                     [2 * (bc - ad), aa + cc - bb - dd, 2 * (cd + ab), 0],
                     [2 * (bd + ac), 2 * (cd - ab), aa + dd - bb - cc, 0],
                     [0, 0, 0, 1]], dtype=np.float32)

def translation_matrix(translation: np.ndarray) -> np.ndarray:
    return np.array([
        [1, 0, 0, translation[0]],
        [0, 1, 0, translation[1]],
        [0, 0, 1, translation[2]],
        [0, 0, 0, 1],
    ])