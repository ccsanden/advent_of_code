#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <fstream>
#include <vector>
#include <tuple>
#include <vector>
#include <algorithm>
#include <functional>

/// Returns the number of valid passports
void parse_file(std::string input_file, std::vector<std::string>& boarding_passes)
{
    std::ifstream indata{input_file};

    std::string entry;

    while(std::getline(indata, entry)) {
        boarding_passes.emplace_back(std::move(entry));
    }
}

constexpr size_t seat_id(size_t row, size_t column)
{
    return row * 8 + column;
}

std::pair<size_t, size_t> get_row_column(const std::string boarding_pass)
{
    size_t row = 0u;
    size_t column = 0u;

    size_t range_start = 0u;
    size_t range_end = 127u;

    for (size_t i = 0u; i < 7; ++i)
    {
        size_t curr_range = range_end - range_start + 1;
        size_t half_range = curr_range / 2;
        switch (boarding_pass[i])
        {
            case 'F':
            {
                range_end -= half_range;
                break;
            }
            case 'B':
            {
                range_start += half_range;
                break;
            }
        }
    }
    row = range_start;

    range_start = 0u;
    range_end = 7u;
    for (size_t i = 7u; i < 10u; ++i)
    {
        const size_t curr_range = range_end - range_start + 1;
        const size_t half_range = curr_range / 2;

        switch (boarding_pass[i])
        {
            case 'R':
            {
                range_start += half_range;
                break;
            }
            case 'L':
            {
                range_end -= half_range;
                break;
            }
        }
    }

    column = range_start;

    return {row, column};
}

void test()
{
    if (seat_id(44, 5) != 357)
    {
        std::cerr << "seat_id is broken\n";
    }

    auto [row, column] = get_row_column("BFFFBBFRRR");
    if (row != 70u || column != 7)
    {
        std::cerr << "BFFFBBFRRR failed: row = " << row << ", column == " << column << "\n";
    }

    auto [row1, column1] = get_row_column("FFFBBBFRRR");
    if (row1 != 14u || column1 != 7)
    {
        std::cerr << "FFFBBBFRRR failed: row = " << row1 << ", column == " << column1 << "\n";
    }

    auto [row2, column2] = get_row_column("BBFFBBFRLL");
    if (row2 != 102 || column2 != 4)
    {
        std::cerr << "BBFFBBFRLL failed: row = " << row2 << ", column == " << column2 << "\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "argc: " << argc << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        std::cerr << "Usage: " << argv[0] << " test" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "test") == 0)
    {
        test();
        return 0;
    }

    std::vector<std::string> boarding_passes;
    boarding_passes.reserve(100u);

    auto input_file = std::string(argv[1]);

    size_t maximum_seat_id = 0u;

    parse_file(input_file, boarding_passes);
    std::cout << "Got " << boarding_passes.size() << " boarding passes...\n";

    std::vector<size_t> seat_ids;
    seat_ids.reserve(boarding_passes.size());


    for (const auto s : boarding_passes)
    {
        auto [row, column] = get_row_column(s);

        const auto curr_seat_id = seat_id(row, column);

        seat_ids.push_back(curr_seat_id);

        maximum_seat_id = std::max(maximum_seat_id, curr_seat_id);
    }

    std::cout << "Part 1: Maximum seat id is " << maximum_seat_id << "\n";

    std::sort(std::begin(seat_ids), std::end(seat_ids));

    size_t last_id = seat_ids[0];
    for (size_t i = 1u; i < seat_ids.size(); ++i)
    {
        if (seat_ids[i] != (last_id + 1u))
        {
            std::cout << "Part 2: My seat id is " << last_id + 1 << "\n";
            break;
        }
        last_id = seat_ids[i];
    }

    return 0;
}
