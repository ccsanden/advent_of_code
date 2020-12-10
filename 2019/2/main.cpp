#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

namespace
{
    void tokenize(const std::string& program, /* out */std::vector<uint32_t>& tokens)
    {
        std::string now = "";
        for (const auto c : program) {
            if (c == ',') {
                tokens.push_back(std::stoi(now));
                now = "";
            } else {
                now.push_back(c);
            }
        }
        if (now.length() > 0u) {
            tokens.push_back(std::stoi(now));
        }
    }

    std::vector<uint32_t> run_program(std::vector<uint32_t> tokens)
    {
        static constexpr uint32_t OP_SIZE = 4u;
        static constexpr uint32_t ADD = 1u;
        static constexpr uint32_t MULT = 2u;
        static constexpr uint32_t END = 99u;

        static constexpr uint32_t OPERAND1 = 1u;
        static constexpr uint32_t OPERAND2 = OPERAND1 + 1u;
        static constexpr uint32_t DESTINATION = OPERAND2 + 1u;

        size_t current_op = 0u;

        while (tokens[current_op] != END) {
            if (tokens[current_op] == ADD) {
                const auto operand1 = tokens[tokens[current_op + OPERAND1]];
                const auto operand2 = tokens[tokens[current_op + OPERAND2]];
                tokens[tokens[current_op + DESTINATION]] = operand1 + operand2;

            } else if (tokens[current_op] == MULT) {
                const auto operand1 = tokens[tokens[current_op + OPERAND1]];
                const auto operand2 = tokens[tokens[current_op + OPERAND2]];
                tokens[tokens[current_op + DESTINATION]] = operand1 * operand2;
            } else {
                // Syntax error
                std::cout << "Syntax error token " << current_op << ": " << tokens[current_op] << "\n";
                break;
            }
            current_op += OP_SIZE;
        }
        return tokens;
    }

    int test()
    {
        auto t = [] (const std::string program, const std::vector<uint32_t> expected)  -> bool {
            std::vector<uint32_t> tokens;
            tokenize(program, tokens);
            tokens = run_program(tokens);
            if (tokens.size() != expected.size()) {
                std::cerr << "Incorrect size of output\n";
                std::cerr << "Expected: ";
                for (auto t : expected) {
                    std::cerr << t << ",";
                }
                std::cerr << "\n";
                std::cerr << "Actual: ";
                for (auto t : tokens) {
                    std::cerr << t << ",";
                }
                std::cerr << "\n";
                return false;
            }

            for (size_t i = 0u; i < tokens.size(); ++i) {
                if (tokens[i] != expected[i]) {
                    std::cerr << "Error on token " << i << "\n";
                    return false;
                }
            }
            return true;
        };

        if (!t("1,0,0,0,99", {2u, 0u, 0u, 0u, 99u})) {
            return 1;
        }

        if (!t("2,3,0,3,99", {2, 3, 0, 6, 99})) {
            return 2;
        }

        if (!t("2,4,4,5,99,0", {2u, 4u, 4u, 5u, 99u, 9801u})) {
            return 3;
        }

        if (!t("1,1,1,4,99,5,6,0,99", {30, 1, 1, 4, 2, 5, 6, 0, 99})) {
            return 4;
        }
        if (!t("1,9,10,3,2,3,11,0,99,30,40,50", {3500u, 9u, 10u, 70u, 2u, 3u, 11u, 0u, 99u, 30u, 40u,50u})) {
            return 5;
        }
        return 0;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    if (strcmp(argv[1], "test") == 0) {
        return test();
    }

    std::ifstream indata{argv[1]};
    std::string program;
    std::getline(indata, program);

    std::vector<uint32_t> orig_state;
    orig_state.reserve(5000u);
    tokenize(program, orig_state);

    { // Part 1
        auto tmp = orig_state;
        tmp[1u] = 12u;
        tmp[2u] = 2u;
        std::cout << "Part 1: " << run_program(tmp)[0u] << "\n";
    }

    // Part2 (brute force)
    static constexpr uint32_t goal = 19690720u;

    for(uint32_t i = 0u; i <= 99u; i++ ) {
        for(uint32_t j = 0u; j <= 99u; j++ ) {
            auto tmp = orig_state;
            tmp[1u] = i;
            tmp[2u] = j;
            if (run_program(tmp)[0] == goal) {
                std::cout << "Part 2: " << 100 * i + j << std::endl;
                return 0;
            }
        }
    }


    return 0;
}
