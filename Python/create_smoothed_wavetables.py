# Created with AI

import numpy as np

# Configuration Constants
SAMPLES = 2048
FILE_NAME = "smoothed_square_wavetable.S"
TRANSITION_WIDTH = 64  # Multi-sample smoothing window for extreme alias rejection

# 1. Generate the Blackman-Harris integrated step profile (0.0 to 1.0)
# This mathematical window is famous for providing >92dB of side-lobe attenuation
alpha = 0.16
a0 = 0.35875
a1 = 0.48829
a2 = 0.14128
a3 = 0.01168

n = np.arange(TRANSITION_WIDTH)
# Manual Blackman-Harris window calculation to avoid external scipy dependency
window = (a0 
          - a1 * np.cos(2 * np.pi * n / (TRANSITION_WIDTH - 1)) 
          + a2 * np.cos(4 * np.pi * n / (TRANSITION_WIDTH - 1)) 
          - a3 * np.cos(6 * np.pi * n / (TRANSITION_WIDTH - 1)))

window /= np.sum(window)  # Normalize so the integral equals exactly 1.0
edge_profile = np.cumsum(window)

# 2. Initialize a perfect geometric square wave (-1.0 to 1.0)
wave = np.ones(SAMPLES)
wave[SAMPLES // 2 :] = -1.0

half_width = TRANSITION_WIDTH // 2
midpoint = SAMPLES // 2

# 3. Apply the high-rejection smoothing curve to the transitions
for i in range(-half_width, half_width):
    # Smooth the first rising/falling transition at index 0
    idx_0 = i % SAMPLES
    wave[idx_0] = -1.0 + 2.0 * edge_profile[i + half_width]
    
    # Smooth the second inversion transition at the midpoint
    idx_mid = (midpoint + i) % SAMPLES
    wave[idx_mid] = 1.0 - 2.0 * edge_profile[i + half_width]

# 4. Normalize and scale to signed 16-bit range (-32768 to 32767)
wave_normalized = wave / np.max(np.abs(wave))
wavetable = np.round(wave_normalized * 32767).astype(np.int16)

# 5. Format assembly file content with strict structural requirements
assembly_lines = []
for i in range(0, SAMPLES, 8):
    data_chunk = wavetable[i:i+8]
    # Format values as standard signed decimal strings
    decimal_values = [str(val) for val in data_chunk]
    # Exactly 8 leading spaces, followed by .hword directive
    formatted_line = "        .hword " + ", ".join(decimal_values)
    assembly_lines.append(formatted_line)

# 6. Save directly to an assembly source (.S) file
with open(FILE_NAME, "w") as asm_file:
    asm_file.write("\n".join(assembly_lines) + "\n")

print(f"Success! High-rejection signed decimal wavetable saved to {FILE_NAME}")
