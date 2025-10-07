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

# DOWNSAMPLE by 4x (44.1kHz -> 11.025kHz)
# This is still good quality for a small speaker
downsample_factor = 64
data_downsampled = data[::downsample_factor]
effective_fs = fs / downsample_factor

# Scale to 12-bit DAC range (0–4095)
data_downsampled = (data_downsampled + 1) * 2047.5
data_downsampled = np.clip(data_downsampled, 0, 4095)

# Convert to integers
lut = data_downsampled.astype(int)
lut_size = len(lut)

# Plot full waveform
plt.figure(figsize=(14, 4))
plt.plot(lut)
plt.title(f"Piano LUT - {lut_size} samples ({lut_size/effective_fs:.2f} seconds at {effective_fs:.0f} Hz)")
plt.xlabel("Sample")
plt.ylabel("Value (0-4095)")
plt.grid(True)
plt.tight_layout()
plt.show()

# Plot zoomed section
plt.figure(figsize=(14, 4))
plt.plot(lut[:1000])
plt.title(f"Piano LUT Detail (first {1000/effective_fs:.3f} seconds)")
plt.xlabel("Sample")
plt.ylabel("Value (0-4095)")
plt.grid(True)
plt.tight_layout()
plt.show()

# Print statistics
print(f"Original sample rate: {fs} Hz")
print(f"Downsampled rate: {effective_fs:.0f} Hz")
print(f"Downsample factor: {downsample_factor}x")
print(f"LUT size: {lut_size} samples")
print(f"Duration: {lut_size/effective_fs:.2f} seconds")
print(f"Memory required: {lut_size * 4 / 1024:.1f} KB ({lut_size * 4 / (1024*1024):.3f} MB)")
print(f"Min value: {lut.min()}, Max value: {lut.max()}")

# Memory check
flash_size_kb = 512  # Typical STM32F4
if (lut_size * 4 / 1024) < flash_size_kb * 0.6:
    print(f"\n✓ LUT should fit comfortably in {flash_size_kb}KB Flash")
else:
    print(f"\n⚠ WARNING: LUT may be tight for {flash_size_kb}KB Flash")

# Write to text file
output_filename = "piano_lut.txt"
with open(output_filename, 'w') as f:
    f.write(f"// Piano LUT - {lut_size} samples, {lut_size/effective_fs:.2f}s at {effective_fs:.0f} Hz (downsampled {downsample_factor}x)\n")
    f.write(f"// Memory required: {lut_size * 4 / 1024:.1f} KB\n")
    f.write(f"// Original sample rate: {fs} Hz\n\n")
    f.write(f"const uint32_t Piano_LUT[{lut_size}] = {{\n")
    
    for i, val in enumerate(lut):
        if i % 16 == 0:
            f.write("    ")
        f.write(f"{val}")
        if i < len(lut) - 1:
            f.write(", ")
        if (i + 1) % 16 == 0:
            f.write("\n")
    
    if lut_size % 16 != 0:
        f.write("\n")
    f.write("};\n")
    
    # Also output the number of samples as a #define
    f.write(f"\n#define PIANO_LUT_SIZE {lut_size}\n")

print(f"\nC array written to {output_filename}")