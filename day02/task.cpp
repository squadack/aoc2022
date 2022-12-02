#include "../base.cpp"

int main() {
    const int pts[3][3] = {{4, 8, 3}, {1, 5, 9}, {7, 2, 6}};
    const int pts2[3][3] = {{3, 4, 8}, {1, 5, 9}, {2, 6, 7}};
    std::ifstream ifs{"input.txt"};
    char a, b;
    int sum = 0, sum2 = 0;
    while (ifs >> a >> b) {
        a -= 'A';
        b -= 'X';
        sum += pts[a][b];
        sum2 += pts2[a][b];
    }
    part_1(sum);
    part_2(sum2);
}
