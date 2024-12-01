#include<algorithm>
#include<fstream>
#include<tuple>
#include<vector>

int main() {
    std::ifstream puzzle_input("day-1-puzzle-input.txt");
    std::string line;
    std::string fst_num;
    std::string snd_num;

    std::vector<std::tuple<int, int>> numbers;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            std::getline(line, fst_num, ' ');
            std::getline(line, snd_num, ' ');
            numbers.push_back(std::tuple<int, int>(std::stoi(fst_num), std::stoi(snd_num)));
        }
    }
}
