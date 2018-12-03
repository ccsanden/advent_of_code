#include <iostream>
#include <array>

constexpr size_t SIDE_LENGTH = 1000u;

using Fabric = std::array<uint8_t, SIDE_LENGTH * SIDE_LENGTH>;

void two_ints_from_delimited_string(const std::string& s, const char delimeter, size_t& a, size_t& b)
{
    const auto del_pos = s.find(delimeter);
    a = std::stoi(s.substr(0u, del_pos));
    b = std::stoi(s.substr(del_pos + 1u));
}

struct Origin
{
    explicit Origin(const std::string& o)
    {
        two_ints_from_delimited_string(o, ',', this->column, this->row);
    }
    size_t row;
    size_t column;
};

struct Dimensions
{
    explicit Dimensions(const std::string& d)
    {
        two_ints_from_delimited_string(d, 'x', this->columns, this->rows);
    }
    size_t rows;
    size_t columns;
};

void incr(Fabric& f, const Origin& origin, const Dimensions& dimensions)
{
    for (size_t row = origin.row; row < (origin.row + dimensions.rows); ++row) {
        for (size_t col = origin.column; col < (origin.column + dimensions.columns); ++col) {
            f[row * SIDE_LENGTH + col] += 1u;
        }
    }
}

int main(int, char*[])
{
    Fabric f;
    f.fill(0u);

    for (std::string line; std::getline(std::cin, line);) {
        const auto at_pos = line.find('@');
        const auto colon_pos = line.find(':', at_pos);

        const Origin origin{line.substr(at_pos + 2u, colon_pos - (at_pos + 2u))};
        const Dimensions dims{line.substr(colon_pos + 2u)};

        incr(f, origin, dims);

    }

    size_t inches_conflict = 0u;
    for (auto i : f) {
        if (i > 1) {
            inches_conflict++;
        }
    }

    std::cout << "Result: " << inches_conflict << "\n";
    return 0;
}
