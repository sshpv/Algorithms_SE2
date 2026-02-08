#include "random_stream_gen.h"

static const std::string kAlphabet =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "-";

RandomStreamGen::RandomStreamGen(Config cfg)
    : cfg_(cfg),
      rng_(cfg.seed),
      char_dist_(0, static_cast<int>(kAlphabet.size() - 1)) {}

char RandomStreamGen::random_char() {
    return kAlphabet[static_cast<std::size_t>(char_dist_(rng_))];
}

std::string RandomStreamGen::random_string() {
    std::uniform_int_distribution<int> len_dist(1, static_cast<int>(cfg_.max_len));
    int len = len_dist(rng_);
    std::string s;
    s.reserve(static_cast<std::size_t>(len));
    for (int i = 0; i < len; ++i) s.push_back(random_char());
    return s;
}

std::vector<std::string> RandomStreamGen::generate_stream() {
    std::vector<std::string> stream;
    stream.reserve(cfg_.stream_size);

    std::bernoulli_distribution dup(cfg_.duplicate_prob);

    for (std::size_t i = 0; i < cfg_.stream_size; ++i) {
        if (i > 0 && dup(rng_)) {
            std::uniform_int_distribution<std::size_t> pick(0, i - 1);
            stream.push_back(stream[pick(rng_)]);
        } else {
            stream.push_back(random_string());
        }
    }
    return stream;
}

std::vector<std::size_t> RandomStreamGen::prefix_sizes_by_step_percent(int step_percent) const {
    if (step_percent <= 0) step_percent = 10;
    if (step_percent > 100) step_percent = 100;
    std::vector<std::size_t> res;
    for (int p = step_percent; p <= 100; p += step_percent) {
        std::size_t sz = static_cast<std::size_t>((static_cast<long double>(cfg_.stream_size) * p) / 100.0L);
        if (sz == 0) sz = 1;
        if (sz > cfg_.stream_size) sz = cfg_.stream_size;
        if (res.empty() || res.back() != sz) res.push_back(sz);
    }
    if (res.empty() || res.back() != cfg_.stream_size) res.push_back(cfg_.stream_size);
    return res;
}
