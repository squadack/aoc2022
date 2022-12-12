#include "../base.cpp"

class Monkey {
public:
    void takeTurn(std::vector<Monkey>& monkeys, int64_t mod = 0) {
        for (int64_t x : inventory) {
            x = operation(x);
            if (mod == 0)
                x /= 3;
            else
                x %= mod;
            if (x % test_divisor == 0)
                monkeys[target_true].receive(x);
            else
                monkeys[target_false].receive(x);
        }
        inspection_count += inventory.size();
        inventory.clear();
    }

    void receive(int64_t val) {
        inventory.push_back(val);
    }

    int getInspectionCount() const { return inspection_count; }
    int getTestDivisor() const { return test_divisor; }

    friend std::istream& operator>>(std::istream&, Monkey&);
private:
    std::vector<int64_t> inventory;
    std::function<int64_t(int64_t)> operation;
    int test_divisor;
    int target_true;
    int target_false;
    int inspection_count = 0;
};

std::istream& operator>>(std::istream& is, Monkey& m) {
    std::string dump;
    std::string line;
    is >> dump >> dump; //Monkey N:

    if (!is) return is; //no monkey to parse

    is >> dump >> dump; //Starting items:
    std::getline(is, line);
    m.inventory = getDataStr<int64_t>(line, ", ");

    is >> dump >> dump >> dump; //Operation: new =
    if (is.peek() == ' ') is.get(); // dangling space
    std::getline(is, line);
    auto op = split(line);
    m.operation = [op](int64_t old) {
        int64_t left = op[0] == "old" ? old : convertStr<int64_t>(op[0]);
        int64_t right = op[2] == "old" ? old : convertStr<int64_t>(op[2]);
        if (op[1] == "+")
            return left + right;
        else if (op[1] == "*")
            return left * right;
        return 0l;
    };

    is >> dump >> dump >> dump >> m.test_divisor; //Test: divisible by test_divisor
    is >> dump >> dump >> dump >> dump >> dump >> m.target_true; // If true: throw to monkey TARGET
    is >> dump >> dump >> dump >> dump >> dump >> m.target_false; // If false: throw to monkey TARGET
    return is;
}

int64_t monkey_business(const std::vector<Monkey>& monkeys) {
    std::vector<int64_t> inspection_counts;
    for (const auto& m : monkeys)
        inspection_counts.push_back(m.getInspectionCount());
    std::sort(inspection_counts.rbegin(), inspection_counts.rend());

    std::cout << "Inspecion counts: " << inspection_counts << '\n';

    return inspection_counts[0] * inspection_counts[1];
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::vector<Monkey> monkeys;
    Monkey m;
    while (ifs >> m)
        monkeys.emplace_back(std::move(m));

    std::vector<Monkey> monkeys_2 = monkeys;

    for (int i = 0; i < 20; i += 1) {
        for (Monkey& m : monkeys)
            m.takeTurn(monkeys);
    }
    part_1(monkey_business(monkeys));

    int64_t mod = 1;
    for (const Monkey& m : monkeys_2)
        mod *= m.getTestDivisor();

    for (int i = 0; i < 10000; i += 1) {
        for (Monkey& m : monkeys_2)
            m.takeTurn(monkeys_2, mod);
    }
    part_2(monkey_business(monkeys_2));
}
