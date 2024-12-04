#include<algorithm>
#include<fstream>
#include<iostream>
#include<regex>
#include<vector>

void part1(std::vector<std::vector<char>> grid) {
    std::regex xmas("XMAS"), samx("SAMX");
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::string> all_lines;
    int sum = 0;

    for (auto line_chars : grid) {
        std::string line(line_chars.begin(), line_chars.end());
        all_lines.push_back(line);
    }

    for (int index = 0; index < cols; ++index) {
        std::string line = "";
        for (int row = 0; row < rows; ++row)
            line += grid[row][index];
        all_lines.push_back(line);
    }

    for (int index = 0; index < cols; ++index) {
        std::string line = "";
        for (int i = 0; i + index < rows && i + index < cols; ++i)
            line += grid[i][i + index];
        all_lines.push_back(line);
    }

    for (int index = 1; index < rows; ++index) {
        std::string line = "";
        for (int i = 0; i + index < rows && i + index < cols; ++i) {
            line += grid[i + index][i];
        }
        all_lines.push_back(line);
    }

    std::reverse(grid.begin(), grid.end());

    for (int index = 0; index < cols; ++index) {
        std::string line = "";
        for (int i = 0; i + index < rows && i + index < cols; ++i) {
            line += grid[i][i + index];
        }
        all_lines.push_back(line);
    }

    for (int index = 1; index < rows; ++index) {
        std::string line = "";
        for (int i = 0; i + index < rows && i + index < cols; ++i) {
            line += grid[i + index][i];
        }
        all_lines.push_back(line);
    }

    for (auto line : all_lines) {
        int xmas_matches = std::distance(
            std::sregex_iterator(line.begin(), line.end(), xmas),
            std::sregex_iterator()
        );
        int samx_matches = std::distance(
            std::sregex_iterator(line.begin(), line.end(), samx),
            std::sregex_iterator()
        );
        sum += xmas_matches + samx_matches;
    }

    std::cout << sum << "\n";
}

void part2(std::vector<std::vector<char>> grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int sum = 0;
    for (int row = 1; row < rows - 1; ++row) {
        for (int col = 1; col < cols - 1; ++col) {
            bool is_a = grid[row][col] == 'A';
            bool major = (grid[row-1][col-1] == 'M' && grid[row+1][col+1] == 'S') ||
                (grid[row-1][col-1] == 'S' && grid[row+1][col+1] == 'M');
            bool minor = (grid[row-1][col+1] == 'M' && grid[row+1][col-1] == 'S') ||
                (grid[row-1][col+1] == 'S' && grid[row+1][col-1] == 'M');
            if (is_a && major && minor)
                ++sum;
        }
    }

    std::cout << sum << "\n";
}

int main() {
    std::ifstream puzzle_input("day-4-puzzle-input.txt");
    std::string line;
    std::vector<std::vector<char>> grid;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                grid.push_back(std::vector<char>(line.begin(), line.end()));
            }
        }
    }

    part1(grid);
    part2(grid);
}
