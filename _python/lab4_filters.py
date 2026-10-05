#%%
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math

# Constants
A1 = 0.846
B1 = 0.154
FS = 750

# Create magnitude list
mag = []

# Initialize Array
f = np.arange(0, 151, 5)
wt = 2*math.pi*f/FS

for value in wt:
    H = B1 / math.sqrt(1-2*A1*math.cos(value)+pow(A1, 2))
    mag.append(H)

mag = np.array(mag)

# Table Output
table = pd.DataFrame({"Frquency" : f, "Magnitude": mag})
print(table)

# Plotting
plt.plot(f, mag) 
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude")
plt.title("IRR Filtering")
plt.legend("First Order") 


# %%
