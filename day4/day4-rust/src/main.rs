use regex::Regex;

fn part1(grid: &Vec<Vec<char>>) {
    let xmas_regex = Regex::new("XMAS").unwrap();
    let smax_regex = Regex::new("SAMX").unwrap();

    let rows = grid.len();
    let cols = grid[0].len();

    // Horizontal collection
    let horizonal_matches = grid
        .iter()
        .map(|line_chars| {
            let line: String = line_chars.into_iter().collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let vertical_matches = (0..cols)
        .map(|index| {
            let line: String = (0..rows).map(|row| grid[row][index]).collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let major_diag_upper_matches = (0..cols)
        .map(|index| {
            let line: String = (0..)
                .take_while(|i| i + index < rows && i + index < cols)
                .map(|i| grid[i][i + index])
                .collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let major_diag_lower_matches = (1..rows)
        .map(|index| {
            let line: String = (0..)
                .take_while(|i| i + index < rows && i + index < cols)
                .map(|i| grid[i + index][i])
                .collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let reversed_grid: Vec<Vec<char>> = grid
        .iter()
        .map(|row| row.iter().rev().cloned().collect::<Vec<_>>())
        .collect();

    let minor_diag_upper_matches = (0..cols)
        .map(|index| {
            let line: String = (0..)
                .take_while(|i| i + index < rows && i + index < cols)
                .map(|i| reversed_grid[i][i + index])
                .collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let minor_diag_lower_matches = (1..rows)
        .map(|index| {
            let line: String = (0..)
                .take_while(|i| i + index < rows && i + index < cols)
                .map(|i| reversed_grid[i + index][i])
                .collect();
            xmas_regex.captures_iter(&line).count() + smax_regex.captures_iter(&line).count()
        })
        .sum::<usize>();

    let answer = horizonal_matches
        + vertical_matches
        + major_diag_upper_matches
        + major_diag_lower_matches
        + minor_diag_upper_matches
        + minor_diag_lower_matches;

    println!("{answer}");
}

fn part2(grid: &Vec<Vec<char>>) {
    let rows = grid.len();
    let cols = grid[0].len();
    let answer = (1..rows-1)
        .map(|row| {
            (1..cols-1)
                .filter(|&col| {
                    let is_a = grid[row][col] == 'A';
                    let major = (grid[row-1][col-1] == 'M' && grid[row+1][col+1] == 'S') ||
                        (grid[row-1][col-1] == 'S' && grid[row+1][col+1] == 'M');
                    let minor = (grid[row-1][col+1] == 'M' && grid[row+1][col-1] == 'S') ||
                        (grid[row-1][col+1] == 'S' && grid[row+1][col-1] == 'M');

                    is_a && major && minor
                })
                .count()
        })
        .sum::<usize>();
    println!("{answer}");
}

fn main() {
    let puzzle_input: Vec<Vec<char>> = std::fs::read_to_string("day-4-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| line.chars().collect())
        .collect();
    part1(&puzzle_input);
    part2(&puzzle_input);
}
