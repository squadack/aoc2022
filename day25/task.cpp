#include "../base.cpp"

const std::unordered_map<char, int64_t> charValue{{'0', 0}, {'1', 1}, {'2', 2}, {'-', -1}, {'=', -2}};
int64_t SNAFU2dec(const std::string& snafu) {
    int64_t result = 0;
    int64_t power = 1;
    for (auto it = snafu.rbegin(); it != snafu.rend(); it++) {
        result += power * charValue.at(*it);
        power *= 5;
    }
    return result;
}

std::string dec2SNAFU(int64_t dec) {
    const char digit[5] = {'0', '1', '2', '=', '-'};
    const int64_t carry[5] = {0, 0, 0, 2, 1};
    std::string result;
    while (dec != 0) {
        int64_t mod = dec % 5;
        result += digit[mod];
        dec += carry[mod];
        dec /= 5;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    int64_t sum = 0;
    for (const auto& line : data) {
        sum += SNAFU2dec(line);
    }
    part_1(dec2SNAFU(sum));
    part_2("GG EZ ;)");
}
