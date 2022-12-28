#include "../base.cpp"

const int DIR[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;

struct Blizzard {
    int x;
    int y;
    int dir;

    Blizzard(int x, int y, int dir) : x{x}, y{y}, dir{dir} {}

    bool isHorizontal() const {
        return dir % 2 == 1;
    }

    void move(int width, int height) {
        if (isHorizontal()) {
            x += DIR[dir][0] + width;
            x %= width;
        }
        else {
            y += DIR[dir][1] + height;
            y %= height;
        }
    }
};

struct Valley {
    std::vector<Blizzard> blizz;
    int width;
    int height;
    std::vector<std::vector<std::vector<bool>>> busy;

    Valley(int w, int h)
    : width{w}, height{h}
    {
        busy.emplace_back(std::vector<std::vector<bool>>(height, std::vector<bool>(width, false)));
    }

    void addBlizz(int x, int y, int dir) {
        Blizzard b{x, y, dir};
        blizz.emplace_back(std::move(b));
        busy[0][y][x] = true;
    }

    bool isFieldFree(int x, int y, int time) {
        if (time == busy.size())
            calculateNextMinute();
        if (!inBounds(x, y))
            return true;
        return !busy.at(time)[y][x];
    }

    void calculateNextMinute() {
        busy.emplace_back(std::vector<std::vector<bool>>(height, std::vector<bool>(width, false)));
        for (Blizzard& b : blizz) {
            b.move(width, height);
            busy.back()[b.y][b.x] = true;
        }
    }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

int bfs(Valley& v, int sx, int sy, int ex, int ey, int t = 0) {
    const int MOVES[5][3] = {
        {1, 0, 1},
        {-1, 0, 1},
        {0, 1, 1},
        {0, -1, 1},
        {0, 0, 1}
    };

    int mod = std::lcm(v.width, v.height);

    std::set<std::tuple<int, int, int>> visited;
    std::queue<std::tuple<int, int, int>> q;
    q.emplace(sx, sy, t);
    visited.emplace(sx, sy, t);

    while (!q.empty()) {
        auto [x, y, t] = q.front();
        q.pop();
        for (int i = 0; i < 5; i += 1) {
            const int nx = x + MOVES[i][0];
            const int ny = y + MOVES[i][1];
            const int nt = t + MOVES[i][2];
            if (nx == ex && ny == ey)
                return nt;
            if (v.inBounds(nx, ny) || nx == sx && ny == sy) {
                auto tp = std::make_tuple(nx, ny, nt % mod);
                if (visited.find(tp) != visited.end())
                    continue;
                if (v.isFieldFree(nx, ny, nt % mod)) {
                    visited.emplace(tp);
                    q.emplace(nx, ny, nt);
                }
            }
        }
    }
    return -1;
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);

    int height = data.size() - 2;
    int width = data[0].size() - 2;
    Valley v{width, height};

    int start_y = -1;
    int start_x;
    for (int x = 0; x < data[0].size(); x += 1) {
        if (data[0][x] == '.') {
            start_x = x - 1;
            break;
        }
    }

    for (int y = 1; y + 1 < data.size(); y += 1) {
        for (int x = 0; x < data[y].size(); x += 1) {
            const int yy = y - 1;
            const int xx = x - 1;
            if (data[y][x] == '>') {
                v.addBlizz(xx, yy, RIGHT);
            }
            else if (data[y][x] == '<') {
                v.addBlizz(xx, yy, LEFT);
            }
            else if (data[y][x] == '^') {
                v.addBlizz(xx, yy, UP);
            }
            else if (data[y][x] == 'v') {
                v.addBlizz(xx, yy, DOWN);
            }
        }
    }

    int end_y = v.height;
    int end_x;
    for (int x = 0; data.back().size(); x += 1) {
        if (data.back()[x] == '.') {
            end_x = x - 1;
            break;
        }
    }

    int result = bfs(v, start_x, start_y, end_x, end_y);
    part_1(result);
    result = bfs(v, end_x, end_y, start_x, start_y, result);
    result = bfs(v, start_x, start_y, end_x, end_y, result);
    part_2(result);
}
