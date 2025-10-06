import numpy as np
from scipy.io import wavfile
import matplotlib.pyplot as plt

# Load wav file
fs, data = wavfile.read("guitar.wav")

# If stereo, take one channel
if data.ndim > 1:
    data = data[:, 0]

# Convert to float
data = data.astype(float)

# Normalize to -1 to +1 range
data = data / np.max(np.abs(data))

# Scale to 12-bit DAC range (0–4095)
data = (data + 1) * 2047.5  # shift from [-1,1] to [0,4095]
data = np.clip(data, 0, 4095)  # ensure values stay in range

# Choose LUT size based on what fits in memory
lut_size = 5000  # 1 second at 44.1kHz - will fit in Flash

# Downsample or truncate
if len(data) > lut_size:
    # Take first 1 second of audio
    lut = data[:lut_size].astype(int)
else:
    lut = data.astype(int)

# Plot to check
plt.figure(figsize=(12, 4))
plt.plot(lut)
plt.title(f"Piano LUT - {lut_size} samples ({lut_size/fs:.2f} seconds)")
plt.xlabel("Sample")
plt.ylabel("Value (0-4095)")
plt.grid(True)
plt.show()

# Print statistics to console
print(f"Sample rate: {fs} Hz")
print(f"LUT size: {lut_size} samples")
print(f"Duration: {lut_size/fs:.2f} seconds")
print(f"Memory required: {lut_size * 4 / 1024:.1f} KB")
print(f"Min value: {lut.min()}, Max value: {lut.max()}")

# Write to text file
output_filename = "piano_lut.txt"
with open(output_filename, 'w') as f:
    f.write(f"// Piano LUT - {lut_size} samples, {lut_size/fs:.2f} seconds at {fs} Hz\n")
    f.write(f"// Memory required: {lut_size * 4 / 1024:.1f} KB\n")
    f.write(f"const uint32_t Piano_LUT[{lut_size}] = {{\n")
    
    for i, val in enumerate(lut):
        if i % 16 == 0:
            f.write("    ")
        f.write(f"{val}")
        if i < len(lut) - 1:
            f.write(", ")
        if (i + 1) % 16 == 0:
            f.write("\n")
    
    f.write("\n};\n")

print(f"\nC array written to {output_filename}")