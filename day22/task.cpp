#include "../base.cpp"

struct Terrain {
    std::vector<std::string> _map;

    Terrain(std::vector<std::string> map) : _map{std::move(map)} {}

    int next_x(int x, int y) const {
        do {
            x = (x + 1) % _map.at(y).size();
        } while (_map.at(y)[x] == ' ');
        return x;
    }

    int prev_x(int x, int y) const {
        do {
            x = (x + _map.at(y).size() - 1) % _map.at(y).size();
        } while (_map.at(y)[x] == ' ');
        return x;
    }

    int next_y(int x, int y) const {
        do {
            y = (y + 1) % _map.size();
        } while (_map.at(y).size() <= x || _map.at(y)[x] == ' ');
        return y;
    }

    int prev_y(int x, int y) const {
        do {
            y = (y + _map.size() - 1) % _map.size();
        } while (_map.at(y).size() <= x || _map.at(y)[x] == ' ');
        return y;
    }

    const std::string& operator[](unsigned idx) const {
        return _map.at(idx);
    }
};

void flip_dir(std::tuple<int, int, int>& a) {
    std::get<2>(a) = (std::get<2>(a) + 2) % 4;
}

const int RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3;

const int CUBE_SIZE = 50;
struct Cube {
    int _size = 50;
    std::vector<std::string> _map;
    std::map<std::tuple<int, int, int>, std::tuple<int, int, int>> wrap; //x, y, dir

    Cube(std::vector<std::string> map) : _size{CUBE_SIZE}, _map{std::move(map)} {
        //Hardcoded for shape:
        //  ##
        //  #
        // ##
        // #

        // a.x, a.y, a.dir, a.dx, a.dy, b.x, b.y, b.dir,. b.dx, b.dy
        const int edges[7][10] = {
            {2 * _size - 1, 1 * _size    , RIGHT, 0, 1, 2 * _size    , 1 * _size - 1, UP   , 1 , 0 }, //1
            {2 * _size - 1, 2 * _size    , RIGHT, 0, 1, 3 * _size - 1, 1 * _size - 1, LEFT , 0 , -1}, //2
            {1 * _size - 1, 3 * _size    , RIGHT, 0, 1, 1 * _size    , 3 * _size - 1, UP   , 1 , 0 }, //3
            {0            , 4 * _size - 1, DOWN , 1, 0, 2 * _size    , 0            , DOWN , 1 , 0 }, //4
            {0            , 2 * _size    , UP   , 1, 0, 1 * _size    , 1 * _size    , RIGHT, 0 , 1 }, //5
            {0            , 2 * _size    , LEFT , 0, 1, 1 * _size    , 1 * _size - 1, RIGHT, 0 , -1}, //6
            {0            , 3 * _size    , LEFT , 0, 1, 1 * _size    , 0            , DOWN , 1 , 0 }  //7
        };

        for (int i = 0; i < 7; i += 1) {
            std::tuple<int, int, int> a, b;
            a = std::make_tuple(edges[i][0], edges[i][1], edges[i][2]);
            b = std::make_tuple(edges[i][5], edges[i][6], edges[i][7]);
            for (int k = 0; k < _size; k += 1) {
                wrap[a] = b;
                flip_dir(a);
                flip_dir(b);
                wrap[b] = a;
                flip_dir(a);
                flip_dir(b);
                std::get<0>(a) += edges[i][3];
                std::get<1>(a) += edges[i][4];
                std::get<0>(b) += edges[i][8];
                std::get<1>(b) += edges[i][9];
            }
        }
    }

    void step(int& x, int& y, int& dir) {
        std::tuple<int, int, int> a = std::make_tuple(x, y, dir);
        if (wrap.find(a) != wrap.end()) {
            auto b = wrap.at(a);
            x = std::get<0>(b);
            y = std::get<1>(b);
            dir = std::get<2>(b);
            return;
        }
        switch (dir) {
            case 0:
                x += 1;
                break;
            case 1:
                y += 1;
                break;
            case 2:
                x -= 1;
                break;
            case 3:
                y -= 1;
                break;
        }
    }

    const std::string& operator[](unsigned idx) const {
        return _map.at(idx);
    }
};

int main() {
    std::ifstream ifs{"input.txt"};
    std::vector<std::string> map;
    std::string line;
    while (std::getline(ifs, line), !line.empty()) {
        map.emplace_back(std::move(line));
    }
    Terrain t{map};
    Cube cube{map};
    int x, cube_x, y, cube_y, dir, cube_dir;
    x = cube_x = CUBE_SIZE;
    y = cube_y = 0;
    dir = cube_dir = 0;

    int n;
    char c;
    while (ifs >> n) {
        while (n--) {
            int nx = x;
            int ny = y;

            int cube_nx = cube_x;
            int cube_ny = cube_y;
            int cube_ndir = cube_dir;
            cube.step(cube_nx, cube_ny, cube_ndir);

            switch (dir) {
                case 0:
                    nx = t.next_x(nx, ny);
                    break;
                case 1:
                    ny = t.next_y(nx, ny);
                    break;
                case 2:
                    nx = t.prev_x(nx, ny);
                    break;
                case 3:
                    ny = t.prev_y(nx, ny);
                    break;
            }
            if (t[ny][nx] != '#') {
                x = nx;
                y = ny;
            }
            if (cube[cube_ny][cube_nx] != '#') {
                cube_x = cube_nx;
                cube_y = cube_ny;
                cube_dir = cube_ndir;
            }
        }
        ifs >> c;
        if (!ifs)
            break;
        if (c == 'R') {
            dir = (dir + 1) % 4;
            cube_dir = (cube_dir + 1) % 4;
        }
        else {
            dir = (dir + 3) % 4;
            cube_dir = (cube_dir + 3) % 4;
        }
    }

    int result = (y + 1) * 1000 + (x + 1) * 4 + dir;
    part_1(result);

    int cube_result = (cube_y + 1) * 1000 + (cube_x + 1) * 4 + cube_dir;
    part_2(cube_result);
}
