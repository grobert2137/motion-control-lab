import numpy as np
import matplotlib.pyplot as plt

# Measured data
freq_Hz = np.array([5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 125, 150])
first_order_mag = np.array([0.982, 0.908, 0.812, 0.725, 0.641, 0.572, 0.460, 0.386, 0.343, 0.303, 0.273, 0.249, 0.233, 0.199, 0.179])
second_order_mag = np.array([0.992, 0.849, 0.682, 0.538, 0.428, 0.339, 0.238, 0.180, 0.156, 0.139, 0.131, 0.123, 0.120, 0.114, 0.112])

# Constants
A1, B1 = 0.846, 0.154
A2, B2 = 0.776, 0.224
FS = 750

# Predicted response
f = np.arange(0, 151, 5)
wt = 2 * np.pi * f / FS

def first_order_iir(w):
    return B1 / np.sqrt(1 - 2 * A1 * np.cos(w) + A1**2)

def second_order_iir(w):
    return B2**2 / (1 - 2 * A2 * np.cos(w) + A2**2)

mag_fo_pred = first_order_iir(wt)
mag_so_pred = second_order_iir(wt)

# Plotting
fig, ax = plt.subplots(figsize=(8, 5))

# Measured (solid, with markers)
ax.plot(freq_Hz, first_order_mag, color='tab:blue', linewidth=1.5, label='First order (measured)')
ax.plot(freq_Hz, second_order_mag, color='tab:orange', linewidth=1.5, label='Second order (measured)')

# Predicted (dashed)
ax.plot(f, mag_fo_pred, '--', color='tab:blue', linewidth=1.5, label='First order (predicted)')
ax.plot(f, mag_so_pred, '--', color='tab:orange', linewidth=1.5, label='Second order (predicted)')

# -3 dB level (DC gain of 1)
#ax.axhline(0.7071, color='gray', linestyle=':', linewidth=1, label='0.7071 (-3 dB)')

ax.set_xlabel('Frequency (Hz)')
ax.set_ylabel('Magnitude')
ax.set_title('IIR: Measured vs. Predicted')
ax.grid(True)
ax.legend()

plt.tight_layout()
plt.show()