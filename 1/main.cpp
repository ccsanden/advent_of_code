#include <iostream>
#include <string>
#include <vector>
#include <chrono>

int main(const int argc, char const* argv[])
{
    int32_t sum = 0;
    std::vector<int32_t> inputs;
    std::vector<int32_t> seen_frequencies;

    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(std::stoi(line));
    }

    seen_frequencies.reserve(inputs.size());

    for (const auto i : inputs) {
        sum += i;
        seen_frequencies.push_back(sum);
    }

    while (true) {
        for (const auto i : inputs) {
            sum += i;
            for (const auto f : seen_frequencies) {
                if (f == sum) {
                    std::cout << "Result: " << f << "\n";
                    return 0;
                }
            }
        }
    }

    return 1;
}
