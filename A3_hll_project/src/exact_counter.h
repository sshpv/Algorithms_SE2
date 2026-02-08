#pragma once
#include <unordered_set>
#include <string>
#include <vector>

inline std::size_t exact_distinct_prefix(const std::vector<std::string>& stream, std::size_t prefix_len) {
    std::unordered_set<std::string> st;
    st.reserve(prefix_len * 2);
    for (std::size_t i = 0; i < prefix_len; ++i) st.insert(stream[i]);
    return st.size();
}
