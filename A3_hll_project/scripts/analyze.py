import argparse
import math
import os

import pandas as pd
import matplotlib.pyplot as plt

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", default="results.csv")
    ap.add_argument("--outdir", default="out")
    ap.add_argument("--B", type=int, default=12)
    args = ap.parse_args()

    df = pd.read_csv(args.csv)

    g = df.groupby("step_idx")
    agg = g.agg(
        prefix_len=("prefix_len", "first"),
        true_mean=("true_F0", "mean"),
        est_mean=("estimate_N", "mean"),
        est_std=("estimate_N", "std"),
    ).reset_index()

    m = 2 ** args.B
    rse_104 = 1.042 / math.sqrt(m)
    rse_132 = 1.32  / math.sqrt(m)

    os.makedirs(args.outdir, exist_ok=True)

    df0 = df[df["stream_id"] == 0].sort_values("step_idx")
    plt.figure()
    plt.plot(df0["prefix_len"], df0["true_F0"], label="True F0_t")
    plt.plot(df0["prefix_len"], df0["estimate_N"], label="HLL N_t")
    plt.xlabel("Processed prefix size |S_t|")
    plt.ylabel("Distinct count")
    plt.title("Graph #1: True F0_t vs HLL estimate N_t (stream 0)")
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(args.outdir, "graph1_true_vs_est_stream0.png"), dpi=200)
    plt.close()

    x = agg["prefix_len"].to_numpy(dtype=float)
    mean = agg["est_mean"].to_numpy(dtype=float)
    std = agg["est_std"].to_numpy(dtype=float)
    lower = mean - std
    upper = mean + std

    plt.figure()
    plt.plot(x, mean, label="E[N_t]")
    plt.fill_between(x, lower, upper, alpha=0.2, label="± sigma")
    plt.xlabel("Processed prefix size |S_t|")
    plt.ylabel("Estimated distinct count")
    plt.title("Graph #2: E[N_t] with ± sigma")
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(args.outdir, "graph2_mean_pm_std.png"), dpi=200)
    plt.close()

    agg["abs_rel_err_mean"] = ((agg["est_mean"] - agg["true_mean"]) / agg["true_mean"]).abs()

    plt.figure()
    plt.plot(x, agg["abs_rel_err_mean"].to_numpy(dtype=float), label="|E[N_t]-F0_t|/F0_t")
    plt.plot(x, [rse_104]*len(agg), label="1.042/sqrt(m)")
    plt.plot(x, [rse_132]*len(agg), label="1.32/sqrt(m)")
    plt.xlabel("Processed prefix size |S_t|")
    plt.ylabel("Abs. relative error / RSE")
    plt.title("Graph #3: Mean error vs theory")
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(args.outdir, "graph3_error_vs_theory.png"), dpi=200)
    plt.close()

    agg.to_csv(os.path.join(args.outdir, "aggregated_stats.csv"), index=False)

if __name__ == "__main__":
    main()
