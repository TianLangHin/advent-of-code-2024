#include<algorithm>
#include<array>
#include<deque>
#include<fstream>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<utility>
#include<vector>

typedef std::array<char, 2> computer_t;

void part1(std::map<computer_t, std::set<computer_t>>& graph) {
    std::set<std::vector<computer_t>> found_cliques;

    for (const auto& entry : graph) {
        computer_t computer = entry.first;
        std::set<computer_t> linked = entry.second;

        if (computer[0] == 't') {
            std::set<std::pair<computer_t, computer_t>> cliques;

            for (const computer_t& c : linked) {
                if (graph.count(c)) {
                    for (const computer_t& e : graph[c]) {
                        cliques.insert(std::pair { c, e });
                    }
                }
            }

            for (const auto& clique : cliques) {
                if (linked.count(clique.second)) {
                    std::vector<computer_t> vec_clique {
                        computer,
                        clique.first,
                        clique.second,
                    };
                    std::sort(vec_clique.begin(), vec_clique.end());
                    found_cliques.insert(vec_clique);
                }
            }
        }
    }

    std::cout << found_cliques.size() << "\n";
}

void part2(std::map<computer_t, std::set<computer_t>>& graph) {
    std::deque<computer_t> keys;
    for (auto& item : graph) {
        keys.push_back(item.first);
    }
    std::set<computer_t> biggest_clique_found;

    while (keys.size() > biggest_clique_found.size()) {
        computer_t starting_computer = keys.front();
        keys.pop_front();
        std::set<computer_t> clique;
        clique.insert(starting_computer);

        for (const computer_t& key : keys) {
            if (graph.count(key)) {
                std::vector<computer_t> intersection;
                std::set_intersection(
                    graph[key].begin(), graph[key].end(),
                    clique.begin(), clique.end(),
                    std::back_inserter(intersection)
                );
                if (intersection.size() == clique.size()) {
                    clique.insert(key);
                }
            }
        }

        auto it = keys.begin();
        while (it != keys.end()) {
            if (clique.count(*it)) {
                it = keys.erase(it);
            } else {
                ++it;
            };
        }

        if (clique.size() > biggest_clique_found.size()) {
            biggest_clique_found = clique;
        }
    }

    std::vector<computer_t> computers;
    for (const auto& item : biggest_clique_found) {
        computers.push_back(item);
    }
    std::sort(computers.begin(), computers.end());

    std::cout << computers[0][0] << computers[0][1];
    for (auto it = std::next(computers.begin()); it != computers.end(); ++it) {
        std::cout << "," << (*it)[0] << (*it)[1];
    }
    std::cout << "\n";
}

int main() {
    std::fstream puzzle_input("day-23-puzzle-input.txt");
    std::string line;

    std::map<computer_t, std::set<computer_t>> graph;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                computer_t c1 { line[0], line[1] };
                computer_t c2 { line[3], line[4] };
                if (!graph.count(c1)) {
                    graph[c1] = std::set { c2 };
                } else {
                    graph[c1].insert(c2);
                }
                if (!graph.count(c2)) {
                    graph[c2] = std::set { c1 };
                } else {
                    graph[c2].insert(c1);
                }
            }
        }
    }

    part1(graph);
    part2(graph);
}
