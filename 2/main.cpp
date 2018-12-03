#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[])
{
    uint32_t count_two = 0u;
    uint32_t count_three = 0u;

    std::vector<std::string> lines;

    if (argc == 2 && std::string(argv[1]) == "test") {
        lines.push_back("abcdef");
        lines.push_back("bababc");
        lines.push_back("abbcde");
        lines.push_back("abcccd");
        lines.push_back("aabcdd");
        lines.push_back("abcdee");
        lines.push_back("ababab");
    } else {
        for (std::string line; std::getline(std::cin, line);) {
            lines.emplace_back(std::move(line));
        }
    }

    for (const auto& line : lines) {
        std::unordered_map<unsigned char, uint16_t> occur;
        for (char c : line) {
            occur[c]++;
        }

        bool line_count_two = false;
        bool line_count_three = false;
        for (auto iter = occur.begin();
             iter != occur.end() && !(line_count_two && line_count_three);
             ++iter) {
            if (iter->second == 2u && !line_count_two) {
                line_count_two = true;
                count_two++;
            } else if (iter->second == 3u && !line_count_three) {
                line_count_three = true;
                count_three++;
            }
        }
    }

    std::cout << "Result: " << count_two * count_three << "\n";
    return 0;
}
