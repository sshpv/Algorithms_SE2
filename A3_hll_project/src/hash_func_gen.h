#pragma once
#include <cstdint>
#include <string>

class HashFuncGen {
public:
    explicit HashFuncGen(std::uint64_t seed = 0x9e3779b97f4a7c15ULL);

    std::uint32_t operator()(const std::string& s) const;

private:
    std::uint64_t seed_;

    static std::uint64_t splitmix64(std::uint64_t x);
    std::uint64_t fnv1a64(const std::string& s) const;
};
