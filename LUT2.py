from scipy.io import wavfile
import numpy as np
from scipy.signal import resample
import matplotlib.pyplot as plt
import os

# --- Configuration ---
filename = "piano.wav"  # IMPORTANT: Ensure this file is in the same directory
N_S = 20000             # Target look-up table size (as requested)
DAC_RESOLUTION = 12     # DAC Resolution (0 to 4095)
MAX_DAC_VALUE = 2**DAC_RESOLUTION - 1
HALF_DAC_VALUE = MAX_DAC_VALUE / 2

# --- File Loading and Check ---
if not os.path.exists(filename):
    print(f"Error: The file '{filename}' was not found.")
    print("Please make sure the audio file is in the same directory as this script.")
    exit()

fs, data = wavfile.read(filename)

print(f"--- Loaded File: {filename} ---")
print(f"Sample rate: {fs} Hz")
print(f"Original data length: {len(data)}")
print(f"Data min: {data.min()}, max: {data.max()}")

# Convert stereo to mono if necessary (take the first channel)
if data.ndim > 1:
    data = data[:, 0]
    print("Converted stereo to mono (channel 0).")

# --- Normalization and Scaling ---
# Convert to float for processing
data = data.astype(float)
# 1. Remove DC offset (center around 0)
data = data - np.mean(data)
# 2. Scale to the 0-4095 range
# Find the maximum absolute value to ensure no clipping during scaling
max_amplitude = np.max(np.abs(data))
if max_amplitude == 0:
    max_amplitude = 1e-6 # Avoid division by zero

# Scale data to fit in [-HALF_DAC_VALUE, HALF_DAC_VALUE] and then shift up
data = data * (HALF_DAC_VALUE / max_amplitude) + HALF_DAC_VALUE

# 3. Clip to ensure values stay within 0 and 4095
data = np.clip(data, 0, MAX_DAC_VALUE)
print(f"Normalized min: {data.min():.2f}, max: {data.max():.2f}")

# --- Segmenting and Resampling ---

# Find the segment with the highest energy (e.g., ~0.1s of audio, to focus on the attack)
# This finds a segment that is likely to contain the 'meat' of the sound.
segment_duration_s = 0.1
window_size = int(fs * segment_duration_s) 

if len(data) < window_size:
    # If the file is too short, use the whole file
    print(f"Warning: File is shorter than {window_size} samples. Using entire file.")
    segment = data
else:
    # Calculate energy over all possible windows
    energy = np.array([np.sum(data[i:i+window_size]**2) 
                       for i in range(len(data) - window_size)])
    start_idx = np.argmax(energy)
    segment = data[start_idx:start_idx + window_size]
    print(f"Extracted high-energy segment (Duration: {segment_duration_s}s) starting at index {start_idx}.")


# Resample the extracted segment to the target size (N_S)
# This resamples the focused sound clip to exactly 20000 points.
lut_float = resample(segment, N_S)

# Round to the nearest integer and cast to the required C data type (uint16_t)
lut = np.round(lut_float).astype(np.uint16)

print(f"Final LUT length: {len(lut)}")
print(f"Final LUT min: {lut.min()}, max: {lut.max()}")

# --- Plotting ---
plt.figure(figsize=(10, 6))
plt.plot(lut)
plt.xlabel(f'Sample Index (Total {N_S} points)')
plt.ylabel(f'Value (0 to {MAX_DAC_VALUE})')
plt.title(f'Resampled Waveform from {filename} ({N_S} Samples)')
plt.grid(True)
plt.show()

# --- C Code Output ---
# Clean up filename for variable name
var_name = filename.split('.')[0].replace('-', '_').lower()

print("\n--- C Array Output (Copy to your main.c file) ---")
# Generate the C array definition string
lut_string = ', '.join(map(str, lut))
c_array_output = (
    f"// Audio file: {filename} (Original Fs: {fs} Hz)\n"
    f"// LUT Size: {N_S} samples. DAC Resolution: 12-bit.\n"
    f"uint16_t {var_name}_lut[{N_S}] = {{ {lut_string} }};"
)

# Printing the ENTIRE array to the console
print(c_array_output)
