from scipy.io import wavfile
import numpy as np
from scipy.signal import resample
import matplotlib.pyplot as plt

filename = "piano.wav"  # Full file path
fs, data = wavfile.read(filename)

print(f"Sample rate: {fs} Hz")
print(f"Original data length: {len(data)}")
print(f"Data min: {data.min()}, max: {data.max()}")

if data.ndim > 1:
    data = data[:, 0]

# Convert to float and normalize to 0-4095 range
data = data.astype(float)
data = data - np.mean(data)  # Remove DC offset
data = data * (2047.5 / (np.max(np.abs(data)) + 1e-6)) + 2047.5  # Scale to 0-4095
data = np.clip(data, 0, 4095)
print(f"Normalized min: {data.min()}, max: {data.max()}")

# Find the segment with the highest energy (e.g., 4410 samples ~0.1s)
window_size = 4410  # ~0.1s at 44100 Hz
energy = np.array([np.sum(data[i:i+window_size]**2) for i in range(len(data)-window_size)])
start_idx = np.argmax(energy)
segment = data[start_idx:start_idx + window_size]

# Resample the segment to 128 points
if len(segment) > 128:
    segment = resample(segment, 128)
elif len(segment) < 128:
    segment = np.pad(segment, (0, 128 - len(segment)), 'constant')

lut = np.round(segment).astype(int)
print(f"Final LUT length: {len(lut)}")
print(f"Final LUT min: {lut.min()}, max: {lut.max()}")

# Plot the waveform
plt.figure(figsize=(10, 6))
plt.plot(lut)
plt.xlabel('Sample Index')
plt.ylabel('Value (0-4095)')
plt.title(f'Waveform from {filename}')
plt.grid(True)
plt.show()

print(f"uint16_t {filename.split('.')[0]}_lut[128] = {{ {', '.join(map(str, lut))} }};")