import numpy as np


def look_at(eye, center, up):
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

def create_perspective_matrix(left, right, bottom, top, near, far):
   return np.array([
       [2*near/(right-left), 0, (right+left)/(right-left), 0],
       [0, 2*near/(top-bottom), (top+bottom)/(top-bottom), 0],
       [0, 0, (near+far)/(near-far), 2*near*far/(near-far)],
       [0, 0, -1, 0]
   ], dtype=np.float32)

def create_perspective_matrix_2(fov_deg, aspect_ratio, near, far):
   y_max = near*np.tan(fov_deg * np.pi / 360)
   x_max = y_max * aspect_ratio
   return create_perspective_matrix(-x_max, x_max, -y_max, y_max, near, far)

def rotate_x(angle):
    return np.array([
        [1, 0, 0, 0],
        [0, np.cos(angle), -np.sin(angle), 0],
        [0, np.sin(angle), np.cos(angle), 0],
        [0, 0, 0, 1]
    ], dtype=np.float32)

def rotate_y(angle):
    return np.array([
        [np.cos(angle), 0, -np.sin(angle), 0],
        [0, 1, 0, 0],
        [np.sin(angle), 0, np.cos(angle), 0],
        [0, 0, 0, 1]
    ], dtype=np.float32)

def rotate_z(angle):
    return np.array([
        [np.cos(angle), -np.sin(angle), 0, 0],
        [np.sin(angle), np.cos(angle), 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ], dtype=np.float32)
