#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "random_stream_gen.h"
#include "hyperloglog.h"
#include "exact_counter.h"

// формат цсв:
// stream_id,step_idx,prefix_len,true_F0,estimate_N
static void write_csv_row(std::ofstream& out,
                          int stream_id,
                          int step_idx,
                          std::size_t prefix_len,
                          std::size_t true_F0,
                          double est_N) {
    out << stream_id << ","
        << step_idx << ","
        << prefix_len << ","
        << true_F0 << ","
        << std::fixed << std::setprecision(6) << est_N
        << "\n";
}

static void usage() {
    std::cout
        << "Usage: hll [--streams N] [--size S] [--step PERCENT] [--dup PROB]\n"
        << "           [--B BITS] [--seed SEED] [--out results.csv]\n"
        << "\n"
        << "Defaults: streams=30, size=200000, step=10, dup=0.20, B=12, seed=42\n";
}

int main(int argc, char** argv) {
    int num_streams = 30;
    std::size_t stream_size = 200000;
    int step_percent = 10;
    double dup_prob = 0.20;
    int B = 12;
    std::uint64_t seed = 42;
    std::string out_path = "results.csv";

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto need = [&](const std::string& opt) {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for " << opt << "\n";
                std::exit(2);
            }
        };
        if (a == "--help" || a == "-h") { usage(); return 0; }
        if (a == "--streams") { need(a); num_streams = std::stoi(argv[++i]); continue; }
        if (a == "--size")    { need(a); stream_size = static_cast<std::size_t>(std::stoll(argv[++i])); continue; }
        if (a == "--step")    { need(a); step_percent = std::stoi(argv[++i]); continue; }
        if (a == "--dup")     { need(a); dup_prob = std::stod(argv[++i]); continue; }
        if (a == "--B")       { need(a); B = std::stoi(argv[++i]); continue; }
        if (a == "--seed")    { need(a); seed = static_cast<std::uint64_t>(std::stoull(argv[++i])); continue; }
        if (a == "--out")     { need(a); out_path = argv[++i]; continue; }

        std::cerr << "Unknown option: " << a << "\n";
        usage();
        return 2;
    }

    if (B < 4 || B > 20) {
        std::cerr << "B should be in [4,20]\n";
        return 2;
    }

    RandomStreamGen::Config cfg;
    cfg.stream_size = stream_size;
    cfg.max_len = 30;
    cfg.duplicate_prob = dup_prob;

    std::ofstream out(out_path);
    if (!out) {
        std::cerr << "Cannot open output file: " << out_path << "\n";
        return 2;
    }
    out << "stream_id,step_idx,prefix_len,true_F0,estimate_N\n";

    for (int s = 0; s < num_streams; ++s) {
        cfg.seed = seed + static_cast<std::uint64_t>(s) * 10007ULL;
        RandomStreamGen gen(cfg);

        auto stream = gen.generate_stream();
        auto prefixes = gen.prefix_sizes_by_step_percent(step_percent);

        HyperLogLog hll(B, seed + static_cast<std::uint64_t>(s) * 99991ULL);

        std::size_t prev = 0;
        int step_idx = 0;
        for (std::size_t pref : prefixes) {
            for (std::size_t i = prev; i < pref; ++i) {
                hll.add(stream[i]);
            }
            prev = pref;

            std::size_t trueF0 = exact_distinct_prefix(stream, pref);
            double estN = hll.estimate();
            write_csv_row(out, s, step_idx, pref, trueF0, estN);
            step_idx++;
        }
        std::cerr << "Done stream " << (s + 1) << "/" << num_streams << "\n";
    }

    std::cerr << "Wrote: " << out_path << "\n";
    return 0;
}
