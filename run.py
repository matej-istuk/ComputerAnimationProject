import pyglet

from pygletgui import SimulationWindow
from simulation import Simulation

simulation = Simulation((10, 15))

try:
    config = pyglet.gl.Config(sample_buffers=1, samples=4,
                    depth_size=16, double_buffer=True, )
    main_window = SimulationWindow(simulation, config=config)
except pyglet.window.NoSuchConfigException:
    main_window = SimulationWindow(simulation)

pyglet.app.run()
