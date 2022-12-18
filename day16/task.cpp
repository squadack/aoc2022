#include "../base.cpp"

struct Valve {
    int rate;
    std::vector<std::string> next;
};

struct Edge {
    std::string to;
    int cost;
};

std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << '(' << e.to << ", " << e.cost << ')';
    return os;
}

std::vector<Edge> bfs(const std::map<std::string, Valve>& tunnels, const std::string& start) {
    std::map<std::string, int> cost;
    std::queue<std::string> q;
    cost[start] = 0;
    q.push(start);
    while (!q.empty()) {
        std::string s = q.front();
        q.pop();
        for (const std::string& neigh : tunnels.at(s).next) {
            if (cost.find(neigh) != cost.end())
                continue;
            cost[neigh] = cost[s] + 1;
            q.push(neigh);
        }
    }
    std::vector<Edge> result;
    for (const auto& [name, v] : tunnels) {
        if (v.rate > 0 && name != start)
            result.emplace_back(Edge{name, cost[name] + 1}); //NOTE opening included
    }
    return result;
}

void dfs(const std::map<std::string, std::vector<Edge>>& g, std::map<std::string, int>& flow, const std::string& current, int budget, int& max, int c = 0, bool human = true) {
    bool none = true;
    for (const auto& edge : g.at(current)) {
        if (flow[edge.to] > 0 && budget >= edge.cost) {
            none = false;

            int f = flow[edge.to];
            flow[edge.to] = 0;
            int result_increase = f * (budget - edge.cost);

            dfs(g, flow, edge.to, budget - edge.cost, max, c + result_increase, human);

            flow[edge.to] = f;
        }
    }
    if (human) {
        int m = 0;
        dfs(g, flow, "AA", 26, m, 0, false);
        max = std::max(c + m, max);
    }
    if (none && !human) {
        max = std::max(c, max);
    }
}

int main() {
    std::ifstream ifs{"input.txt"};
    std::regex re{"Valve (\\w{2}) has flow rate=(\\d+); tunnels? leads? to valves? ((?:\\w{2})(?:, (?:\\w{2}))*)"};
    auto data = getLines(ifs);
    std::map<std::string, Valve> tunnels;
    for (const auto& line : data) {
        std::smatch match;
        if (std::regex_search(line, match, re)) {
            Valve v;
            v.rate = convertStr<int>(match[2]);
            v.next = split(match[3], ", ");
            tunnels[match[1]] = std::move(v);
        }
    }

    std::map<std::string, std::vector<Edge>> graph;
    std::map<std::string, int> flow;
    graph["AA"] = bfs(tunnels, "AA");
    flow["AA"] = 0;
    for (const auto& [name, v] : tunnels) {
        if (v.rate > 0) {
            graph[name] = bfs(tunnels, name);
            flow[name] = v.rate;
        }
    }

    int p = 0;
    dfs(graph, flow, "AA", 30, p, 0, false);
    part_1(p);

    std::map<std::string, int> flow2;
    int myrun = 0;
    int elerun = 0;
    dfs(graph, flow, "AA", 26, myrun);
    part_2(myrun);
}
