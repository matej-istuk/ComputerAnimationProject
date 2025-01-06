import os

import hydra
from hydra.utils import instantiate
from omegaconf import DictConfig
import pyglet

from gui.opengl.app import SimulationWindow
from gui.opengl.rendering.material import Material
from simulation.simulation import Simulation, Fabric
from simulation.util import StaticPointSetter


@hydra.main(version_base=None, config_path="conf", config_name="config")
def main(cfg: DictConfig):
    fabric: Fabric = instantiate(cfg.fabric)
    simulation: Simulation = instantiate(cfg.simulation, fabric=fabric)
    material: Material = instantiate(cfg.material)
    static_point_setter: StaticPointSetter = instantiate(cfg.static_point_setter)
    static_point_setter.set_statics(fabric)

    try:
        config = pyglet.gl.Config(sample_buffers=1, samples=4,
                        depth_size=16, double_buffer=True)
        instantiate(cfg.app, simulation=simulation, fabric_material=material, app_config=config)
    except pyglet.window.NoSuchConfigException:
        instantiate(cfg.app, simulation=simulation, fabric_material=material)

    pyglet.app.run()

if __name__ == '__main__':
    os.environ['HYDRA_FULL_ERROR'] = '1'
    main()