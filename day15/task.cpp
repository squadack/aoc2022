#include "../base.cpp"

int distance(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

struct Sensor {
    int x;
    int y;
    int beacon_x;
    int beacon_y;

    Sensor(int x, int y, int sx, int sy) : x{x}, y{y}, beacon_x{sx}, beacon_y{sy} {}

    int range() const {
        return distance(x, y, beacon_x, beacon_y);
    }
};


bool isOutOfRange(const std::vector<Sensor>& sensors, int x, int y) {
    if (x < 0 || x > 4000000 || y < 0 || y > 4000000)
        return false;
    for (const Sensor& s : sensors) {
        if (distance(x, y, s.x, s.y) <= s.range())
            return false;
    }
    return true;
}

int64_t f(const std::vector<Sensor>& sensors) {
    const int STEP[4][2] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    for (const Sensor& s : sensors) {
        int x = s.x;
        int y = s.y - s.range() - 1;
        for (int side = 0; side < 4; side += 1) {
            for (int i = 0; i < s.range() + 1; i += 1, x += STEP[side][0], y += STEP[side][1]) {
                if (isOutOfRange(sensors, x, y)) {
                    int64_t result = x;
                    result *= 4000000;
                    result += y;
                    return result;
                }
            }
        }
    }
    return 0;
}

const int ROW = 2000000;

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    std::vector<Sensor> sensors;

    std::regex re{"Sensor at x=(-?[\\d]+), y=(-?[\\d]+): closest beacon is at x=(-?[\\d]+), y=(-?[\\d]+)"};
    for (const auto& line : data) {
        std::smatch match;
        if (std::regex_search(line, match, re)) {
            sensors.emplace_back(convertStr<int>(match[1]), convertStr<int>(match[2]), convertStr<int>(match[3]), convertStr<int>(match[4]));

            // std::cout << "sensor:\n";
            // const Sensor& s = sensors.back();
            // std::cout << s.x << ' ' << s.y << ' ' << s.beacon_x << ' ' << s.beacon_y << ' ' << s.range() << '\n';
        }
    }

    std::unordered_set<int> impossible;
    std::unordered_set<int> beacons_on_row;
    for (const Sensor& s : sensors) {
        if (s.beacon_y == ROW)
            beacons_on_row.insert(s.beacon_x);
        int b = s.range() - std::abs(s.y - ROW);
        if (b < 0)
            continue;
        for (int i = s.x - b; i <= s.x + b; i += 1) {
            impossible.insert(i);
        }
    }
    part_1(impossible.size() - beacons_on_row.size());

    part_2(f(sensors));
}
