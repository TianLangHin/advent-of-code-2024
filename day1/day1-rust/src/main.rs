use std::collections::HashMap;

fn part1(numbers: &Vec<(i32, i32)>) {
    let mut first: Vec<i32> = numbers.iter().map(|&(x, _)| x).collect();
    let mut second: Vec<i32> = numbers.iter().map(|&(_, x)| x).collect();
    first.sort();
    second.sort();
    println!(
        "{}",
        first
            .iter()
            .zip(second.iter())
            .map(|(&x, &y)| (x - y).abs())
            .sum::<i32>()
    );
}

fn part2(numbers: &Vec<(i32, i32)>) {
    let first: Vec<i32> = numbers.iter().map(|&(x, _)| x).collect();
    let second: Vec<i32> = numbers.iter().map(|&(_, x)| x).collect();
    let mut snd_counter: HashMap<i32, usize> = HashMap::new();
    for item in second {
        snd_counter.insert(item, snd_counter.get(&item).unwrap_or(&0) + 1);
    }
    println!(
        "{}",
        first
            .iter()
            .map(|&x| (x as usize) * snd_counter.get(&x).unwrap_or(&0))
            .sum::<usize>()
    );
}

fn main() {
    let puzzle_input = std::fs::read_to_string("day-1-puzzle-input.txt").unwrap();
    let numbers: Vec<(i32, i32)> = puzzle_input
        .lines()
        .map(|line| {
            let x: Vec<_> = line.split_whitespace().collect();
            (x[0].parse::<i32>().unwrap(), x[1].parse::<i32>().unwrap())
        })
        .collect();
    part1(&numbers);
    part2(&numbers);
}
