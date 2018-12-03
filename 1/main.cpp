#include <iostream>
#include <string>
int main(const int argc, char const* argv[])
{
    int32_t sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        sum += std::stoi(line);
    }
    std::cout << "Result: " << sum << "\n";
    return 0;
}
