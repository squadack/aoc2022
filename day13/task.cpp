#include "../base.cpp"

struct Value {
    bool isList = false;
    int x;
    std::vector<Value> list;

    bool operator<(const Value& v) const {
        if (!isList && !v.isList) {
            return x < v.x;
        }
        else if (isList && v.isList) {
            auto it = list.begin();
            auto vit = v.list.begin();
            while (it != list.end() && vit != v.list.end()) {
                if (*it < *vit)
                    return true;
                if (*vit < *it)
                    return false;
                it++;
                vit++;
            }
            if (it == list.end() && vit != v.list.end())
                return true;
            return false;
        }
        else if (!isList) {
            Value tmp;
            tmp.isList = true;
            tmp.list.push_back(*this);
            return tmp < v;
        }
        else { // !v.isList
            Value tmp;
            tmp.isList = true;
            tmp.list.push_back(v);
            return *this < tmp;
        }
    }

    bool operator==(const Value& v) const {
        return !(*this < v) && !(v < *this);
    }
};

std::ostream& operator<<(std::ostream& os, const Value& v) {
    if (v.isList)
        os << v.list;
    else
        os << v.x;
    return os;
}

Value parse(std::istringstream& iss) {
    Value result;
    if (iss.peek() == '[') {
        iss.get();
        result.isList = true;
        while (iss.peek() != ']') {
            result.list.emplace_back(parse(iss));
            if (iss.peek() == ',')
                iss.get(); //remove comma
        }
        iss.get(); //remove ']'
    }
    else
        iss >> result.x;
    return result;
}

Value parse(const std::string& line) {
    std::istringstream iss{line};
    return parse(iss);
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    std::vector<Value> packets;
    for (const auto& line : data) {
        if (line.empty())
            continue;
        packets.emplace_back(parse(line));
    }

    unsigned sum = 0;
    for (unsigned i = 1; i < packets.size(); i += 2) {
        if (packets[i - 1] < packets[i])
            sum += i / 2 + 1;
    }
    part_1(sum);

    Value divider1 = parse("[[2]]");
    Value divider2 = parse("[[6]]");
    packets.push_back(divider1);
    packets.push_back(divider2);

    const int a = std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider1)) + 1;
    const int b = std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider2)) + 1;

    part_2(a * b);
}
