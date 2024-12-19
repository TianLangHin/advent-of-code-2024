fn part1(towels: &Vec<String>, designs: &Vec<String>) {
    let answer: i32 = designs
        .iter()
        .map(|design| {
            let design_vec: Vec<char> = design.chars().collect();
            let mut solvable: Vec<bool> = vec![false; design.len()];
            for i in 0..design.len() {
                for towel in towels {
                    if i + 1 >= towel.len() {
                        let start_index = i + 1 - towel.len();
                        let matches_suffix = design_vec[start_index..i + 1]
                            .iter()
                            .zip(towel.chars())
                            .all(|(&d, t)| d == t);
                        let previous_solutions = start_index == 0 || solvable[start_index];
                        if matches_suffix && previous_solutions {
                            solvable[i] = true;
                            break;
                        }
                    }
                }
            }
            if *solvable.last().unwrap() {
                1
            } else {
                0
            }
        })
        .sum();
    println!("{answer}");
}

fn part2(towels: &Vec<String>, designs: &Vec<String>) {
    let answer: i64 = designs
        .iter()
        .map(|design| {
            let design_vec: Vec<char> = design.chars().collect();
            let mut solvable: Vec<i64> = vec![0; design.len()];
            for i in 0..design.len() {
                for towel in towels {
                    if i + 1 >= towel.len() {
                        let start_index = i + 1 - towel.len();
                        let matches_suffix = design_vec[start_index..i + 1]
                            .iter()
                            .zip(towel.chars())
                            .all(|(&d, t)| d == t);
                        if matches_suffix {
                            let previous_solutions = if start_index == 0 {
                                1
                            } else {
                                solvable[start_index - 1]
                            };
                            solvable[i] += previous_solutions;
                        }
                    }
                }
            }
            *solvable.last().unwrap()
        })
        .sum();
    println!("{answer}");
}

fn main() {
    let (towels, designs) = std::fs::read_to_string("day-19-puzzle-input.txt")
        .unwrap()
        .lines()
        .fold((Vec::<String>::new(), Vec::<String>::new()), |acc, line| {
            let (mut towels, mut designs) = acc;
            if towels.is_empty() {
                towels = line.split(", ").map(|s| s.to_string()).collect();
            } else if !line.is_empty() {
                designs.push(line.to_string());
            }
            (towels, designs)
        });
    part1(&towels, &designs);
    part2(&towels, &designs);
}
