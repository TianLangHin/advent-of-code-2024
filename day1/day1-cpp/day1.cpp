#include<algorithm>
#include<fstream>
#include<iostream>
#include<map>
#include<sstream>
#include<vector>

std::pair<int, int> make_pair(std::string line) {
  std::stringstream ss;
  ss << line;
  std::string fst, snd;
  std::getline(ss, fst, ' ');
  std::getline(ss, snd);
  return std::pair<int, int>(std::stoi(fst), std::stoi(snd));
}

void part1(std::vector<std::pair<int, int>>& numbers) {
  std::vector<int> first, second;
  for (auto pair : numbers) {
    first.push_back(pair.first);
    second.push_back(pair.second);
  }
  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());

  int sum = 0;
  for (
    auto it_fst = first.begin(), it_snd = second.begin();
    it_fst != first.end() && it_snd != second.end();
    it_fst++, it_snd++) {

    int diff = *it_fst - *it_snd;
    int abs_diff = diff < 0 ? -diff : diff;

    sum += abs_diff;
  }

  std::cout << sum << "\n";
}

void part2(std::vector<std::pair<int, int>>& numbers) {
  std::vector<int> first, second;
  for (auto pair : numbers) {
    first.push_back(pair.first);
    second.push_back(pair.second);
  }
  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());

  std::map<int, int> snd_counter;
  for (auto item : second) {
    snd_counter.count(item) ? ++snd_counter[item] : snd_counter[item] = 1;
  }

  int sum = 0;
  for (auto x : first) {
    sum += x * (snd_counter.count(x) ? snd_counter[x] : 0);
  }

  std::cout << sum << "\n";
}

int main() {
  std::ifstream puzzle_input("day-1-puzzle-input.txt");
  std::string line;

  std::vector<std::pair<int, int>> numbers;

  if (puzzle_input.is_open()) {
    while (puzzle_input.good()) {
      std::getline(puzzle_input, line);
      if (!line.empty()) {
        numbers.push_back(make_pair(line));
      }
    }
  }
  part1(numbers);
  part2(numbers);
}
