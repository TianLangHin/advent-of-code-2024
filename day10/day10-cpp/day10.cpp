#include<algorithm>
#include<deque>
#include<fstream>
#include<iostream>
#include<set>
#include<tuple>
#include<utility>
#include<vector>

void part1(const std::vector<std::vector<int>>& landscape) {
    int answer = 0;
    int rows = landscape.size();
    int cols = landscape[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            if (landscape[i][j] == 0) {
                std::set<std::pair<int, int>> found_nines;
                std::deque<std::tuple<int, int, int>> frontier { { i, j, 0 } };
                while (!frontier.empty()) {
                    auto state = frontier.front();
                    frontier.pop_front();
                    int row = std::get<0>(state);
                    int col = std::get<1>(state);
                    int num = std::get<2>(state);
                    if (num == 9) {
                        found_nines.insert({ row, col });
                    } else {
                        std::pair<int, int> neighbours[] = {
                            { std::max(row-1, 0), col },
                            { std::min(row+1, rows-1), col },
                            { row, std::max(col-1, 0) },
                            { row, std::min(col+1, cols-1) }
                        };
                        for (const auto& coord : neighbours) {
                            if (landscape[coord.first][coord.second] == num + 1) {
                                frontier.push_back({ coord.first, coord.second, num + 1 });
                            }
                        }
                    }
                }
                answer += found_nines.size();
            }
        }
    }
    std::cout << answer << "\n";
}

void part2(std::vector<std::vector<int>>& landscape) {
    std::vector<std::vector<int>> reachable;
    for (const auto& row : landscape) {
        std::vector<int> reachable_row;
        for (const int& n : row) {
            reachable_row.push_back(n == 9 ? 1 : 0);
        }
        reachable.push_back(reachable_row);
    }
    int rows = landscape.size();
    int cols = landscape[0].size();
    for (int number = 8; number >= 0; --number) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (landscape[i][j] == number) {
                    std::pair<int, int> neighbours[] = {
                        { std::max(i-1, 0), j },
                        { std::min(i+1, rows-1), j },
                        { i, std::max(j-1, 0) },
                        { i, std::min(j+1, cols-1) }
                    };
                    for (const auto& coord : neighbours) {
                        if (landscape[coord.first][coord.second] == number + 1) {
                            reachable[i][j] += reachable[coord.first][coord.second];
                        }
                    }
                }
            }
        }
    }
    int answer = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (landscape[i][j] == 0) {
                answer += reachable[i][j];
            }
        }
    }
    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-10-puzzle-input.txt");
    std::string line;

    std::vector<std::vector<int>> landscape;
    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                std::vector<int> row;
                for (char& c : line) {
                    row.push_back(c - '0');
                }
                landscape.push_back(row);
            }
        }
    }

    part1(landscape);
    part2(landscape);
}
