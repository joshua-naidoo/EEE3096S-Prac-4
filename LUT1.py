import numpy as np
import matplotlib.pyplot as plt

ns = 128
x = np.arange(ns)
sine = 2047.5 * (1 + np.sin(2 * np.pi * x / ns))  # Centered at 2048, ranges 0-4095
saw = 4095 * x / (ns - 1)  # Linear ramp from 0 to 4095
tri = np.concatenate((4095 * x[:ns//2] / (ns//2 - 1), 4095 * (1 - x[:ns//2+1] / (ns//2 - 1))[1:]))  # Triangular wave

# Round to integers for 12-bit resolution
sine = np.round(sine).astype(int)
saw = np.round(saw).astype(int)
tri = np.round(tri).astype(int)

# Plot to verify
plt.figure(figsize=(10, 6))
plt.plot(x, sine, label='Sine')
plt.plot(x, saw, label='Sawtooth')
plt.plot(x, tri, label='Triangular')
plt.xlabel('Sample Index')
plt.ylabel('Value (0-4095)')
plt.title('Waveform Lookup Tables')
plt.legend()
plt.grid(True)
plt.show()

# Output LUTs as C arrays
print("uint16_t sine_lut[128] = {" + ", ".join(map(str, sine)) + "};")
print("uint16_t saw_lut[128] = {" + ", ".join(map(str, saw)) + "};")
print("uint16_t tri_lut[128] = {" + ", ".join(map(str, tri)) + "};")