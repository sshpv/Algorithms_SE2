#include <iostream>
#include <vector>

using namespace std;

const int PAR = 15;

void insertion_sort(vector<long long> &a, int l, int r) {
    for (int i = l + 1; i < r; ++i) {
        long long key = a[i];
        int j = i - 1;
        while (j >= l && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

void merge_sort(vector<long long> &a, vector<long long> &tmp, int l, int r) {
    int len = r - l;
    if (len <= PAR) {
        insertion_sort(a, l, r);
        return;
    }

    int m = l + (r - l) / 2;
    merge_sort(a, tmp, l, m);
    merge_sort(a, tmp, m, r);

    int i = l;
    int j = m;
    int k = l;

    while (i < m && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i < m) {
        tmp[k++] = a[i++];
    }
    while (j < r) {
        tmp[k++] = a[j++];
    }
    for (int t = l; t < r; ++t) {
        a[t] = tmp[t];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) {
        return 0;
    }

    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n > 1) {
        vector<long long> tmp(n);
        merge_sort(a, tmp, 0, n);
    }

    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';

    return 0;
}
