#include<fstream>
#include<iostream>
#include<numeric>
#include<sstream>
#include<string>
#include<vector>

int get_combo_operand(int x, int a, int b, int c) {
    switch (x) {
        case 0: case 1: case 2: case 3:
            return x;
        case 4:
            return a;
        case 5:
            return b;
        case 6:
            return c;
    }
    __builtin_unreachable();
}

void part1(int a, int b, int c, const std::vector<int>& program) {

    bool outputted = false;

    int i = 0;
    while (i < program.size()) {
        int fst = program[i];
        int snd = program[i+1];

        int combo_operand;

        switch (fst) {
            case 0:
                combo_operand = get_combo_operand(snd, a, b, c);
                a = a / (1 << combo_operand);
                i += 2;
                break;
            case 1:
                b = b ^ snd;
                i += 2;
                break;
            case 2:
                combo_operand = get_combo_operand(snd, a, b, c);
                b = combo_operand % 8;
                i += 2;
                break;
            case 3:
                i = a == 0 ? i + 2 : snd;
                break;
            case 4:
                b = b ^ c;
                i += 2;
                break;
            case 5:
                combo_operand = get_combo_operand(snd, a, b, c);
                i += 2;
                std::cout << (outputted ? "," : "");
                std::cout << (combo_operand % 8);
                outputted = true;
                break;
            case 6:
                combo_operand = get_combo_operand(snd, a, b, c);
                b = a / (1 << combo_operand);
                i += 2;
                break;
            case 7:
                combo_operand = get_combo_operand(snd, a, b, c);
                c = a / (1 << combo_operand);
                i += 2;
                break;
            default:
                break; 
        }
    }
    std::cout << "\n";
}

int main() {
    std::fstream puzzle_input("day-17-puzzle-input.txt");
    std::string line;

    int a = 0, b = 0, c = 0;
    std::vector<int> program;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                std::stringstream ss;
                std::string value;
                if (line.rfind("Register A: ", 0) == 0) {
                    ss << line;
                    std::getline(ss, value, ' ');
                    std::getline(ss, value, ' ');
                    std::getline(ss, value);
                    a = std::stoi(value);
                } else if (line.rfind("Register B: ", 0) == 0) {
                    ss << line;
                    std::getline(ss, value, ' ');
                    std::getline(ss, value, ' ');
                    std::getline(ss, value);
                    b = std::stoi(value);
                } else if (line.rfind("Register C: ", 0) == 0) {
                    ss << line;
                    std::getline(ss, value, ' ');
                    std::getline(ss, value, ' ');
                    std::getline(ss, value);
                    c = std::stoi(value);
                } else if (line.rfind("Program: ", 0) == 0) {
                    ss << line << ',';
                    std::getline(ss, value, ' ');
                    std::getline(ss, value, ',');
                    while (!value.empty()) {
                        program.push_back(std::stoi(value));
                        std::getline(ss, value, ',');
                    }
                }
            }
        }
    }

    part1(a, b, c, program);
}
