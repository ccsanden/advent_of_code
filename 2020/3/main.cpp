#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

size_t do_strategy(const std::vector<bool>& trees, size_t width, size_t height, size_t x_vec, size_t y_vec);

// returns tuple (width, height) transforms "trees" to a vector width * height
// large in which a value 'true' == tree
std::tuple<size_t, size_t> parse_file(std::string input_file, std::vector<bool>& trees)
{
    std::ifstream indata{input_file};

    size_t width = 0u;
    bool set_width = false;
    size_t height = 0u;

    std::string entry;
    while(std::getline(indata, entry)) {
        if (!set_width)
        {
            width = entry.size();
        }
        else
        {
            assert(entry.size() == width);
        }
        for (size_t j = 0u; j < entry.size(); ++j)
        {
            trees.push_back(entry[j] == '#');
        }
        height += 1;
    }

    return {width, height};
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "argc: " << argc << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    auto input_file = std::string(argv[1]);

    std::vector<bool> trees;
    trees.reserve(1024u);
    auto [width, height] = parse_file(input_file, trees);

    const size_t num_fields = width * height;

    assert(num_fields == trees.size());

    auto part_1 = do_strategy(trees, width, height, 3u, 1u);

    auto part_2_1 = do_strategy(trees, width, height, 1u, 1u);
    auto part_2_3 = do_strategy(trees, width, height, 5u, 1u);
    auto part_2_4 = do_strategy(trees, width, height, 7u, 1u);
    auto part_2_5 = do_strategy(trees, width, height, 1u, 2u);

    std::cout << "Part 1: " << part_1 << "\n";
    std::cout << "Part 2: " << part_1 * part_2_1 * part_2_3 * part_2_4 * part_2_5 << "\n";

    return 0;
}

size_t do_strategy(const std::vector<bool>& trees, size_t width, size_t height, size_t x_vec, size_t y_vec)
{
    // Current position in the vector
    size_t num_trees = 0u;
    size_t y = 0u;
    size_t x = 0u;

    while(true)
    {
        if (trees[y * width + x])
        {  // Hit a tree...
            num_trees++;
        }

        y += y_vec;
        if (x + x_vec > (width - 1u))
        {
            x = x_vec - (width - x);
        } else
        {
            x += x_vec;
        }

        if (y > height)
        {
            break;
        }
    }
    return num_trees;

}
