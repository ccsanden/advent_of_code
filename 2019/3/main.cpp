#include <limits>
#include <array>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <fstream>

struct PositionHash {
    std::size_t operator() (const std::pair<int32_t, int32_t>& p) const
    {
        return std::hash<int32_t>()(p.first) ^ std::hash<int32_t>()(p.second);
    }
};

using WiresSteps = std::array<size_t, 2u>;

// X,Y offsets from origin (0,0) and how many wires cross the point.
using Positions = std::unordered_map<std::pair<int32_t, int32_t>, WiresSteps, PositionHash>;

namespace
{
    std::vector<std::string> tokenize(const std::string& input_line)
    {
        std::vector<std::string> retval;
        retval.reserve(500u);

        std::string now = "";
        for (const auto c : input_line) {
            if (c == ',') {
                retval.push_back(now);
                now = "";
            } else {
                now.push_back(c);
            }
        }
        if (now.length() > 0u) {
            retval.push_back(now);
        }
        return retval;
    }

    int test()
    {
        return 0;
    }
}

void process_path(size_t id, const std::vector<std::string>& path_descriptors, Positions& paths)
{
    int32_t curr_x = 0;
    int32_t curr_y = 0;
    size_t curr_step = 0u;
    for (const auto& pd : path_descriptors) {
        int32_t steps = std::stoi(pd.substr(1u, std::string::npos));
        if (pd[0] == 'R') {
            for (int32_t i = 0u; i < steps; ++i) {
                curr_step += 1u;
                curr_x += 1;
                auto& curr = paths[std::make_pair(curr_x, curr_y)];
                if (curr[id] == 0u) {
                    curr[id] = curr_step;
                }
            }
        } else if (pd[0] == 'L') {
            for (int32_t i = 0u; i < steps; ++i) {
                curr_step += 1u;
                curr_x -= 1;
                auto& curr = paths[std::make_pair(curr_x, curr_y)];
                if (curr[id] == 0u) {
                    curr[id] = curr_step;
                }
            }
        } else if (pd[0] == 'U') {
            for (int32_t i = 0u; i < steps; ++i) {
                curr_step += 1u;
                curr_y -= 1;
                auto& curr = paths[std::make_pair(curr_x, curr_y)];
                if (curr[id] == 0u) {
                    curr[id] = curr_step;
                }
            }
        } else if (pd[0] == 'D') {
            for (int32_t i = 0u; i < steps; ++i) {
                curr_step += 1u;
                curr_y += 1;
                auto& curr = paths[std::make_pair(curr_x, curr_y)];
                if (curr[id] == 0u) {
                    curr[id] = curr_step;
                }
            }
        }
    }
}

std::vector<Positions::key_type> get_intersections(const Positions& paths)
{
    std::vector<std::pair<int32_t, int32_t>> retval;
    for (const auto& entry : paths) {
        if (entry.second[0u] > 0u && entry.second[1u] > 0u) {
            // Both wires cross
            retval.push_back(entry.first);
        }
    }
    return retval;
}

size_t manhattan_from_origo(const std::pair<int32_t, int32_t>& pos)
{
    return abs(pos.first) + abs(pos.second);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    if (strcmp(argv[1], "test") == 0) {
        return test();
    }

    std::ifstream indata{argv[1]};
    std::string line1;
    std::string line2;
    std::getline(indata, line1);
    std::getline(indata, line2);

    Positions paths;

    process_path(0u, tokenize(line1), paths);
    process_path(1u, tokenize(line2), paths);

    std::vector<std::pair<int32_t, int32_t>> intersections = get_intersections(paths);

    std::cout << "Found " << intersections.size() << " intersection points.\n";

    size_t min_dist = std::numeric_limits<size_t>::max();
    size_t min_tot_steps = std::numeric_limits<size_t>::max();
    for (auto i : intersections) {
        const auto d = manhattan_from_origo(i);
        if (d < min_dist) {
            min_dist = d;
        }
        const auto& steps = paths.at(i);
        const auto steps_taken = steps[0u] + steps[1u];
        if (steps_taken < min_tot_steps) {
            min_tot_steps = steps_taken;
        }

    }

    std::cout << "Part 1: " << min_dist << "\n";
    std::cout << "Part 2: " << min_tot_steps << "\n";

    return 0;
}
