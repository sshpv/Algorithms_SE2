#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include "hash_func_gen.h"

// hyperLogLog на 32 битном хэше
class HyperLogLog {
public:
    explicit HyperLogLog(int B, std::uint64_t hash_seed = 12345);

    void add(const std::string& x);
    double estimate() const;

    int B() const { return B_; }
    std::size_t m() const { return m_; }
    const std::vector<std::uint8_t>& registers() const { return regs_; }

private:
    int B_;
    std::size_t m_;
    HashFuncGen hash_;
    std::vector<std::uint8_t> regs_;

    static double alpha(std::size_t m);
    static int rho(std::uint32_t w, int max_bits);
};
