#include "hash_func_gen.h"

HashFuncGen::HashFuncGen(std::uint64_t seed) : seed_(seed) {}

std::uint64_t HashFuncGen::splitmix64(std::uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

std::uint64_t HashFuncGen::fnv1a64(const std::string& s) const {
    const std::uint64_t FNV_OFFSET = 1469598103934665603ULL;
    const std::uint64_t FNV_PRIME  = 1099511628211ULL;
    std::uint64_t h = FNV_OFFSET ^ seed_;
    for (unsigned char c : s) {
        h ^= static_cast<std::uint64_t>(c);
        h *= FNV_PRIME;
    }
    return h;
}

std::uint32_t HashFuncGen::operator()(const std::string& s) const {
    std::uint64_t h64 = fnv1a64(s);
    std::uint64_t mixed = splitmix64(h64);
    return static_cast<std::uint32_t>(mixed & 0xFFFFFFFFULL);
}
