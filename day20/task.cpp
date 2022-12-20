#include "../base.cpp"

int64_t groveCoordinates(const std::list<int64_t>& sequence) {
    auto it = std::find(sequence.begin(), sequence.end(), 0);
    int64_t sum = 0;
    for (int i = 0; i < 3; i += 1) {
        for (int i = 0; i < 1000; i += 1) {
            it = next_looping(it, sequence.begin(), sequence.end());
        }
        sum += *it;
    }
    return sum;
}

void mix(std::list<int64_t>& sequence, std::vector<std::list<int64_t>::iterator>& order) {
    const unsigned loopSize = sequence.size() - 1;
    for (auto& it : order) {
        int64_t val = *it;
        int64_t n = val % loopSize;
        if (n < 0) n += loopSize;
        it = sequence.erase(it);
        if (it == sequence.end()) it = sequence.begin();
        for (int64_t i = 0; i < n; i += 1) {
                it = next_looping(it, sequence.begin(), sequence.end());
        }
        it = sequence.insert(it, val);
    }
}

int main() {
    const int64_t KEY = 811589153;

    std::ifstream ifs{"input.txt"};
    std::list<int64_t> sequence, sequence2;
    int64_t x;
    while (ifs >> x) {
        sequence.push_back(x);
    }
    sequence2 = sequence;
    std::vector<std::list<int64_t>::iterator> order;
    for (auto it = sequence.begin(); it != sequence.end(); it++) {
        order.push_back(it);
    }

    mix(sequence, order);
    part_1(groveCoordinates(sequence));

    std::vector<std::list<int64_t>::iterator> order2;
    for (auto it = sequence2.begin(); it != sequence2.end(); it++) {
        order2.push_back(it);
        *it *= KEY;
    }
    for (int i = 0; i < 10; i += 1) {
        mix(sequence2, order2);
    }
    part_2(groveCoordinates(sequence2));
}
