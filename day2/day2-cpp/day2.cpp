#include<cmath>
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>

std::vector<int> make_report(std::string line) {
    std::vector<int> report_line;

    std::stringstream ss;
    ss << line << " ";
    std::string element;

    std::getline(ss, element, ' ');
    while (!element.empty()) {
        report_line.push_back(std::stoi(element));
        std::getline(ss, element, ' ');
    }

    return report_line;
}

void part1(std::vector<std::vector<int>> reports) {
    int sum = 0;
    for (auto report : reports) {
        bool differences = true, increasing = true, decreasing = true;
        for (int i = 0; i < report.size() - 1; ++i) {
            int diff = std::abs(report[i] - report[i+1]);
            differences &= diff >= 1 && diff <= 3;
            increasing &= report[i] < report[i+1];
            decreasing &= report[i] > report[i+1];
        }
        if (differences && (increasing || decreasing)) {
            ++sum;
        }
    }
    std::cout << sum << "\n";
}

void part2(std::vector<std::vector<int>> reports) {
    int sum = 0;
    for (auto report : reports) {
        bool differences = true, increasing = true, decreasing = true;
        for (int i = 0; i < report.size() - 1; ++i) {
            int diff = std::abs(report[i] - report[i+1]);
            differences &= diff >= 1 && diff <= 3;
            increasing &= report[i] < report[i+1];
            decreasing &= report[i] > report[i+1];
        }
        if (differences && (increasing || decreasing)) {
            ++sum;
        } else {
            for (int i = 0; i < report.size(); ++i) {
                differences = true; increasing = true; decreasing = true;
                int j = i == 0 ? 1 : 0;
                int k = i == 0 || i == 1 ? 2 : 1;
                for (; j < report.size() && k < report.size(); ++j == i ? ++j : 0, ++k == i ? ++k : 0) {
                    int diff = std::abs(report[j] - report[k]);
                    differences &= diff >= 1 && diff <= 3;
                    increasing &= report[j] < report[k];
                    decreasing &= report[j] > report[k];
                }
                if (differences && (increasing || decreasing)) {
                    ++sum;
                    break;
                }
            }
        }
    }
    std::cout << sum << "\n";
}

int main() {
    std::ifstream puzzle_input("day-2-puzzle-input.txt");
    std::string line;

    std::vector<std::vector<int>> reports;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                reports.push_back(make_report(line));
            }
        }
    }

    part1(reports);
    part2(reports);
}
