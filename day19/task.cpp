#include "../base.cpp"
#include <atomic>
#include <thread>

struct Blueprint {
    int number;
    int ore;
    int clay;
    int obsidian[2];
    int geode[2];

    Blueprint(int number, int ore, int clay, int ob_ore, int ob_clay, int geo_ore, int geo_ob)
    : number{number}, ore{ore}, clay{clay}, obsidian{ob_ore, ob_clay}, geode{geo_ore, geo_ob}
    {}

    int maxOre() const {
        return std::max({ore, clay, obsidian[0], geode[0]});
    }

    int maxClay() const {
        return obsidian[1];
    }

    int maxObsidian() const {
        return geode[1];
    }
};

struct State {
    int bots[4] = {1, 0, 0, 0};
    int mats[4] = {0};

    void tick() {
        for (int i = 0; i < 4; i += 1) {
            mats[i] += bots[i];
        }
    }

    bool canBuild(const Blueprint& bp, int idx) const {
        switch (idx) {
            case 0:
                if (bots[0] >= bp.maxOre() || bp.ore > mats[0])
                    return false;
                return true;
                break;
            case 1:
                if (bots[1] >= bp.maxClay() || bp.clay > mats[0])
                    return false;
                return true;
                break;
            case 2:
                if (bots[2] >= bp.maxObsidian() || bp.obsidian[0] > mats[0] || bp.obsidian[1] > mats[1])
                    return false;
                return true;
                break;
            case 3:
                if (bp.geode[0] > mats[0] || bp.geode[1] > mats[2])
                    return false;
                return true;
                break;
        }
        return false;
    }

    void buildBot(const Blueprint& bp, int idx) {
        switch (idx) {
            case 0:
                mats[0] -= bp.ore;
                break;
            case 1:
                mats[0] -= bp.clay;
                break;
            case 2:
                mats[0] -= bp.obsidian[0];
                mats[1] -= bp.obsidian[1];
                break;
            case 3:
                mats[0] -= bp.geode[0];
                mats[2] -= bp.geode[1];
                break;
        }
        bots[idx] += 1;
    }

};

int very_happy_path(int time_left, int current_geode_bots) {
    return time_left * (current_geode_bots + current_geode_bots + time_left - 1) / 2;
}

int dfs_impl(const Blueprint& bp, int budget, State s, int& max) {
    if (budget == 0) {
        max = std::max(max, s.mats[3]);
        return s.mats[3];
    }
    if (max != 0 && very_happy_path(budget, s.bots[3]) + s.mats[3] < max)
        return max; //nie ma szans na lepszy wynik

    int result = 0;
    bool build[4];
    for (int i = 0; i < 4; i += 1) {
        build[i] = s.canBuild(bp, i);
    }

    s.tick();

    for (int i = 3; i >= 0; i -= 1) {
        if (build[i]) {
            State tmp = s;
            tmp.buildBot(bp, i);
            result = std::max(result, dfs_impl(bp, budget - 1, tmp, max));
        }
    }
    result = std::max(result, dfs_impl(bp, budget - 1, s, max));
    return result;
}

int dfs(const Blueprint& bp, int budget) {
    int max = 0;
    State s;
    return dfs_impl(bp, budget, s, max);
}

std::ostream& operator<<(std::ostream& os, const Blueprint& b) {
    os << "Blueprint:\n";
    os << "Ore bot: " << b.ore << " ore\n";
    os << "Clay bot: " << b.clay << " ore\n";
    os << "Obsidian bot: " << b.obsidian[0] << " ore and " << b.obsidian[1] << " clay\n";
    os << "Geode bot: " << b.geode[0] << " ore and " << b.geode[1] << " obsidian\n";
    return os;
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::regex re{"Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian."};
    auto data = getLines(ifs);

    std::vector<Blueprint> blueprints;

    for (const auto& line : data) {
        std::smatch match;
        if (std::regex_search(line, match, re)) {
            int a[7];
            for (int i = 0; i < 7; i += 1) {
                a[i] = convertStr<int>(match[i + 1]);
            }
            blueprints.emplace_back(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
        }
    }

    std::atomic<int> sum = 0;
    std::vector<std::thread> t;
    for (const Blueprint& bp : blueprints) {
        t.emplace_back(std::thread([&sum, &bp]() {
            sum.fetch_add(bp.number * dfs(bp, 24), std::memory_order_relaxed);
            // std::cout << "Blueprint " << bp.number << " done." << std::endl;
        }));
    }
    for (std::thread& th : t)
        th.join();

    part_1(sum);

    // std::cout << "Calculating part 2:\n";
    t.clear();
    int p[3];
    for (int i = 0; i < 3; i += 1) {
        const auto& bp = blueprints[i];
        t.emplace_back(std::thread([&p, &bp]() {
            p[bp.number - 1] = dfs(bp, 32);
            // std::cout << "Blueprint " << bp.number << " done." << std::endl;
        }));
    }
    for (std::thread& th : t)
        th.join();
    part_2(p[0] * p[1] * p[2]);
}
