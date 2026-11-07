import matplotlib.pyplot as plt
# import numpy.polynomial.polyutils
import pandas as pd
import numpy as np
import glob

# Constants
FS = 1000
TIME = 2000 # ms

# Helper funcs
def get_plant(inputString):
    result = inputString.split("_")
    match result(1):
            case "uw":
                return "unweighted"
            case "w" : 
                return "weighted"

def get_kp(inputString):
    result = inputString.split("_")
    match result(2):
            case "8":
                return 0.08
            case "4" : 
                return 0.04
            case "1" :
                return 0.01

# Create np array vector
t = np.arange(0, 2000)

# Organize files
file_list = sorted(glob.glob("/Lab7_Pcontrol_Automated/data/*.txt"))

print(file_list)





