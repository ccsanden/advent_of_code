#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <array>
#include <algorithm>


static constexpr size_t NUM_COUNT = 1024u;
static constexpr uint32_t SUM_TARGET = 2020u;


void to_array(std::array<uint64_t, NUM_COUNT> &nums, size_t& num_count, const std::string input_file)
{
    std::ifstream indata{input_file};

    std::string entry;
    while(std::getline(indata, entry)) {
        if (num_count >= NUM_COUNT)
        {
            throw std::runtime_error{"Not enough space in array"};
        }
        nums[num_count++] = std::stoull(entry);
    }
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

    std::array<uint64_t, NUM_COUNT> nums{};
    size_t count_nums = 0u;

    to_array(nums, count_nums, input_file);
    std::sort(std::begin(nums), std::begin(nums) + count_nums);

    size_t too_large_index = count_nums;
    for (; too_large_index != 0u; --too_large_index)
    {
        if (nums[too_large_index] < SUM_TARGET)
        {
            break;
        }
    }

    bool part1_done = false;
    bool part2_done = false;

    for (uint64_t i = 0u; i < too_large_index; ++i)
    {
        for (uint64_t j = i + 1; j < too_large_index; ++j)
        {
            if (!part1_done && SUM_TARGET == (nums[j] + nums[i]))
            {
                // Got it
                uint64_t product = nums[j] * nums[i];
                std::cout << "Part1: Answer is " << product << "\n";
                part1_done = true;
            }

            if (!part2_done)
            {
                for (uint64_t k = j + 1; k < too_large_index; ++k)
                {
                    if (SUM_TARGET == (nums[i] + nums[j] + nums[k]))
                    {
                        // Got it
                        uint64_t product = nums[j] * nums[i] * nums[k];
                        std::cout << "Part2: Answer is " << product << "\n";
                        part2_done = true;
                        break;
                    }
                }
            }

            if (part1_done && part2_done)
            {
                return 0;
            }
        }
    }

    std::flush(std::cout);

    return 2;
}
