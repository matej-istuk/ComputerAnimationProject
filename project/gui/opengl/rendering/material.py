from typing import Tuple


class Material:
    def __init__(self,
                 ambient_coefficient: Tuple[float, float, float] = None,
                 diffuse_coefficient: Tuple[float, float, float] = None,
                 specular_coefficient: Tuple[float, float, float] = None,
                 shininess_exponent: float = None):
        if ambient_coefficient is None:
            ambient_coefficient = (0.5, 0.5, 0.5)
        if diffuse_coefficient is None:
            diffuse_coefficient = (0.5, 0.5, 0.5)
        if specular_coefficient is None:
            specular_coefficient = (0.5, 0.5, 0.5)
        if shininess_exponent is None:
            shininess_exponent = 0.5
        self.ambient_coefficient = ambient_coefficient
        self.diffuse_coefficient = diffuse_coefficient
        self.specular_coefficient = specular_coefficient
        self.shininess_exponent = shininess_exponent