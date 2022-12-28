#include "../base.cpp"

struct Position {
    int x;
    int y;

    Position() = default;
    Position(int x, int y) : x{x}, y{y} {}

    bool operator<(const Position& p) const {
        return x < p.x || x == p.x && y < p.y;
    }
};

Position getUpperLeft(const std::set<Position>& pos) {
    Position result;
    result.x = std::min_element(pos.begin(), pos.end(), [](const Position& a, const Position& b){ return a.x < b.x;})->x;
    result.y = std::min_element(pos.begin(), pos.end(), [](const Position& a, const Position& b){ return a.y < b.y;})->y;
    return result;
}

Position getLowerRight(const std::set<Position>& pos) {
    Position result;
    result.x = std::max_element(pos.begin(), pos.end(), [](const Position& a, const Position& b){ return a.x < b.x;})->x;
    result.y = std::max_element(pos.begin(), pos.end(), [](const Position& a, const Position& b){ return a.y < b.y;})->y;
    return result;
}

void print(const std::set<Position>& elves) {
    Position ul = getUpperLeft(elves);
    Position lr = getLowerRight(elves);
    int width = lr.x - ul.x + 1;
    int height = lr.y - ul.y + 1;
    int dx = ul.x;
    int dy = ul.y;
    std::vector<std::string> buf(height, std::string(width, '.'));

    for (const Position& elf : elves) {
        buf[elf.y - dy][elf.x - dx] = '#';
    }

    for (const auto& s : buf) {
        std::cout << s << '\n';
    }
}

const int neigh[8][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};


// NSWE
const int dir[4][3][2] = {
    {{0, -1}, {1, -1}, {-1, -1}},
    {{0, 1}, {1, 1}, {-1, 1}},
    {{-1, 0}, {-1, -1}, {-1, 1}},
    {{1, 0}, {1, -1}, {1, 1}}
};

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    std::set<Position> elves;
    for (int y = 0; y < data.size(); y += 1) {
        for (int x = 0; x < data[y].size(); x += 1) {
            if (data[y][x] == '#')
                elves.emplace(x, y);
        }
    }

    for (int round = 0;; round += 1) {
        if (round == 10) {
            Position ul = getUpperLeft(elves);
            Position lr = getLowerRight(elves);
            int width = lr.x - ul.x + 1;
            int height = lr.y - ul.y + 1;
            int result = width * height - elves.size();
            part_1(result);
        }
        std::map<Position, int> proposition;
        std::map<Position, Position> target;
        int stationary = 0;

        for (const Position& elf : elves) {
            bool none = true;
            for (int i = 0; i < 8; i += 1) {
                const int nx = elf.x + neigh[i][0];
                const int ny = elf.y + neigh[i][1];
                Position n{nx, ny};
                if (elves.find(n) != elves.end()) {
                    none = false;
                    break;
                }
            }
            if (none) {
                proposition[elf] += 1;
                target[elf] = elf;
                stationary += 1;
                continue;
            }
            bool has_target = false;
            for (int i = 0; i < 4; i += 1) {
                int idx = (i + round) % 4;
                bool empty = true;
                for (int k = 0; k < 3; k += 1) {
                    Position n{elf.x + dir[idx][k][0], elf.y + dir[idx][k][1]};
                    if (elves.find(n) != elves.end()) {
                        empty = false;
                        break;
                    }
                }
                if (empty) {
                    Position p{elf.x + dir[idx][0][0], elf.y + dir[idx][0][1]};
                    proposition[p] += 1;
                    target[elf] = p;
                    has_target = true;
                    break;
                }
            }
            if (!has_target) {
                proposition[elf] += 1;
                target[elf] = elf;
                stationary += 1;
            }
        }

        if (stationary == elves.size()) {
            part_2(round + 1);
            break;
        }

        std::set<Position> new_elves;

        for (const Position& elf : elves) {
            if (proposition.at(target.at(elf)) == 1)
                new_elves.emplace(std::move(target.at(elf)));
            else
                new_elves.emplace(elf);
        }

        elves = std::move(new_elves);
    }
}
