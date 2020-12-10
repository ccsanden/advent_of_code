#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>

std::pair<bool, bool> is_password_valid(std::string policy, const std::string& passwd)
{
    auto dash_pos = policy.find('-', 1);
    auto space_pos = policy.find(' ', dash_pos);

    size_t min_count = std::stoul(policy.substr(0, dash_pos));
    size_t max_count = std::stoul(policy.substr(dash_pos + 1, space_pos));
    char character = policy[space_pos + 1];
    std::cout << "Max count=" << max_count << ", min count=" << min_count << ", character=" << character << "\n";

    bool p1_ok = true;
    bool p2_ok = false;

    size_t char_count = 0u;
    for (size_t i = 0u; p1_ok && i < passwd.size(); i++)
    {
        if (passwd[i] == character)
        {
            char_count++;
            if (char_count > max_count)
            {
                p1_ok = false;
            }
        }
    }

    if (char_count < min_count)
    {
        p1_ok = false;
    }

    if (passwd[min_count - 1] == character && passwd[max_count - 1] != character)
    {
        p2_ok = true;
    } else if (passwd[min_count - 1] != character && passwd[max_count -1 ] == character)
    {
        p2_ok = true;
    }

    return {p1_ok, p2_ok};
}

bool parse_file(std::string input_file)
{
    std::ifstream indata{input_file};

    size_t valid_passwords = 0u;
    size_t valid_passwords_part_2 = 0u;

    std::string entry;
    while(std::getline(indata, entry)) {
        auto colon_pos = entry.find(':', 4u);

        if (colon_pos == std::string::npos)
        {
            std::cerr << "no colon in: " << entry << "\n";
            return false;
        }

        auto passwd = entry.substr(colon_pos + 2);
        auto policy = entry.substr(0, colon_pos);

        std::cout << "policy: " << policy << ", passwd: " << passwd << "\n";

        auto [valid_p1, valid_p2] = is_password_valid(policy, passwd);

        if (valid_p1)
        {
            valid_passwords++;
        }

        if (valid_p2)
        {
            valid_passwords_part_2++;
        }
    }

    std::cout << "Part 1: Number of valid passwords: " << valid_passwords << "\n";
    std::cout << "Part 2: Number of valid passwords: " << valid_passwords_part_2 << "\n";

    return true;
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


    if (parse_file(input_file))
    {
        return 0;
    }
    return 2;
}
