#include<fstream>
#include<iostream>
#include<string>
#include<tuple>
#include<vector>
#include<utility>

void part1(std::vector<std::pair<int, bool>>& files_or_spaces) {

    int final_length = 0;
    std::vector<int> all_interval_sizes;
    std::vector<std::pair<int, int>> files_forward, files_reverse;

    int i = 0;
    for (auto pair : files_or_spaces) {
        int num = pair.first;
        bool is_file = pair.second;
        if (is_file && num != 0) {
            files_forward.insert(files_forward.begin(), { i, num });
            files_reverse.push_back({ i, num });
            final_length += num;
            ++i;
        }
        all_interval_sizes.push_back(num);
    }

    long long checksum = 0;
    bool on_file = true;
    int itvl = 0;
    int interval_idx = all_interval_sizes[itvl];

    for (int i = 0; i < final_length; ++i) {
        while (interval_idx == 0) {
            interval_idx = all_interval_sizes[++itvl];
            on_file = !on_file;
        }
        auto& files = on_file ? files_forward : files_reverse;
        if (!files.empty()) {
            auto num_and_size = files.back();
            files.pop_back();
            int num = num_and_size.first;
            int size = num_and_size.second;
            if (size > 1) {
                files.push_back({ num, size - 1 });
            }
            checksum += i * num;
        }
        --interval_idx;
    }

    std::cout << checksum << "\n";
}

void part2(std::vector<std::pair<int, bool>>& files_or_spaces) {
    std::vector<std::tuple<int, int, int>> files;
    std::vector<std::pair<int, int>> spaces;
    int idx = 0;
    int id = 0;
    for (auto block : files_or_spaces) {
        int size = block.first;
        bool is_file = block.second;
        if (is_file) {
            files.push_back({ id, idx, size });
            idx += size;
            ++id;
        } else {
            spaces.push_back({ idx, size });
            idx += size;
        }
    }
    for (int i = files.size() - 1; i >= 0; --i) {
        int file_id = std::get<0>(files[i]);
        int file_size = std::get<2>(files[i]);
        int free_space = -1;
        for (int j = 0; j < spaces.size(); ++j) {
            if (spaces[j].second >= file_size) {
                free_space = j;
                break;
            }
        }
        if (free_space != -1) {
            if (spaces[free_space].first < std::get<1>(files[i])) {
                int space_idx = spaces[free_space].first;
                int space_size = spaces[free_space].second;
                files[i] = { file_id, space_idx, file_size };
                spaces[free_space] = { space_idx + file_size, space_size - file_size };
            }
        }
    }

    long long checksum = 0;
    for (auto file : files) {
        int file_id = std::get<0>(file);
        int file_idx = std::get<1>(file);
        int file_size = std::get<2>(file);
        for (int i = file_idx; i < file_idx + file_size; ++i) {
            checksum += file_id * i;
        }
    }

    std::cout << checksum << "\n";
}

int main() {
    std::fstream puzzle_input("day-9-puzzle-input.txt");
    std::string line;

    int i = 0;
    std::vector<std::pair<int, bool>> files_or_spaces;
    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                for (char& c : line) {
                    int num = c - '0';
                    files_or_spaces.push_back({ num, i++ % 2 == 0 });
                }
            }
        }
    }

    part1(files_or_spaces);
    part2(files_or_spaces);
}
