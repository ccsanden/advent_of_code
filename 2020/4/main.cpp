#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

/// Returns the number of valid passports
size_t parse_file(std::string input_file)
{
    std::ifstream indata{input_file};

    size_t count_valid = 0u;

    std::string entry;
    bool has_byr = false;
    bool has_iyr = false;
    bool has_eyr = false;
    bool has_hgt = false;
    bool has_hcl = false;
    bool has_ecl = false;
    bool has_pid = false;

    const std::string amb{"amb"};
    const std::string blu{"blu"};
    const std::string brn{"brn"};
    const std::string gry{"gry"};
    const std::string grn{"grn"};
    const std::string hzl{"hzl"};
    const std::string oth{"oth"};

    while(std::getline(indata, entry)) {
        // Ignore CID (it is optional)
        //
        if (entry.size() != 0)  // blank lines are new passport entries
        {
            std::istringstream f{entry};
            std::string passport_entry;
            while (std::getline(f, passport_entry, ' '))
            {
                const auto colon_pos = passport_entry.find(':');
                if (colon_pos != std::string::npos)
                {
                    if (colon_pos == 3u)
                    {
                        const auto t = passport_entry.substr(0u, colon_pos);
                        const auto val = passport_entry.substr(colon_pos + 1);
                        if (t == "byr")
                        {
                            try
                            {
                                auto int_val = std::stol(val);
                                if (int_val >= 1920 && int_val <= 2002)
                                {
                                    has_byr = true;
                                }
                            } catch (std::invalid_argument&)
                            {
                                std::cerr << "Invalid byr: " << val << "\n";
                            }
                        }
                        if (t == "iyr")
                        {
                            try
                            {
                                auto int_val = std::stol(val);
                                if (int_val >= 2010 && int_val <= 2020)
                                {
                                    has_iyr = true;
                                }
                            } catch (std::invalid_argument&)
                            {
                                std::cerr << "Invalid iyr: " << val << "\n";
                            }
                        }
                        if (t == "eyr")
                        {
                            try
                            {
                                const auto int_val = std::stol(val);
                                if (int_val >= 2020 && int_val <= 2030)
                                {
                                    has_eyr = true;
                                }
                            } catch (std::invalid_argument)
                            {
                                std::cerr << "Invalid eyr: " << val << "\n";
                            }
                        }
                        if (t == "hgt")
                        {
                            try
                            {
                                const auto int_val = std::stol(val.substr(0, val.size() - 2u));
                                const auto unit = val.substr(val.size() - 2u);
                                if (unit == "in")
                                {  // Inches
                                    has_hgt = (int_val >= 59 && int_val <= 76);
                                } else if (unit == "cm")
                                {
                                    has_hgt = (int_val >= 150 && int_val <= 193);
                                } else {
                                    std::cerr << "Invalid hgt: " << val << "\n";
                                }
                            } catch (std::invalid_argument&)
                            {
                                std::cerr << "Invalid hgt: " << val << "\n";
                            }

                        }
                        if (t == "hcl")
                        {
                            if (val.size() == 7u && val[0] == '#')
                            {

                                has_hcl = std::all_of(std::begin(val) + 1, std::end(val), [] (const char c) -> bool {
                                        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z');
                                });
                            }
                        }
                        if (t == "ecl")
                        {
                            if (amb == val || blu == val || brn == val || gry == val
                                || hzl == val || oth == val || grn == val)
                            {
                                has_ecl = true;
                            } else
                            {
                                std::cerr << "Unrecognized eye color: " << val << "\n";
                            }
                        }
                        if (t == "pid")
                        {
                            if (val.size() == 9u)
                            {
                                try {
                                    (void)std::stoll(val);
                                    has_pid = true;
                                } catch (std::invalid_argument&)
                                {
                                    std::cerr << "Invalid pid: " << val << "\n";
                                }
                            }
                        }
                    }
                }
            }
        } else
        {

            if (has_byr && has_iyr && has_eyr && has_hgt && has_hcl && has_ecl && has_pid)
            {
                count_valid++;
            }
            has_byr = false;
            has_iyr = false;
            has_eyr = false;
            has_hgt = false;
            has_hcl = false;
            has_ecl = false;
            has_pid = false;
        }
    }
    if (has_byr && has_iyr && has_eyr && has_hgt && has_hcl && has_ecl && has_pid)
    {
        count_valid++;
    }

    return count_valid;
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

    auto count_valid = parse_file(input_file);
    std::cout << "Part 1: " << count_valid << " valid passports\n";

    return 0;
}
