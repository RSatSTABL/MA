import pybamm
import numpy as np

class BatterySimulator:
    def __init__(self):
        self.model = pybamm.lithium_ion.DFN()
        self.param = self.model.default_parameter_values
        self.param.update({"Current function [A]": pybamm.InputParameter("Current")})
        self.sim = pybamm.Simulation(self.model, parameter_values=self.param)
        self.sim.solve([0, 1e-6], inputs={"Current": 0})
        self.solution = self.sim.solution
        self.time = 0

    def step(self, current, duration):
        """Simulate one step with given current (A) and duration (s)."""
        # Set the external current as input parameter only once in __init__
        t_eval = np.linspace(self.time, self.time + duration, 2)
        solution = self.sim.solve(t_eval=t_eval, inputs={"Current": current})
        self.time += duration
        self.solution = solution
        soc = float(solution["State of Charge"].entries[-1])
        voltage = float(solution["Terminal voltage [V]"].entries[-1])
        return soc, current, voltage
    

# Load the model
model = pybamm.lithium_ion.DFN()

# Create simulation instance
sim = pybamm.Simulation(model)

# Solve the model
sim.solve([0, 3600])

