#include "../base.cpp"

void prepareH(const std::vector<std::vector<int8_t>>& forest, std::vector<std::vector<int8_t>>& tmp, bool reverse = false) {
    for (uint32_t row = 0; row < forest.size(); row += 1) {
        int8_t max = -1;
        for (uint32_t col = 0; col < forest[row].size(); col += 1) {
            uint32_t c = col;
            if (reverse)
                c = forest[row].size() - c - 1;
            tmp[row][c] = max;
            max = std::max(max, forest[row][c]);
        }
    }
}

void prepareV(const std::vector<std::vector<int8_t>>& forest, std::vector<std::vector<int8_t>>& tmp, bool reverse = false) {
    for (uint32_t col = 0; col < forest.front().size(); col += 1) {
        int8_t max = -1;
        for (uint32_t row = 0; row < forest.size(); row += 1) {
            uint32_t r = row;
            if (reverse)
                r = forest.size() - r - 1;
            tmp[r][col] = max;
            max = std::max(max, forest[r][col]);
        }
    }
}

// i give up
uint32_t scenicScore(const std::vector<std::vector<int8_t>>& forest, uint32_t row, uint32_t col) {
    uint32_t left{col}, right{forest.front().size() - col - 1}, up{row}, down{forest.size() - row - 1};
    for (int i = col - 1; i >= 0; i -= 1) {
        if (forest[row][i] >= forest[row][col]) {
            left = col - i;
            break;
        }
    }
    for (uint32_t i = col + 1; i < forest.front().size(); i += 1) {
        if (forest[row][i] >= forest[row][col]) {
            right = i - col;
            break;
        }
    }
    for (int i = row - 1; i >= 0; i -= 1) {
        if (forest[i][col] >= forest[row][col]) {
            up = row - i;
            break;
        }
    }
    for (uint32_t i = row + 1; i < forest.size(); i += 1) {
        if (forest[i][col] >= forest[row][col]) {
            down = i - row;
            break;
        }
    }
    return left * right * up * down;
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto lines = getLines(ifs);
    std::vector<std::vector<int8_t>> forest(lines.size());
    for (uint32_t row = 0; row < lines.size(); row += 1) {
        for (char c: lines[row])
            forest[row].push_back(c - '0');
    }

    std::vector<std::vector<std::vector<int8_t>>> tmp(4, std::vector<std::vector<int8_t>>(forest.size(), std::vector<int8_t>(forest.front().size(), -1)));

    prepareH(forest, tmp[0]);
    prepareH(forest, tmp[1], true);
    prepareV(forest, tmp[2]);
    prepareV(forest, tmp[3], true);

    uint32_t sum = 0;
    for (uint32_t row = 0; row < forest.size(); row += 1) {
        for (uint32_t col = 0; col < forest[row].size(); col += 1) {
            const auto t = forest[row][col];
            for (const auto& p : tmp)
                if (t > p[row][col]) {
                    sum += 1;
                    break;
                }
        }
    }
    part_1(sum);

    uint32_t best = 0;
    for (uint32_t row = 0; row < forest.size(); row += 1)
        for (uint32_t col = 0; col < forest[row].size(); col += 1)
            best = std::max(best, scenicScore(forest, row, col));
    part_2(best);
}
