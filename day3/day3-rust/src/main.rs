use regex::Regex;

fn part1(puzzle_input: &str) {
    let pattern = Regex::new(r"mul\((\d+)\,(\d+)\)").unwrap();
    let answer = pattern
        .captures_iter(puzzle_input)
        .map(|capture| capture[1].parse::<i64>().unwrap() * capture[2].parse::<i64>().unwrap())
        .sum::<i64>();
    println!("{answer}");
}

fn part2(puzzle_input: &str) {
    let pattern = Regex::new(r"mul\(\d+\,\d+\)|do\(\)|don't\(\)").unwrap();
    let mul = Regex::new(r"mul\((\d+)\,(\d+)\)").unwrap();

    let mut enabled = true;
    let mut sum = 0;
    for capture in pattern.captures_iter(puzzle_input) {
        if &capture[0] == "do()" {
            enabled = true;
            continue;
        } else if &capture[0] == "don't()" {
            enabled = false;
            continue;
        }
        if enabled {
            let nums = mul.captures(&capture[0]).unwrap();
            sum += nums[1].parse::<i64>().unwrap() * nums[2].parse::<i64>().unwrap();
        }
    }
    println!("{sum}");
}

fn main() {
    let puzzle_input: String = std::fs::read_to_string("day-3-puzzle-input.txt")
        .unwrap()
        .lines()
        .collect::<Vec<_>>()
        .join("");
    part1(&puzzle_input);
    part2(&puzzle_input);
}
