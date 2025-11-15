#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct Circle {
    double cx;
    double cy;
    double r;
};

struct Box {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

bool insideCircle(const Circle& c, double x, double y) {
    const double dx = x - c.cx;
    const double dy = y - c.cy;
    return dx * dx + dy * dy <= c.r * c.r;
}

bool insideAll(const std::vector<Circle>& circles, double x, double y) {
    for (const auto& c : circles) {
        if (!insideCircle(c, x, y)) {
            return false;
        }
    }
    return true;
}

double estimateArea(const Box& box,
                    const std::vector<Circle>& circles,
                    int samples,
                    std::mt19937_64& rng)
{
    std::uniform_real_distribution<double> distX(box.xmin, box.xmax);
    std::uniform_real_distribution<double> distY(box.ymin, box.ymax);

    int hits = 0;
    for (int i = 0; i < samples; ++i) {
        double x = distX(rng);
        double y = distY(rng);
        if (insideAll(circles, x, y)) {
            ++hits;
        }
    }

    const double boxArea = (box.xmax - box.xmin) * (box.ymax - box.ymin);
    return boxArea * static_cast<double>(hits) / static_cast<double>(samples);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const double root5 = std::sqrt(5.0);

    Circle c1{1.0, 1.0, 1.0};
    Circle c2{1.5, 2.0, root5 / 2.0};
    Circle c3{2.0, 1.5, root5 / 2.0};

    std::vector<Circle> circles = {c1, c2, c3};

    Box wide{};
    wide.xmin = std::min({c1.cx - c1.r, c2.cx - c2.r, c3.cx - c3.r});
    wide.xmax = std::max({c1.cx + c1.r, c2.cx + c2.r, c3.cx + c3.r});
    wide.ymin = std::min({c1.cy - c1.r, c2.cy - c2.r, c3.cy - c3.r});
    wide.ymax = std::max({c1.cy + c1.r, c2.cy + c2.r, c3.cy + c3.r});

    Box narrow{};
    narrow.xmin = std::max({c1.cx - c1.r, c2.cx - c2.r, c3.cx - c3.r});
    narrow.xmax = std::min({c1.cx + c1.r, c2.cx + c2.r, c3.cx + c3.r});
    narrow.ymin = std::max({c1.cy - c1.r, c2.cy - c2.r, c3.cy - c3.r});
    narrow.ymax = std::min({c1.cy + c1.r, c2.cy + c2.r, c3.cy + c3.r});

    if (narrow.xmin >= narrow.xmax || narrow.ymin >= narrow.ymax) {
        std::cerr << "Error: narrow box is empty.\n";
        return 1;
    }

    std::mt19937_64 rng(123456789);

    std::cout << std::fixed << std::setprecision(8);

    for (int N = 100; N <= 100000; N += 500) {
        double S_wide   = estimateArea(wide,   circles, N, rng);
        double S_narrow = estimateArea(narrow, circles, N, rng);

        std::cout << N << ' '
                  << S_wide << ' '
                  << S_narrow << '\n';
    }

    return 0;
}
