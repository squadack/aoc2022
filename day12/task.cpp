#include "../base.cpp"

const int MOVES[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int bfs(const std::vector<std::vector<int16_t>>& field, unsigned sx, unsigned sy, unsigned ex, unsigned ey, bool pt2 = false) {
    std::vector<std::vector<int16_t>> cost(field.size(), std::vector<int16_t>(field.front().size(), -1));
    std::queue<std::pair<unsigned, unsigned>> q;
    if (pt2) {
        for (unsigned y = 0; y < field.size(); y += 1) {
            for (unsigned x = 0; x < field[y].size(); x += 1) {
                if (field[y][x] == 0) {
                    cost[y][x] = 0;
                    q.push(std::make_pair(x, y));
                }
            }
        }
    }
    else {
        cost[sy][sx] = 0;
        q.push(std::make_pair(sx, sy));
    }
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int m = 0; m < 4; m += 1) {
            const unsigned nx = x + MOVES[m][0];
            const unsigned ny = y + MOVES[m][1];
            if (ny >= field.size() || nx >= field[ny].size())
                continue;
            if (field[ny][nx] - field[y][x] > 1)
                continue;
            if (cost[ny][nx] == -1) {
                cost[ny][nx] = cost[y][x] + 1;
                q.push(std::make_pair(nx, ny));
            }
        }
    }
    return cost[ey][ex];
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto lines = getLines(ifs);
    std::vector<std::vector<int16_t>> field;
    unsigned sX, sY, eX, eY;
    for (const std::string& line : lines) {
        std::vector<int16_t> row;
        for (char c : line) {
            if (c == 'S') {
                sX = row.size();
                sY = field.size();
                c = 'a';
            }
            else if (c == 'E') {
                eX = row.size();
                eY = field.size();
                c = 'z';
            }
            row.push_back(c - 'a');
        }
        field.emplace_back(std::move(row));
    }

    part_1(bfs(field, sX, sY, eX, eY));
    part_2(bfs(field, sX, sY, eX, eY, true));
}
