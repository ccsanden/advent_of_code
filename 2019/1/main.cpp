#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

namespace
{
    uint32_t fuel_needed(const std::vector<int32_t>& masses, std::vector<int32_t>& fuel_masses)
    {
        int32_t total_fuel = 0u;

        for (const auto m : masses) {
            // Does not handle overflow
            int32_t mass_fuel = m / 3u - 2;
            if (mass_fuel > 0) {
                total_fuel += mass_fuel;
                fuel_masses.push_back(mass_fuel);
            }
        }
        return total_fuel;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return 1;
    }


    std::vector<int32_t> module_masses;
    module_masses.reserve(5000u);

    std::ifstream indata{argv[1]};
    std::string tmp;
    while (std::getline(indata, tmp)) {
        const auto mass = std::stoi(tmp);
        module_masses.push_back(static_cast<int32_t>(mass));
    }

    std::vector<int32_t> fuel_masses;
    fuel_masses.reserve(5000u);

    auto mass_fuel = fuel_needed(module_masses, fuel_masses);
    std::cout << "Part 1 answer is: " << mass_fuel << "\n";

    while (fuel_masses.size() > 0u) {
        module_masses.clear();
        mass_fuel += fuel_needed(fuel_masses, module_masses);
        std::swap(fuel_masses, module_masses);
        module_masses.clear();
    }

    std::cout << "Part 2 answer is: " << mass_fuel << "\n";

    return 0;
}
