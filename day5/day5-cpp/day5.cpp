#include<fstream>
#include<iostream>
#include<map>
#include<set>
#include<sstream>
#include<string>
#include<vector>
#include<utility>

typedef std::vector<std::pair<int, int>> rule_list;
typedef std::vector<std::vector<int>> update_list;

bool has_intersection(std::set<int> fst_set, std::set<int> snd_set) {
    auto fst = fst_set.begin(), snd = snd_set.begin();
    while (fst != fst_set.end() && snd != snd_set.end()) {
        if (*fst == *snd)
            return true;
        *fst < *snd ? ++fst : ++snd;
    }
    return false;
}

void part1(rule_list page_ordering_rules, update_list updates) {
    std::map<int, std::set<int>> page_orders;
    for (auto pair : page_ordering_rules) {
        if (page_orders.count(pair.first)) {
            page_orders[pair.first].insert(pair.second);
        } else {
            page_orders[pair.first] = std::set<int> { pair.second };
        }
    }
    int sum = 0;
    for (auto update : updates) {
        bool correct = true;
        std::set<int> seen { update[0] };
        for (int i = 1; i < update.size(); ++i) {
            if (page_orders.count(update[i])) {
                if (has_intersection(page_orders[update[i]], seen)) {
                    correct = false;
                    break;
                }
            }
            seen.insert(update[i]);
        }
        if (correct)
            sum += update[update.size() / 2];
    }
    std::cout << sum << "\n";
}

void part2(rule_list page_ordering_rules, update_list updates) {
    std::map<int, std::set<int>> page_orders;
    for (auto pair : page_ordering_rules) {
        if (page_orders.count(pair.first)) {
            page_orders[pair.first].insert(pair.second);
        } else {
            page_orders[pair.first] = std::set<int> { pair.second };
        }
    }
    int sum = 0;
    for (auto update : updates) {
        bool swapped = false;
        int index = 0;
        while (index < update.size()) {
            bool violation_found = false;
            int swap_index = 0;
            for (int i = 0; i < index && !violation_found; ++i) {
                if (page_orders.count(update[index]) &&
                    page_orders[update[index]].count(update[i])) {
                    violation_found = true;
                    swap_index = i;
                }
            }
            if (violation_found) {
                swapped = true;
                std::swap(update[index], update[swap_index]);
                index = swap_index + 1;
            } else {
                ++index;
            }
        }
        if (swapped)
            sum += update[update.size() / 2];
    }
    std::cout << sum << "\n";
}

int main() {
    std::fstream puzzle_input("day-5-puzzle-input.txt");
    std::string line;

    rule_list page_ordering_rules;
    update_list updates;
    bool section = true;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (line.empty()) {
                if (section) {
                    section = false;
                } else {
                    break;
                }
            } else if (section) {
                std::stringstream ss;
                ss << line;
                std::string fst, snd;
                std::getline(ss, fst, '|');
                std::getline(ss, snd);
                page_ordering_rules.push_back(
                    std::pair<int, int>(std::stoi(fst), std::stoi(snd))
                );
            } else {
                std::stringstream ss;
                ss << line << ",";
                std::string element;
                std::vector<int> update;

                std::getline(ss, element, ',');
                while (!element.empty()) {
                    update.push_back(std::stoi(element));
                    std::getline(ss, element, ',');
                }
                updates.push_back(update);
            }
        }
    }

    part1(page_ordering_rules, updates);
    part2(page_ordering_rules, updates);
}
