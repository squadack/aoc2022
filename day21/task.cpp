#include "../base.cpp"

struct Monkey {
    std::string name;
    char op;
    std::string left, right;
    int64_t val;

    int64_t getVal(const std::map<std::string, Monkey>& monkeys) const {
        static const std::map<char, std::function<int64_t(int64_t, int64_t)>> f = {
            {'+', [](int64_t a, int64_t b){return a + b;}},
            {'-', [](int64_t a, int64_t b){return a - b;}},
            {'*', [](int64_t a, int64_t b){return a * b;}},
            {'/', [](int64_t a, int64_t b){return a / b;}}};
        if (op == '!')
            return val;
        int64_t v_left = monkeys.at(left).getVal(monkeys);
        int64_t v_right = monkeys.at(right).getVal(monkeys);
        return f.at(op)(v_left, v_right);
    }

    bool contains(const std::string& name, const std::map<std::string, Monkey>& monkeys) const {
        if (this->name == name)
            return true;
        if (op == '!')
            return false;
        return monkeys.at(left).contains(name, monkeys) || monkeys.at(right).contains(name, monkeys);
    }
};

int main() {
    std::ifstream ifs{"input.txt"};
    std::map<std::string, Monkey> monkeys;

    auto lines = getLines(ifs);
    for (const auto& line : lines) {
        std::string name(line, 0, 4);
        std::string rhs(line.begin() + 6, line.end());
        auto words = split(rhs);

        Monkey m;
        m.name = name;
        if (words.size() == 1) {
            m.val = convertStr<int64_t>(words[0]);
            m.op = '!';
            // incoming[name] = 0;
        }
        else {
            m.op = words[1][0];
            m.left = words[0];
            m.right = words[2];
        }
        monkeys[name] = std::move(m);
    }

    part_1(monkeys["root"].getVal(monkeys));

    const std::string human = "humn";
    std::string current;
    int64_t cval;
    if (monkeys[monkeys["root"].left].contains(human, monkeys)) {
        current = monkeys["root"].left;
        cval = monkeys[monkeys["root"].right].getVal(monkeys);
    }
    else {
        current = monkeys["root"].right;
        cval = monkeys[monkeys["root"].left].getVal(monkeys);
    }

    while (current != human) {
        const std::string& left = monkeys[current].left;
        const std::string& right = monkeys[current].right;
        const std::string& h = monkeys[left].contains(human, monkeys) ? left : right;
        const std::string& other = h == left ? right : left;
        switch (monkeys[current].op) {
            case '+':
                cval = cval - monkeys[other].getVal(monkeys);
                break;
            case '-':
                if (left == h) {
                    cval = cval + monkeys[other].getVal(monkeys);
                }
                else {
                    cval = monkeys[other].getVal(monkeys) - cval;
                }
                break;
            case '*':
                cval = cval / monkeys[other].getVal(monkeys);
                break;
            case '/':
                if (left == h) {
                    cval = cval * monkeys[other].getVal(monkeys);
                }
                else {
                    cval = monkeys[other].getVal(monkeys) / cval;
                }
                break;
        }
        current = h;
    }
    part_2(cval);
}
