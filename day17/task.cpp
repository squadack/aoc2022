#include "../base.cpp"

const std::vector<std::vector<std::pair<int, int>>> rocks{
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    {{0, 0}, {1, 0}, {1, 1}, {0, 1}}
};

void spawnRock(std::deque<std::vector<char>>& cavern, int top, int& x, int& y) {
    x = 2;
    y = top + 3;
    while (cavern.size() <= y + 3)
        cavern.push_back(std::vector<char>(7, '.'));
}

bool isPositionFree(std::deque<std::vector<char>>& cavern, int x, int y, int rockType) {
    const auto& r = rocks.at(rockType);
    for (const auto& [rx, ry] : r) {
        const int nx = x + rx;
        const int ny = y + ry;
        if (nx < 0 || nx >= 7 || ny < 0 || cavern[ny][nx] == '#')
            return false;
    }
    return true;
}

void applyWind(std::deque<std::vector<char>>& cavern, int rockType, char dir, int& x, int& y) {
    int d = dir == '>' ? 1 : -1;
    if (isPositionFree(cavern, x + d, y, rockType)) {
        x += d;
    }
}

bool fallDown(std::deque<std::vector<char>>& cavern, int rockType, int& top, int& x, int& y) {
    if (isPositionFree(cavern, x, y - 1, rockType)) {
        y -= 1;
        return true;
    }
    else {
        for (auto [rx, ry] : rocks[rockType]) {
            const int nx = x + rx;
            const int ny = y + ry;
            cavern[ny][nx] = '#';
            top = std::max(top, ny + 1);
        }
        return false;
    }
}

void print(const std::deque<std::vector<char>>& cave, int rows = -1) {
    int r = 0;
    for (auto it = cave.rbegin(); it != cave.rend(); it++) {
        std::cout << '|';
        for (char c : *it)
            std::cout << c;
        std::cout << '|' << '\n';
        if (rows != -1 && ++r >= rows)
            break;
    }
}

int64_t trunc(std::deque<std::vector<char>>& cave, int& top) {
    int64_t removed = 0;
    while (cave.size() > 200) {
        cave.pop_front();
        removed += 1;
        top -= 1;
    }
    return removed;
}

struct CacheKey {
    unsigned val;

    CacheKey(unsigned wind, int rock) {
        val = wind;
        val <<= 3;
        val += rock;
    }

    bool operator<(const CacheKey& x) const {
        return val < x.val;
    }
};

struct CacheEntry {
    int64_t h;
    int64_t idx;
    std::deque<std::vector<char>> rows;

    CacheEntry() = default;
    CacheEntry(const std::deque<std::vector<char>>& cavern, int top, int64_t height, int64_t i) {
        idx = i;
        h = height;
        for (int i = 0; i < 10; i += 1) {
            rows.push_front(cavern[top - i]);
        }
    }

    bool isMatch(const std::deque<std::vector<char>>& cavern, int top) const {
        for (int i = 0; i < 10; i += 1) {
            for (int k = 0; k < 7; k += 1) {
                if (cavern[top - i][k] != rows[9 - i][k])
                    return false;
            }
        }
        return true;
    }
};

int main() {
    std::ifstream ifs{"input.txt"};
    std::string wind;
    ifs >> wind;
    std::deque<std::vector<char>> cavern(4, std::vector<char>(7, '.'));

    int top = 0;
    int rockType = 0;

    unsigned wind_idx = 0;

    std::cout << wind.size() << '\n';

    int64_t permatop = 0;

    bool wind_reset = false;
    bool cycle_found = false;
    bool p1_shown = false;

    std::map<CacheKey, CacheEntry> cache;

    const int64_t TOTAL_ROCKS = 1000000000000ll;

    for (int64_t i = 0; i < TOTAL_ROCKS; i += 1) {
        if (i == 2022) {
            part_1(top + permatop);
            p1_shown = true;
        }
        int x;
        int y;

        spawnRock(cavern, top, x, y);
        if (wind_reset && p1_shown && !cycle_found) {
            CacheKey key{wind_idx, rockType};
            if (cache.find(key) == cache.end()) {
                CacheEntry e{cavern, top, permatop + top, i};
                cache[key] = std::move(e);
            }
            else if (cache[key].isMatch(cavern, top)) {
                const int64_t length = i - cache[key].idx;
                cycle_found = true;
                int64_t rocks_left = TOTAL_ROCKS - i;
                int64_t cycle_no = rocks_left / length;
                int64_t h_diff = permatop + top - cache[key].h;
                permatop += h_diff * cycle_no;
                i += cycle_no * length;
            }
        }
        while (true) {
            applyWind(cavern, rockType, wind[wind_idx++], x, y);
            if (wind_idx == wind.size()) {
                wind_reset = true;
                wind_idx = 0;
            }
            if (!fallDown(cavern, rockType, top, x, y)) {
                break;
                if (wind_idx == 0 && rockType == 4) {
                    print(cavern, 20);
                    std::cout << "i == " << i << '\n';
                    exit(0);
                }
            }
        }
        rockType = (rockType + 1) % rocks.size();
        permatop += trunc(cavern, top);
    }
    part_2(top + permatop);
}
