#include<fstream>
#include<iostream>
#include<map>
#include<numeric>
#include<sstream>
#include<string>
#include<vector>

enum LogicOp {
    AND, OR, XOR
};

typedef std::tuple<std::string, std::string, LogicOp> gate_link_t;

void part1(std::map<std::string, bool>& gates, const std::map<gate_link_t, std::string>& links) {
    std::vector<std::string> final_digits;
    for (auto& item : links) {
        if (item.second[0] == 'z') {
            final_digits.push_back(item.second);
        }
    }
    while (std::accumulate(
        final_digits.begin(),
        final_digits.end(),
        false,
        [&](bool acc, const std::string& z) {
            return acc || gates.count(z) == 0;
        })
    ) {
        for (const auto& item : links) {
            std::string g1 = std::get<0>(item.first);
            std::string g2 = std::get<1>(item.first);
            if (gates.count(g1) && gates.count(g2)) {
                bool g1_val = gates[g1];
                bool g2_val = gates[g2];
                LogicOp op = std::get<2>(item.first);
                switch (op) {
                    case AND:
                        gates[item.second] = g1_val && g2_val;
                        break;
                    case OR:
                        gates[item.second] = g1_val || g2_val;
                        break;
                    case XOR:
                        gates[item.second] = g1_val != g2_val;
                        break;
                }
            }
        }
    }
    long long int answer = 0;
    for (auto& d : final_digits) {
        if (gates[d]) {
            answer |= 1LL << std::stoll(d.substr(1, d.size() - 1));
        }
    }
    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-24-puzzle-input.txt");
    std::string line;

    std::map<std::string, bool> initial;
    std::map<gate_link_t, std::string> links;

    bool section = true;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (section) {
                if (line.empty()) {
                    section = false;
                } else {
                    std::stringstream ss;
                    ss << line << ':';

                    std::string component;
                    std::getline(ss, component, ':');
                    std::string gate_name = component;

                    std::getline(ss, component, ' ');
                    std::getline(ss, component, ':');
                    bool gate_value = component == "1";

                    initial.insert({ gate_name, gate_value });
                }
            } else {
                if (line.empty()) {
                    break;
                }
                std::stringstream ss;
                ss << line << ' ';

                std::vector<std::string> components;
                std::string component;
                std::getline(ss, component, ' ');
                while (!component.empty()) {
                    components.push_back(component);
                    std::getline(ss, component, ' ');
                }

                LogicOp gate = components[1] == "AND" ? AND :
                    components[1] == "OR" ? OR : XOR;

                links.insert({
                    std::tuple { components[0], components[2], gate },
                    components[4]
                });
            }
        }
    }

    part1(initial, links);
}
