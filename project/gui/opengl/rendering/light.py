from typing import Tuple

from project.gui.opengl.rendering.object3d import Object3D


class Light(Object3D):
    def __init__(self,
                 intensity: float = None,
                 position: Tuple[float, float, float, float] = None,
                 ambient_intensity: Tuple[float, float, float] = None,
                 diffuse_intensity: Tuple[float, float, float] = None,
                 specular_intensity: Tuple[float, float, float] = None):
        super().__init__(position=position)
        if intensity is None:
            intensity = 0.5
        if ambient_intensity is None:
            ambient_intensity = (intensity, intensity, intensity)
        if diffuse_intensity is None:
            diffuse_intensity = (intensity, intensity, intensity)
        if specular_intensity is None:
            specular_intensity = (intensity, intensity, intensity)
        self.ambient_intensity = ambient_intensity
        self.diffuse_intensity = diffuse_intensity
        self.specular_intensity = specular_intensity
