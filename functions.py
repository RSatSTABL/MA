import pybamm
import numpy as np


def create_simple_current_profile(duration=3600, step=600, current_values=[1, -1, 0]):
    """
    Create a simple current profile for testing.

    Parameters:
        duration: int
            Total duration of the profile in seconds.
        step: int
            Duration of each current step in seconds.
        current_values: list
            List of current values (A) for each step.

    Returns:
        np.ndarray: Array of (time [s], current [A]) pairs.
    """
    times = np.arange(0, duration + step, step)
    currents = np.resize(current_values, times.shape)
    profile = np.column_stack((times, currents))
    return profile




def apply_current_profile(model, current_profile, time_resolution=1):
    """
    Simulate a battery model with a given current profile.

    Parameters:
        model: pybamm.BaseModel
            The PyBaMM battery model to simulate.
        current_profile: np.ndarray or list of tuples
            Array or list of (time [s], current [A]) pairs.
        time_resolution: float, optional
            Time step for output in seconds (default: 1).

    Returns:
        dict: {
            "time": np.ndarray,
            "current": np.ndarray,
            "voltage": np.ndarray,
            "soc": np.ndarray
        }
    """
    # Ensure current_profile is a numpy array
    current_profile = np.array(current_profile)
    times = current_profile[:, 0]
    currents = current_profile[:, 1]

    # Create interpolant for current
    current_interp = pybamm.Interpolant(times, currents, pybamm.t, interpolator="linear")
    model.variables["Current function"] = current_interp

    # Set up and solve
    sim = pybamm.Simulation(model)
    t_eval = np.arange(times[0], times[-1] + time_resolution, time_resolution)
    sim.solve(t_eval=t_eval)

    

    # Extract results
    solution = sim.solution
    time = solution["Time [s]"].entries
    current = solution["Current [A]"].entries
    voltage = solution["Terminal voltage [V]"].entries
    soc = solution["State of Charge"].entries


    return {
        "time": time,
        "current": current,
        "voltage": voltage,
        "soc": soc
    }



