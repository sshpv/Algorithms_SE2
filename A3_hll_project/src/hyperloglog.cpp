#include "hyperloglog.h"
#include <algorithm>

HyperLogLog::HyperLogLog(int B, std::uint64_t hash_seed)
    : B_(B),
      m_(static_cast<std::size_t>(1ULL << B)),
      hash_(hash_seed),
      regs_(m_, 0) {}

double HyperLogLog::alpha(std::size_t m) {
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
}

int HyperLogLog::rho(std::uint32_t w, int max_bits) {
    if (w == 0) return max_bits + 1;
#if defined(__GNUG__) || defined(__clang__)
    return __builtin_clz(w) + 1;
#else
    int cnt = 1;
    for (int i = 31; i >= 0; --i) {
        if ((w >> i) & 1U) return cnt;
        cnt++;
    }
    return max_bits + 1;
#endif
}

void HyperLogLog::add(const std::string& x) {
    std::uint32_t h = hash_(x);
    std::uint32_t idx = h >> (32 - B_);
    std::uint32_t w = (h << B_); 
    int max_bits = 32 - B_;
    int r = rho(w, max_bits);
    if (r > max_bits + 1) r = max_bits + 1;
    std::uint8_t val = static_cast<std::uint8_t>(std::min(r, 255));
    if (val > regs_[idx]) regs_[idx] = val;
}

double HyperLogLog::estimate() const {
    double Z = 0.0;
    int V = 0;
    for (std::uint8_t r : regs_) {
        Z += std::ldexp(1.0, -static_cast<int>(r)); 
        if (r == 0) V++;
    }
    double m = static_cast<double>(m_);
    double E = alpha(m_) * m * m / Z;

    if (E <= 2.5 * m && V > 0) {
        E = m * std::log(m / static_cast<double>(V));
    }

    const double TWO32 = 4294967296.0;
    if (E > (TWO32 / 30.0)) {
        double x = 1.0 - (E / TWO32);
        if (x > 0.0) E = -TWO32 * std::log(x);
    }
    return E;
}
