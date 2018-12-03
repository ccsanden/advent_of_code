#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string string_intersection(const std::string& a, const std::string&b)
{
    std::ostringstream ostr;

    for(size_t i = 0u; i < a.length(); ++i) {
        if (a.at(i) == b.at(i)) {
            ostr << a.at(i);
        }
    }

    return ostr.str();
}

int main(int argc, char* argv[])
{

    std::vector<std::string> lines;

    for (std::string line; std::getline(std::cin, line);) {
        lines.emplace_back(std::move(line));
    }

    for (size_t i = 0u; i < lines.size(); ++i) {
        const std::string& a(lines[i]);
        for (size_t j = i + 1u; j < lines.size(); ++j) {
            const std::string& b(lines[j]);
            const auto intersection = string_intersection(a, b);

            if (a.length() - intersection.length() == 1u) {
                std::cout << "Result: " << intersection << "\n";
                return 0;
            }
        }
    }

    return 1;
}
