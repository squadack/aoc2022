#include "../base.cpp"

const int NEAR[6][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

struct Cube {
    int x, y, z;

    Cube() = default;
    Cube(int x, int y, int z) : x{x}, y{y}, z{z} {}

    int hash() const {
        int h = x;
        h *= 100;
        h += y;
        h *= 100;
        h += z;
        return h;
    }

    bool operator<(const Cube& x) const {
        return hash() < x.hash();
    }
};

int outerSides(const std::set<Cube>& droplet) {
    int min, max;
    min = max = droplet.begin()->x;
    for (const Cube& c : droplet) {
        min = std::min({min, c.x, c.y, c.z});
        max = std::max({max, c.x, c.y, c.z});
    }
    max += 1;
    min -= 1;
    std::set<Cube> visited;
    std::queue<Cube> q;
    Cube start{min, min, min};
    q.push(start);
    visited.insert(start);
    int count = 0;
    while (!q.empty()) {
        Cube c = q.front();
        q.pop();
        for (int i = 0; i < 6; i += 1) {
            Cube ne = c;
            ne.x += NEAR[i][0];
            ne.y += NEAR[i][1];
            ne.z += NEAR[i][2];
            if (visited.find(ne) != visited.end())
                continue;
            if (ne.x < min || ne.x > max || ne.y < min || ne.y > max || ne.z < min || ne.z > max)
                continue;
            if (droplet.find(ne) != droplet.end())
                count += 1;
            else {
                q.push(ne);
                visited.insert(ne);
            }
        }
    }
    return count;
}

int main() {
    std::ifstream ifs{"input.txt"};
    auto data = getLines(ifs);
    std::set<Cube> droplet;
    for (const auto& s : data) {
        Cube c;
        auto v = getDataStr<int>(s, ",");
        c.x = v[0];
        c.y = v[1];
        c.z = v[2];
        droplet.emplace(std::move(c));
    }
    int count = 0;
    for (const Cube& c : droplet) {
        for (int i = 0; i < 6; i += 1) {
            Cube neighbour = c;
            neighbour.x += NEAR[i][0];
            neighbour.y += NEAR[i][1];
            neighbour.z += NEAR[i][2];
            if (droplet.find(neighbour) == droplet.end())
                count += 1;
        }
    }
    part_1(count);
    part_2(outerSides(droplet));
}
