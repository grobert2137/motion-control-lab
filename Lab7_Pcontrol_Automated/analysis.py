import matplotlib.pyplot as plt
# import numpy.polynomial.polyutils
import pandas as pd
import numpy as np

# Constants
FS = 1000

a = pd.Series(pd.read_csv("Lab7_Pcontrol_Automated/ex1_uw_8.txt"))
b = pd.Series(pd.read_csv("Lab7_Pcontrol_Automated/ex1_uw_4.txt"))
c = pd.Series(pd.read_csv("Lab7_Pcontrol_Automated/ex1_uw_1.txt"))

#plt.plot(g);
#plt.show();
print(a)

#uwData = pd.DataFrame(data, columns=["Kp = 0.008", "Kp = 0.004", "Kp = 0.001"])

df = pd.DataFrame(columns=["Plant", "Setpoint", "SS", "SS_Error"])


print(df)
#print(uwData)
