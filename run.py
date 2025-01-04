import hydra
from hydra.utils import instantiate
from omegaconf import DictConfig
import pyglet

from gui.opengl.app import SimulationWindow
from simulation.simulation import Simulation, Fabric

@hydra.main(version_base=None, config_path="conf", config_name="basic")
def main(cfg: DictConfig):
    fabric = instantiate(cfg.fabric)
    simulation = instantiate(cfg.simulation, fabric=fabric)

    for i in range(0, fabric.points.shape[0], 3):
        fabric.set_static(i, 0)

    try:
        config = pyglet.gl.Config(sample_buffers=1, samples=4,
                        depth_size=16, double_buffer=True, )
        SimulationWindow(simulation, config=config)
    except pyglet.window.NoSuchConfigException:
        SimulationWindow(simulation)

    pyglet.app.run()

if __name__ == '__main__':
    main()