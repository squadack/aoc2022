#include "../base.cpp"

void crane(std::vector<std::vector<char>>& stacks, int c, int f, int t, bool pt2 = false) {
    for (int i = 0; i < c; i += 1) {
        char p = stacks[f].back();
        stacks[f].pop_back();
        stacks[t].push_back(p);
    }
    if (pt2)
        std::reverse(stacks[t].rbegin(), stacks[t].rbegin() + c);
}

std::string ans(const std::vector<std::vector<char>>& stacks) {
    std::string a;
    for (const auto& stack : stacks)
        a.push_back(stack.back());
    return a;
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::vector<std::string> startState;
    std::string s;
    while (std::getline(ifs, s), !s.empty())
        startState.emplace_back(std::move(s));
    std::reverse(startState.begin(), startState.end());

    std::vector<std::vector<char>> stacks, stacks2;
    auto it = startState.begin();
    std::istringstream iss{*(it++)};
    int x;
    while (iss >> x)
        stacks.emplace_back();

    for (; it != startState.end(); it++) {
        char open, letter, close, space;
        std::istringstream iss{*it};
        int i = 0;
        for (int i = 0; iss.get(open).get(letter).get(close); i += 1) {
            iss.get(space);
            if (letter >= 'A' && letter <= 'Z')
                stacks[i].push_back(letter);
        }
    }
    stacks2 = stacks;

    int count, from, to;
    while (ifs >> s >> count >> s >> from >> s >> to) {
        crane(stacks, count, from - 1, to - 1);
        crane(stacks2, count, from - 1, to - 1, true);
    }

    part_1(ans(stacks));
    part_2(ans(stacks2));
}
