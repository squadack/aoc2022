#include "../base.cpp"

struct Point {
    int x;
    int y;

    Point(int x = 0, int y = 0) : x{x}, y{y} {}

    void move(char dir) {
        switch (dir) {
            case 'U':
                y -= 1;
                break;
            case 'D':
                y += 1;
                break;
            case 'L':
                x -= 1;
                break;
            case 'R':
                x += 1;
                break;
        }
    }

    bool operator<(const Point& p) const {
        return x < p.x || x == p.x && y < p.y;
    }

    int distance(const Point& p) const {
        return std::max(std::abs(x - p.x), std::abs(y - p.y));
    }
};

struct Rope {
    Rope(int len) : length{len} {
        knots.resize(len);
        visited.insert(knots.back());
    }

    void move(char d, int n) {
        for (int i = 0; i < n; i += 1) {
            move(d);
        }
    }

    void move(char d) {
        knots[0].move(d);
        for (uint32_t i = 1; i < knots.size(); i += 1) {
            if (knots[i].distance(knots[i - 1]) <= 1)
                break;

            const int dx = knots[i - 1].x - knots[i].x;
            const int dy = knots[i - 1].y - knots[i].y;
            if (std::abs(dx) == 2) {
                knots[i].x += dx / std::abs(dx);
            }
            if (std::abs(dy) == 2) {
                knots[i].y += dy / std::abs(dy);
            }

            if (std::abs(dx) + std::abs(dy) <= 2)
                continue;

            if (std::abs(dx) == 1)
                knots[i].x += dx;
            else if (std::abs(dy) == 1)
                knots[i].y += dy;
        }
        visited.insert(knots.back());
    }

    int length;
    std::vector<Point> knots;
    std::set<Point> visited;
};

int main() {
    std::ifstream ifs{"input.txt"};
    char d;
    int val;
    Rope p1{2};
    Rope p2{10};

    while (ifs >> d >> val) {
        p1.move(d, val);
        p2.move(d, val);
    }
    part_1(p1.visited.size());
    part_2(p2.visited.size());
}
