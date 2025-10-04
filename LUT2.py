import numpy as np
from scipy.io import wavfile
import matplotlib.pyplot as plt

# Load wav file
fs, data = wavfile.read("piano.wav")

# If stereo, take one channel
if data.ndim > 1:
    data = data[:, 0]

# Normalize to 0–1
data = data / np.max(np.abs(data))

# Scale to 12-bit DAC range (0–4095)
data = (data + 1) * 2047.5  # shift from [-1,1] to [0,4095]

# Downsample to 128 points
lut_size = 20000
indices = np.linspace(0, len(data)-1, lut_size, dtype=int)
lut = data[indices].astype(int)

# Plot to check
plt.plot(lut)
plt.title("LUT from wav file")
plt.show()

# Print out as C array
print("{", ", ".join(map(str, lut)), "}")
