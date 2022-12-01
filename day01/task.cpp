#include "../base.cpp"

int main() {
    std::ifstream ifs{"input.txt"};
    auto lines = getLines(ifs);
    std::vector<int> calories;
    int sum = 0;
    for (const auto& line : lines) {
        if (line.empty()) {
            calories.push_back(sum);
            sum = 0;
            continue;
        }
        int x;
        line >> x;
        sum += x;
    }
    if (sum > 0)
        calories.push_back(sum);
    std::sort(calories.rbegin(), calories.rend());
    part_1(calories[0]);
    part_2(std::accumulate(calories.begin(), calories.begin() + 3, 0));
}
