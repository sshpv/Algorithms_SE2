#ifndef ARRAY_GENERATOR_HPP
#define ARRAY_GENERATOR_HPP

#include <vector>
#include <random>
#include <algorithm>

class ArrayGenerator {
public:
    ArrayGenerator(int maxLength = 100000, int minVal = 0, int maxVal = 6000)
            : maxLen(maxLength), minValue(minVal), maxValue(maxVal),
              rng(std::random_device{}()), dist(minVal, maxVal)
    {
        base.reserve(maxLen);
        for (int i = 0; i < maxLen; ++i) {
            base.push_back(dist(rng));
        }
    }

    // Случайный массив размера n
    std::vector<int> randomArray(int n) const {
        if (n > maxLen) n = maxLen;
        return std::vector<int>(base.begin(), base.begin() + n);
    }

    // Массив размера n по невозрастанию
    std::vector<int> reversedArray(int n) const {
        if (n > maxLen) n = maxLen;
        std::vector<int> v(base.begin(), base.begin() + n);
        std::sort(v.begin(), v.end());
        std::reverse(v.begin(), v.end());
        return v;
    }

    // Почти отсортированный
    std::vector<int> nearlySortedArray(int n, int swaps) {
        if (n > maxLen) n = maxLen;
        std::vector<int> v(base.begin(), base.begin() + n);
        std::sort(v.begin(), v.end());

        if (n == 0) return v;
        std::uniform_int_distribution<int> posDist(0, n - 1);
        for (int i = 0; i < swaps; ++i) {
            int i1 = posDist(rng);
            int i2 = posDist(rng);
            std::swap(v[i1], v[i2]);
        }
        return v;
    }

private:
    int maxLen;
    int minValue;
    int maxValue;
    std::vector<int> base;

    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<int> dist;
};

#endif // ARRAY_GENERATOR_HPP
