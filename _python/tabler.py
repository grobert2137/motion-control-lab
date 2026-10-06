import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

class Tabler():
    @staticmethod
    def create():
        data = []
        while True:
            x = input("For: ")
            if x == "end":
                break
            y = input("Value: ")
            data.append([x, y])
            print([x, y])

        # Convert to DataFrame to display
        df = pd.DataFrame(data, columns=["Input", "Output"])
        print(df)

        # Prompt for plot
        choice = input("Plot table?")
        if choice == "y":
            Tabler.plot(data)
        else:
            print("Closing...")

    @staticmethod
    def plot(data):
        arr = np.array(data, dtype=float)
        plt.plot(arr[:, 0], arr[:, 1])
        plt.xlabel("Input")
        plt.ylabel("Output")
        plt.title("TABLE PLOTTED")