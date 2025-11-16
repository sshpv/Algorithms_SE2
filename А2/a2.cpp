#include <iostream>
#include <fstream>
#include <vector>
#include "array_generator.hpp"
#include "sort_tester.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ArrayGenerator generator(100000, 0, 6000);
    SortTester tester;

    const int minN = 500;
    const int maxN = 100000;
    const int step = 100;

    const int trialsSmall = 5;   // для маленьких массивов
    const int trialsMedium = 3;  // для средних
    const int trialsLarge = 1;   // для больших

    std::vector<int> thresholds;
    thresholds.push_back(5);
    thresholds.push_back(10);
    thresholds.push_back(15);
    thresholds.push_back(20);
    thresholds.push_back(30);
    thresholds.push_back(50);

    std::ofstream mergeRandom("merge_random.csv");
    std::ofstream mergeReversed("merge_reversed.csv");
    std::ofstream mergeNearly("merge_nearly.csv");

    mergeRandom  << "n,time_us\n";
    mergeReversed<< "n,time_us\n";
    mergeNearly  << "n,time_us\n";

    std::ofstream hybridRandom("hybrid_random.csv");
    std::ofstream hybridReversed("hybrid_reversed.csv");
    std::ofstream hybridNearly("hybrid_nearly.csv");

    auto write_header = [&](std::ofstream &out) {
        out << "n";
        for (int th : thresholds) {
            out << ",t" << th;
        }
        out << "\n";
    };

    write_header(hybridRandom);
    write_header(hybridReversed);
    write_header(hybridNearly);

    for (int n = minN; n <= maxN; n += step) {
        int trials;
        if (n < 5000) {
            trials = trialsSmall;
        } else if (n < 30000) {
            trials = trialsMedium;
        } else {
            trials = trialsLarge;
        }

        std::vector<int> baseRandom   = generator.randomArray(n);
        std::vector<int> baseReversed = generator.reversedArray(n);
        std::vector<int> baseNearly   = generator.nearlySortedArray(n, n / 20);

        long long tMergeRandom   = tester.measureMergeSort(baseRandom,   trials);
        long long tMergeReversed = tester.measureMergeSort(baseReversed, trials);
        long long tMergeNearly   = tester.measureMergeSort(baseNearly,   trials);

        mergeRandom   << n << "," << tMergeRandom   << "\n";
        mergeReversed << n << "," << tMergeReversed << "\n";
        mergeNearly   << n << "," << tMergeNearly   << "\n";

        hybridRandom << n;
        hybridReversed << n;
        hybridNearly << n;

        for (int th : thresholds) {
            long long tHRand  = tester.measureHybridSort(baseRandom,   th, trials);
            long long tHRev   = tester.measureHybridSort(baseReversed, th, trials);
            long long tHNear  = tester.measureHybridSort(baseNearly,   th, trials);

            hybridRandom   << "," << tHRand;
            hybridReversed << "," << tHRev;
            hybridNearly   << "," << tHNear;
        }

        hybridRandom   << "\n";
        hybridReversed << "\n";
        hybridNearly   << "\n";

        std::cerr << "Done n = " << n << " (trials = " << trials << ")\n";
    }

    return 0;
}
