#pragma once
#include <string>
#include <vector>
#include <random>
#include <cstdint>

class RandomStreamGen {
public:
    struct Config {
        std::size_t stream_size = 200000;
        std::size_t max_len = 30;
        double duplicate_prob = 0.20;      // вероятность повторить один из прошлых элементов
        std::uint64_t seed = 42;
    };

    explicit RandomStreamGen(Config cfg);

    std::vector<std::string> generate_stream();

    std::vector<std::size_t> prefix_sizes_by_step_percent(int step_percent) const;

private:
    Config cfg_;
    std::mt19937_64 rng_;
    std::uniform_int_distribution<int> char_dist_;

    char random_char();
    std::string random_string();
};
