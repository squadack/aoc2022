#include "../base.cpp"

bool isUnique(std::string::const_iterator st, std::string::const_iterator ed)
{
    for (uint32_t test = 0; st != ed; st++) {
        int c = *st - 'a';
        if (test & (1 << c))
            return false;
        test |= 1 << c;
    }
    return true;
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::string s;
    ifs >> s;
    uint32_t i;
    for (i = 4; i <= s.size(); i += 1) {
        if (isUnique(s.begin() + i - 4, s.begin() + i))
            break;
    }
    part_1(i);
    for (i += 14 - 4; i <= s.size(); i += 1) {
        if (isUnique(s.begin() + i - 14, s.begin() + i))
            break;
    }
    part_2(i);
}
