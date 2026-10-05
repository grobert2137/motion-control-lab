#%%
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import math

# Constants
A1 = 0.846
B1 = 0.154

A2 = 0.776
B2 = 0.224

FS = 750

# Create magnitude list
magFoIRR = []
magSoIRR = []

# Initialize Array
f = np.arange(0, 151, 5)
wt = 2*math.pi*f/FS

def first_order_iir (input):
    return B1 / math.sqrt(1-(2*A1*math.cos(input))+pow(A1, 2))

def second_order_iir (input):
    return (pow(B2, 2) / (1-(2*A2*math.cos(input))+pow(A2, 2)))


for value in wt:
    H_fo = first_order_iir(value)
    H_so = second_order_iir(value)
    magFoIRR.append(H_fo)
    magSoIRR.append(H_so)

magFoIRR = np.array(magFoIRR)
magSoIRR = np.array(magSoIRR)

# Table Output
table = pd.DataFrame({"Frquency" : f, "Magnitude": magFoIRR})
print(table)

# Plotting
plt.plot(f, magFoIRR, label = "First Order") 
plt.plot(f, magSoIRR, label = "Second Order")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude")
plt.title("IIR Filtering")
plt.legend() 
plt.grid(False)


# %%
