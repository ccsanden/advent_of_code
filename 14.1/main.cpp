#include <iostream>
#include <vector>
#include <sstream>
#include <string>

struct Elves
{
#if 0
    uint8_t _a = 4u;
    uint8_t _b = 8u;
#endif
    uint32_t _a = 0u;
    uint32_t _b = 1u;
};

std::vector<uint8_t> sum_to_parts(const uint8_t sum)
{
    if (sum < 10u) {
        return {sum};
    }
    return {1u, static_cast<uint8_t>((sum - 10u))};
}

uint32_t walk_elf(uint32_t current_index, uint8_t current_score, const std::vector<uint8_t>& recipes)
{
    const uint32_t num_recipes = recipes.size();
    const uint32_t walk_distance = current_score + 1;
    const uint32_t steps = walk_distance % (num_recipes);
    uint32_t new_index = current_index;
    if (current_index + steps >= num_recipes) {
        new_index =  steps - (num_recipes - current_index);
    } else {
        new_index = current_index + steps;
    }

    return new_index;
}

int main(int argc, char* argv[])
{

    if (argc != 2) {
        std::cerr << "Requires number of recipes\n";
        return 1;
    }

    int tmp = std::stoi(argv[1]);
    if (tmp < 0) {
        std::cerr << "Invalid argument: " << argv[1] << "\n";
        return 2;
    }
    const uint32_t num_recipes = static_cast<uint32_t>(tmp);
    const uint32_t must_generate_this_many = num_recipes + 10u;

    //std::vector<uint32_t> current_state = {3u, 7u, 1u, 0u, 1u, 0u, 1u, 2u, 4u, 5u, 1u, 5u, 8u, 9u, 1u, 6u, 7u, 7u, 9u, 2u};
    std::vector<uint8_t> current_state = {3u, 7u};
    current_state.reserve(must_generate_this_many);

    Elves e;

    std::cout << "Recipes:";
    for (const auto c : current_state) {
        std::cout << " " << static_cast<uint16_t>(c);
    }

    std::cout << "  Elf A @ " << e._a << ", Elf B @ " << e._b;
    std::cout << "\n";

    while(current_state.size() < must_generate_this_many) {
        const uint8_t score_a = current_state[e._a];
        const uint8_t score_b = current_state[e._b];

        const auto new_recipes = sum_to_parts(score_a + score_b);
        current_state.insert(std::end(current_state), new_recipes.begin(), new_recipes.end());

        e._a = walk_elf(e._a, score_a, current_state);
        e._b = walk_elf(e._b, score_b, current_state);
    }

    std::ostringstream ostr;
    for(size_t i = 0; i < 10; ++i) {
        ostr << static_cast<uint16_t>(current_state[num_recipes + i]);
    }

    std::cout << "Solution (input=" << num_recipes << "): " << ostr.str() << "\n";

    return 0;
}

