import unittest
from battery_simulator import BatterySimulator

class TestBatterySimulator(unittest.TestCase):

    def setUp(self):
        """Initialize the simulator before each test."""
        self.sim = BatterySimulator()

    def test_discharge_step(self):
        """Test a single discharge step with positive current."""
        soc_before, _, _ = self.sim.step(0, 0)  # get initial SOC
        soc_after, current, voltage = self.sim.step(current=5.0, duration=1.0)

        self.assertTrue(0 <= soc_after <= 1, "SOC should remain in [0, 1]")
        self.assertLess(soc_after, soc_before, "SOC should decrease on discharge")
        self.assertAlmostEqual(current, 5.0, msg="Current should match input")
        self.assertTrue(2.5 <= voltage <= 4.2, "Voltage should be in valid range")

    def test_charge_step(self):
        """Test a single charge step with negative current."""
        soc_before, _, _ = self.sim.step(0, 0)
        soc_after, current, voltage = self.sim.step(current=-3.0, duration=1.0)

        self.assertTrue(0 <= soc_after <= 1, "SOC should remain in [0, 1]")
        self.assertGreater(soc_after, soc_before, "SOC should increase on charging")
        self.assertAlmostEqual(current, -3.0, msg="Current should match input")
        self.assertTrue(2.5 <= voltage <= 4.2, "Voltage should be in valid range")

    def test_soc_bounds(self):
        """Ensure SOC stays within [0, 1] over multiple steps."""
        for _ in range(100):
            soc, _, _ = self.sim.step(current=5, duration=10)
        self.assertGreaterEqual(soc, 0.0)
        self.assertLessEqual(soc, 1.0)

    def test_multiple_steps(self):
        """Check if multiple steps accumulate correctly."""
        soc1, _, _ = self.sim.step(current=5, duration=1)
        soc2, _, _ = self.sim.step(current=5, duration=1)
        self.assertLess(soc2, soc1, "SOC should continue to decrease with repeated discharge")

if __name__ == '__main__':
    unittest.main()
