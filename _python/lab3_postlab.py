
#%%
import matplotlib.pyplot as plt
import pandas as pd

data = {
    'Voltage In': [-9, -1.5, 2.5, 6.0],
    'Voltage Out': [-5.0, -1.525, 2.640, 5.0],
    'Digital Value': [-32768, -9994, 17301, 32767]
}


df = pd.DataFrame(data)
display(df)
plt.table(df)
# %%
