#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Circle {
    double x;
    double y;
    double r2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Circle c1, c2, c3;
    double r;

    if (!(cin >> c1.x >> c1.y >> r)) return 0;
    c1.r2 = r * r;
    if (!(cin >> c2.x >> c2.y >> r)) return 0;
    c2.r2 = r * r;
    if (!(cin >> c3.x >> c3.y >> r)) return 0;
    c3.r2 = r * r;

    double xmin = max({c1.x - sqrt(c1.r2), c2.x - sqrt(c2.r2), c3.x - sqrt(c3.r2)});
    double xmax = min({c1.x + sqrt(c1.r2), c2.x + sqrt(c2.r2), c3.x + sqrt(c3.r2)});
    double ymin = max({c1.y - sqrt(c1.r2), c2.y - sqrt(c2.r2), c3.y - sqrt(c3.r2)});
    double ymax = min({c1.y + sqrt(c1.r2), c2.y + sqrt(c2.r2), c3.y + sqrt(c3.r2)});

    if (xmin >= xmax || ymin >= ymax) {
        cout << fixed << setprecision(20) << 0.0 << '\n';
        return 0;
    }

    double width  = xmax - xmin;
    double height = ymax - ymin;
    double rectArea = width * height;

    const int SAMPLES = 2000000;

    mt19937_64 rng(123456789);
    uniform_real_distribution<double> dist01(0.0, 1.0);

    auto inside_all = [&](double x, double y) -> bool {
        double dx1 = x - c1.x, dy1 = y - c1.y;
        if (dx1 * dx1 + dy1 * dy1 > c1.r2) return false;

        double dx2 = x - c2.x, dy2 = y - c2.y;
        if (dx2 * dx2 + dy2 * dy2 > c2.r2) return false;

        double dx3 = x - c3.x, dy3 = y - c3.y;
        if (dx3 * dx3 + dy3 * dy3 > c3.r2) return false;

        return true;
    };

    long long hits = 0;

    for (int i = 0; i < SAMPLES; ++i) {
        double rx = dist01(rng);
        double ry = dist01(rng);
        double x = xmin + rx * width;
        double y = ymin + ry * height;

        if (inside_all(x, y)) {
            ++hits;
        }
    }

    double p = static_cast<double>(hits) / static_cast<double>(SAMPLES);
    double area_est = rectArea * p;

    cout << fixed << setprecision(20) << area_est << '\n';
    return 0;
}
