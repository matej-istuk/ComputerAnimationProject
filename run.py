import pyglet

from gui.opengl.app import SimulationWindow
from simulation import Simulation, Fabric

fabric = Fabric((10, 15))
simulation = Simulation(fabric)

fabric.set_static(0, 0)
fabric.set_static(-1, 0)
fabric.set_static(0, -1)
fabric.set_static(-1, -1)

try:
    config = pyglet.gl.Config(sample_buffers=1, samples=4,
                    depth_size=16, double_buffer=True, )
    main_window = SimulationWindow(simulation, config=config)
except pyglet.window.NoSuchConfigException:
    main_window = SimulationWindow(simulation)

pyglet.app.run()
