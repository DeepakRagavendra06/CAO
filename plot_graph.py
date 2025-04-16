import matplotlib.pyplot as plt
import pandas as pd

# Read CSV
data = pd.read_csv('timings.csv', names=["Algorithm", "Threads", "Time"])

# Group by algorithm
for algo in data["Algorithm"].unique():
    subset = data[data["Algorithm"] == algo]
    plt.plot(subset["Threads"], subset["Time"], marker='o', label=algo)

plt.title("Performance of Parallel Sorting Algorithms")
plt.xlabel("Number of Threads")
plt.ylabel("Time (seconds)")
plt.grid(True)
plt.legend()
plt.show()

