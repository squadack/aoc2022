#include "../base.cpp"

int main() {
    std::ifstream ifs{"input.txt"};
    std::string instr;

    int X = 1;
    int cycle = 1;
    std::vector<int> hist(256);
    while (ifs >> instr) {
        hist[cycle++] = X;
        if (instr == "addx") {
            int val;
            ifs >> val;
            hist[cycle++] = X;
            X += val;
        }
    }

    int sum = 0;
    for (int i : {20, 60, 100, 140, 180, 220}) {
        sum += hist[i] * i;
    }
    part_1(sum);

    part_2("");
    for (int row = 0; row < 6; row += 1) {
        for (int col = 1; col <= 40; col += 1) {
            if (std::abs(hist[row * 40 + col] - col + 1) <= 1)
                std::cout << '#';
            else
                std::cout << ' ';
        }
        std::cout << '\n';
    }
}
