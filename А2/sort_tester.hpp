#ifndef SORT_TESTER_HPP
#define SORT_TESTER_HPP

#include <vector>
#include <chrono>
#include "sort_algorithms.hpp"

class SortTester {
public:
    // Время стандартного merge sort, среднее по trials, в микросекундах
    long long measureMergeSort(const std::vector<int> &data, int trials) {
        using namespace std::chrono;
        long long total = 0;
        for (int t = 0; t < trials; ++t) {
            std::vector<int> arr = data;
            auto start = high_resolution_clock::now();
            mysort::mergeSort(arr);
            auto end = high_resolution_clock::now();
            total += duration_cast<microseconds>(end - start).count();
        }
        return total / trials;
    }

    // Время гибридного merge+insertion сортировки с данным threshold
    long long measureHybridSort(const std::vector<int> &data,
                                int threshold,
                                int trials) {
        using namespace std::chrono;
        long long total = 0;
        for (int t = 0; t < trials; ++t) {
            std::vector<int> arr = data;
            auto start = high_resolution_clock::now();
            mysort::hybridMergeSort(arr, threshold);
            auto end = high_resolution_clock::now();
            total += duration_cast<microseconds>(end - start).count();
        }
        return total / trials;
    }
};

#endif // SORT_TESTER_HPP
