#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>

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

    const size_t num_digits = strlen(argv[1]);

    std::vector<uint8_t> target_sequence;
    for (int i = 0; i < num_digits; ++i) {
        target_sequence.push_back(argv[1][i] - 48u);
    }

    std::cout << "Target sequence: ";
    for (auto c : target_sequence) {
        std::cout << static_cast<uint16_t>(c);
    }
    std::cout << "\n";

    std::vector<uint8_t> current_state = {3u, 7u};
    current_state.reserve(num_digits);

    Elves e;

    bool found = false;
    uint32_t last_tried_index = 0u;

    while(!found) {
        const uint8_t score_a = current_state[e._a];
        const uint8_t score_b = current_state[e._b];

        const auto new_recipes = sum_to_parts(score_a + score_b);
        current_state.insert(std::end(current_state), new_recipes.begin(), new_recipes.end());

        if ((current_state.size() - (last_tried_index)) >= num_digits) {
            last_tried_index += 1u;

            for (uint32_t i = 0u; i < num_digits; ++i) {
                if (target_sequence[i] != current_state[last_tried_index + i]) {
                    found = false;
                    break;
                } else {
                    found = true;
                }
            }
        }

        e._a = walk_elf(e._a, score_a, current_state);
        e._b = walk_elf(e._b, score_b, current_state);
    }

    std::cout << "Solution (input=" << argv[1] << "): " << last_tried_index << "\n";

    return 0;
}

