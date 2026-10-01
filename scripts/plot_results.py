import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results.csv")
df["time_ms"] = df["time_ns"] / 1e6

def plot_experiment(experiment, title, filename):
    data = df[df["experiment"] == experiment]
    plt.figure(figsize=(7, 5))
    for algo, group in data.groupby("algorithm"):
        group = group.sort_values("text_size")
        plt.plot(group["text_size"], group["time_ms"], marker="o", label=algo)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Text length (characters, log scale)")
    plt.ylabel("Median runtime (ms, log scale)")
    plt.title(title)
    plt.legend()
    plt.grid(True, which="both", alpha=0.3)
    plt.tight_layout()
    plt.savefig(f"results/{filename}", dpi=150)
    plt.close()

plot_experiment("text_size", "Random text (alphabet 26, pattern length 20)", "runtime_random.png")

plot_experiment("repetitive_text", "Repetitive text (AAA...B, pattern AAA...B, length 20)", "runtime_repetitive.png")

# Speedup of KMP over naive for each input type
pivot = df.pivot_table(index=["experiment", "text_size"],
                       columns="algorithm", values="time_ns").reset_index()
pivot["speedup"] = pivot["Naive"] / pivot["KMP"]

plt.figure(figsize=(7, 5))
for experiment, group in pivot.groupby("experiment"):
    group = group.sort_values("text_size")
    plt.plot(group["text_size"], group["speedup"], marker="o", label=experiment)
plt.axhline(1, color="grey", linestyle="--", linewidth=1)
plt.xscale("log")
plt.xlabel("Text length (characters, log scale)")
plt.ylabel("Naive time / KMP time")
plt.title("KMP speedup over naive (above 1 = KMP faster)")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("results/speedup.png", dpi=150)
plt.close()

print("Saved plots to results/")
