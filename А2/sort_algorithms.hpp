#ifndef SORT_ALGORITHMS_HPP
#define SORT_ALGORITHMS_HPP

#include <vector>

namespace mysort {

    inline void insertionSort(std::vector<int> &a, int l, int r) {
        for (int i = l + 1; i < r; ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= l && a[j] > key) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = key;
        }
    }

    inline void mergeSortRec(std::vector<int> &a,
                             std::vector<int> &tmp,
                             int l, int r) {
        if (r - l <= 1) return;
        int m = l + (r - l) / 2;
        mergeSortRec(a, tmp, l, m);
        mergeSortRec(a, tmp, m, r);

        int i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i] <= a[j]) tmp[k++] = a[i++];
            else              tmp[k++] = a[j++];
        }
        while (i < m) tmp[k++] = a[i++];
        while (j < r) tmp[k++] = a[j++];
        for (int t = l; t < r; ++t) a[t] = tmp[t];
    }

    inline void mergeSort(std::vector<int> &a) {
        if (a.size() <= 1) return;
        std::vector<int> tmp(a.size());
        mergeSortRec(a, tmp, 0, static_cast<int>(a.size()));
    }

    // Гибридный
    inline void hybridMergeSortRec(std::vector<int> &a,
                                   std::vector<int> &tmp,
                                   int l, int r,
                                   int threshold) {
        int len = r - l;
        if (len <= threshold) {
            insertionSort(a, l, r);
            return;
        }
        int m = l + len / 2;
        hybridMergeSortRec(a, tmp, l, m, threshold);
        hybridMergeSortRec(a, tmp, m, r, threshold);

        int i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i] <= a[j]) tmp[k++] = a[i++];
            else              tmp[k++] = a[j++];
        }
        while (i < m) tmp[k++] = a[i++];
        while (j < r) tmp[k++] = a[j++];
        for (int t = l; t < r; ++t) a[t] = tmp[t];
    }

    inline void hybridMergeSort(std::vector<int> &a, int threshold) {
        if (a.size() <= 1) return;
        std::vector<int> tmp(a.size());
        hybridMergeSortRec(a, tmp, 0, static_cast<int>(a.size()), threshold);
    }

}

#endif // SORT_ALGORITHMS_HPP
