#include<array>
#include<fstream>
#include<iostream>
#include<map>
#include<string>
#include<vector>

void part1(const std::vector<int>& secrets) {
    long long int total = 0;
    for (const auto& secret : secrets) {
        int num = secret;
        const int mask = (1 << 24) - 1;
        int repetitions = 2000;
    while (repetitions--) {
            num = ((num << 6) ^ num) & mask;
            num = ((num >> 5) ^ num) & mask;
            num = ((num << 11) ^ num) & mask;
        }
        total += num;
    }
    std::cout << total << "\n";
}

void part2(const std::vector<int>& secrets) {
    std::vector<std::vector<int>> buyers;
    for (const auto& secret : secrets) {
        std::vector<int> prices { secret % 10 };
        int num = secret;
        const int mask = (1 << 24) - 1;
        int repetitions = 2000;
        while (repetitions--) {
            num = ((num << 6) ^ num) & mask;
            num = ((num >> 5) ^ num) & mask;
            num = ((num << 11) ^ num) & mask;
            prices.push_back(num % 10);
        }
        buyers.push_back(prices);
    }

    std::vector<std::map<std::array<int, 4>, int>> changes;
    for (int i = 0; i < buyers.size(); ++i) {
        std::map<std::array<int, 4>, int> new_map;
        changes.push_back(new_map);
        for (int j = 4; j < buyers[i].size(); ++j) {
            std::array<int, 4> differences {
                buyers[i][j-3] - buyers[i][j-4],
                buyers[i][j-2] - buyers[i][j-3],
                buyers[i][j-1] - buyers[i][j-2],
                buyers[i][j] - buyers[i][j-1]
            };
            int price = buyers[i][j];

            if (!changes[i].count(differences)) {
                changes[i][differences] = price;
            }
        }
    }

    int most_bananas = 0;

    for (int i = 0; i < changes.size(); ++i) {
        std::vector<std::array<int, 4>> keys;
        for (auto it = changes[i].begin(); it != changes[i].end(); ++it) {
            keys.push_back(it->first);
        }
        for (const auto& key : keys) {
            int bananas = 0;
            for (int j = 0; j < changes.size(); ++j) {
                if (changes[j].count(key)) {
                    bananas += changes[j][key];
                }
            }
            for (int j = 0; j < changes.size(); ++j) {
                changes[j].erase(key);
            }
            if (bananas > most_bananas) {
                most_bananas = bananas;
            }
        }
    }

    std::cout << most_bananas << "\n";
}

int main() {
    std::fstream puzzle_input("day-22-puzzle-input.txt");
    std::string line;
    std::vector<int> secrets;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                secrets.push_back(std::stoi(line));
            }
        }
    }

    part1(secrets);
    part2(secrets);
}
