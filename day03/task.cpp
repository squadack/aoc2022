#include "../base.cpp"

int charValue(char c) {
    if (c >= 'a' && c <= 'z')
        c = c - 'a' + 1;
    else if (c >= 'A' && c <= 'Z')
        c = c - 'A' + 27;
    else
        c = 0;
    return c;
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    int sum = 0;
    for (const auto& line : data) {
        std::set<char> s(line.begin(), line.begin() + line.length() / 2);
        s = intersect(s, std::set<char>(line.begin() + line.length() / 2, line.end()));
        sum += charValue(*s.begin());
    }
    part_1(sum);

    sum = 0;
    for (unsigned i = 0; i < data.size(); i += 3) {
        std::set<char> s(data[i].begin(), data[i].end());
        s = intersect(s, std::set<char>(data[i + 1].begin(), data[i + 1].end()));
        s = intersect(s, std::set<char>(data[i + 2].begin(), data[i + 2].end()));
        sum += charValue(*s.begin());
    }
    part_2(sum);
}
