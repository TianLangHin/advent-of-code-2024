#include<cmath>
#include<fstream>
#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<utility>
#include<vector>

typedef long long longint;
typedef std::map<std::pair<longint, int>, longint> memo_keeper;

int digits(longint x) {
    int d = x == 0 ? 1 : 0;
    while (x) {
        x /= 10;
        ++d;
    }
    return d;
}

longint depth_first_search(memo_keeper& memo, int steps, longint stone_value, int blink_step) {
    if (blink_step == steps) {
        return 1;
    }
    if (memo.count({ stone_value, blink_step })) {
        return memo[{ stone_value, blink_step }];
    } else {
        int d = digits(stone_value);
        longint stones_created;
        if (stone_value == 0) {
            stones_created = depth_first_search(memo, steps, 1, blink_step + 1);
        } else if (d % 2 == 0) {
            longint denominator = 1, power = d / 2;
            while (power--) {
                denominator *= 10;
            }
            longint fst_num = stone_value / denominator;
            longint snd_num = stone_value % denominator;
            stones_created = depth_first_search(memo, steps, fst_num, blink_step + 1)
                + depth_first_search(memo, steps, snd_num, blink_step + 1);
        } else {
            stones_created = depth_first_search(memo, steps, stone_value * 2024, blink_step + 1);
        }
        memo.insert({ { stone_value, blink_step }, stones_created });
        return stones_created;
    }
}

void part1(const std::vector<longint>& stones) {
    memo_keeper memo;
    longint answer = 0;
    for (const auto& stone : stones) {
        answer += depth_first_search(memo, 25, stone, 0);
    }
    std::cout << answer << "\n";
}

void part2(const std::vector<longint>& stones) {
    memo_keeper memo;
    longint answer = 0;
    for (const auto& stone : stones) {
        answer += depth_first_search(memo, 75, stone, 0);
    }
    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-11-puzzle-input.txt");
    std::stringstream ss;
    std::string line;
    std::vector<longint> stones;
    if (puzzle_input.is_open()) {
        std::getline(puzzle_input, line);
        ss << line << ' ';
        std::getline(ss, line, ' ');
        while (!line.empty()) {
            stones.push_back(std::stoll(line));
            std::getline(ss, line, ' ');
        }
    }
    part1(stones);
    part2(stones);
}
