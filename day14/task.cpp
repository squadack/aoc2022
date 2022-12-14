#include "../base.cpp"

int sandfall(std::vector<std::vector<char>> cave) {
    int count = 0;
    while (true) {
        unsigned x = 500;
        unsigned y = 0;
        if (cave[y][x] != '.') {
            return count;
        }
        while (true) {
            if (y + 1 == cave.size()) {
                return count;
            }
            if (cave[y + 1][x] == '.') {
                y += 1;
            }
            else if (cave[y + 1][x - 1] == '.') {
                y += 1;
                x -= 1;
            }
            else if (cave[y + 1][x + 1] == '.') {
                y += 1;
                x += 1;
            }
            else {
                cave[y][x] = 'o';
                count += 1;
                break;
            }
        }
    }
}

void drawLine(std::vector<std::vector<char>>& cave, int x1, int y1, int x2, int y2) {
    int dx;
    int dy;
    if (x1 == x2) {
        dx = 0;
        dy = y1 < y2 ? 1 : -1;
    }
    else {
        dx = x1 < x2 ? 1 : -1;
        dy = 0;
    }
    for (; x1 != x2 || y1 != y2; x1 += dx, y1 += dy) {
        cave[y1][x1] = '#';
    }
    cave[y1][x1] = '#';
}

int drawLine(const std::string& line, std::vector<std::vector<char>>& cave) {
    auto pts = split(line," -> ");
    auto first = split(pts[0], ",");
    int x = convertStr<int>(first[0]);
    int y = convertStr<int>(first[1]);
    int maxy = y;
    for (unsigned i = 1; i < pts.size(); i += 1) {
        auto p = split(pts[i], ",");
        int xp = convertStr<int>(p[0]);
        int yp = convertStr<int>(p[1]);
        drawLine(cave, x, y, xp, yp);
        x = xp;
        y = yp;
        maxy = std::max(maxy, y);
    }
    return maxy;
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::vector<std::vector<char>> cave(1000, std::vector<char>(1000, '.'));
    auto lines = getLines(ifs);
    int maxy = 0;
    for (const auto& line : lines) {
        maxy = std::max(drawLine(line, cave), maxy);
    }
    part_1(sandfall(cave));

    int floor = maxy + 2;
    for (auto& c : cave[floor])
        c = '#';
    part_2(sandfall(cave));
}
