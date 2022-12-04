#include "../base.cpp"

int main() {
    std::ifstream ifs{"input.txt"};
    int a, b, c, d;
    char z;
    int count = 0;
    int count2 = 0;
    while (ifs >> a >> z >> b >> z >> c >> z >> d) {
        if (a >= c && b <= d || c >= a && d <= b)
            count += 1;
        if (a >= c && a <= d || c >= a && c <= b)
            count2 += 1;
    }
    part_1(count);
    part_2(count2);
}
