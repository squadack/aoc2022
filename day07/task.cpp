#include "../base.cpp"

struct File {
    std::string name;
    uint32_t size;

    File(std::string name, uint32_t size) : name{std::move(name)}, size{size} {}
};

struct Dir {
    std::vector<File> files;
    std::map<std::string, Dir> dirs;
    Dir* parentDir;

    Dir(Dir* parent = nullptr) : parentDir{parent} {}

    void addFile(const File& f) { files.emplace_back(f); }
    void addDir(const std::string& name) { dirs[name] = Dir{this}; }

    uint32_t size() const {
        uint32_t s = 0;
        for (const File& f : files)
            s += f.size;
        for (const auto& d : dirs)
            s += d.second.size();
        return s;
    }

    uint32_t sum_of_dir_sizes() const { //part_1
        uint32_t sum = size();
        if (sum > 100000)
            sum = 0;
        for (const auto& d : dirs)
            sum += d.second.sum_of_dir_sizes();
        return sum;
    }

    uint32_t find_dir_bigger_than(uint32_t space_needed) const { //part_2
        uint32_t result = size();
        for (const auto& d : dirs) {
            const uint32_t child_best = d.second.find_dir_bigger_than(space_needed);
            if (child_best >= space_needed)
                result = std::min(result, child_best);
        }
        return result;
    }
};


int main() {
    std::ifstream ifs("input.txt");
    auto lines = getLines(ifs);
    Dir fs;
    fs.addDir("/");

    Dir* current = &fs;

    for (const auto& line : lines) {
        auto tokens = split(line);
        if (tokens[0] == "$") {
            if (tokens[1] == "cd") {
                if (tokens[2] == "/")
                    current = &fs.dirs["/"];
                else if (tokens[2] == "..")
                    current = current->parentDir;
                else
                    current = &current->dirs[tokens[2]];
            }
        }
        else {
            if (tokens[0] == "dir")
                current->addDir(tokens[1]);
            else {
                uint32_t size = convertStr<uint32_t>(tokens[0]);
                current->addFile(File{tokens[1], size});
            }
        }
    }

    part_1(fs.sum_of_dir_sizes());

    const uint32_t total_disk_space = 70000000;
    const uint32_t free_disk_space = total_disk_space - fs.size();
    const uint32_t space_needed = 30000000 - free_disk_space;

    part_2(fs.find_dir_bigger_than(space_needed));
}
