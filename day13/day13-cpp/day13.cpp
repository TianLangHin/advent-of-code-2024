#include<fstream>
#include<iostream>
#include<regex>
#include<utility>
#include<vector>

typedef long long int longint;

struct Machine {
    longint x_a, y_a, x_b, y_b, x, y;
};

void part1(const std::vector<Machine>& claw_machines) {
    longint token_cost = 0;

    for (const Machine& m : claw_machines) {
        std::pair<longint, longint> a_button_times = {
            m.x * m.y_b - m.y * m.x_b,
            m.x_a * m.y_b - m.y_a * m.x_b
        };
        std::pair<longint, longint> b_button_times = {
            m.x * m.y_a - m.y * m.x_a,
            m.x_b * m.y_a - m.y_b * m.x_a
        };

        if (a_button_times.first * a_button_times.second < 0 ||
            b_button_times.first * b_button_times.second < 0) {
            continue;
        }

        if (a_button_times.first < 0) {
            a_button_times = { -a_button_times.first, -a_button_times.second };
        }
        if (b_button_times.first < 0) {
            b_button_times = { -b_button_times.first, -b_button_times.second };
        }

        if (a_button_times.first % a_button_times.second != 0 ||
            b_button_times.first % b_button_times.second != 0) {
            continue;
        }

        const longint n_a = a_button_times.first / a_button_times.second;
        const longint n_b = b_button_times.first / b_button_times.second;

        if (n_a <= 100 && n_b <= 100) {
            token_cost += 3 * n_a + n_b;
        }
    }

    std::cout << token_cost << "\n";
}

void part2(const std::vector<Machine>& claw_machines) {
    longint token_cost = 0;

    for (const Machine& m : claw_machines) {

        longint x = 10000000000000 + m.x;
        longint y = 10000000000000 + m.y;

        std::pair<longint, longint> a_button_times = {
            x * m.y_b - y * m.x_b,
            m.x_a * m.y_b - m.y_a * m.x_b
        };
        std::pair<longint, longint> b_button_times = {
            x * m.y_a - y * m.x_a,
            m.x_b * m.y_a - m.y_b * m.x_a
        };

        if (a_button_times.first * a_button_times.second < 0 ||
            b_button_times.first * b_button_times.second < 0) {
            continue;
        }

        if (a_button_times.first < 0) {
            a_button_times = { -a_button_times.first, -a_button_times.second };
        }
        if (b_button_times.first < 0) {
            b_button_times = { -b_button_times.first, -b_button_times.second };
        }

        if (a_button_times.first % a_button_times.second != 0 ||
            b_button_times.first % b_button_times.second != 0) {
            continue;
        }

        const longint n_a = a_button_times.first / a_button_times.second;
        const longint n_b = b_button_times.first / b_button_times.second;

        token_cost += 3 * n_a + n_b;
    }

    std::cout << token_cost << "\n";
}

int main() {
    std::fstream puzzle_input("day-13-puzzle-input.txt");
    std::string line;
    std::string file_string = "";

    std::vector<Machine> claw_machines;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                file_string += line + "\n";
            }
        }
    }

    std::regex pattern(
        "Button A: X\\+(\\d+)[,] Y\\+(\\d+)\n"
        "Button B: X\\+(\\d+)[,] Y\\+(\\d+)\n"
        "Prize: X=(\\d+)[,] Y=(\\d+)"
    );

    auto matches_begin = std::sregex_iterator(file_string.begin(), file_string.end(), pattern);
    auto matches_end = std::sregex_iterator();
    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        Machine m = {
            std::stoll(match[1]), std::stoll(match[2]),
            std::stoll(match[3]), std::stoll(match[4]),
            std::stoll(match[5]), std::stoll(match[6])
        };
        claw_machines.push_back(m);
    }

    part1(claw_machines);
    part2(claw_machines);
}
