fn part1(puzzle_input: &Vec<Vec<i64>>) {
    let answer = puzzle_input
        .iter()
        .filter(|&report| {
            let pairs: Vec<_> = report.iter().cloned().zip(report.iter().cloned().skip(1)).collect();
            let differences = pairs.iter().all(|&(x, y)| (x-y).abs() >= 1 && (x-y).abs() <= 3);
            let increasing = pairs.iter().all(|&(x, y)| x < y);
            let decreasing = pairs.iter().all(|&(x, y)| x > y);
            differences && (increasing || decreasing)
        })
        .count();
    println!("{answer}");
}

fn part2(puzzle_input: &Vec<Vec<i64>>) {
    let answer = puzzle_input
        .iter()
        .filter(|&report| {
            let pairs: Vec<_> = report.iter().cloned().zip(report.iter().cloned().skip(1)).collect();
            let differences = pairs.iter().all(|&(x, y)| (x-y).abs() >= 1 && (x-y).abs() <= 3);
            let increasing = pairs.iter().all(|&(x, y)| x < y);
            let decreasing = pairs.iter().all(|&(x, y)| x > y);

            let safe_initially = differences && (increasing || decreasing);

            if safe_initially {
                true
            } else {
                (0..report.len()).any(|i| {
                    let removed: Vec<_> = report.iter().cloned().take(i).chain(report.iter().cloned().skip(i + 1)).collect();
                    let pairs: Vec<_> = removed.iter().cloned().zip(removed.iter().cloned().skip(1)).collect();
                    let differences = pairs.iter().all(|&(x, y)| (x-y).abs() >= 1 && (x-y).abs() <= 3);
                    let increasing = pairs.iter().all(|&(x, y)| x < y);
                    let decreasing = pairs.iter().all(|&(x, y)| x > y);

                    differences && (increasing || decreasing)
                })
            }
        })
        .count();
    println!("{answer}");
}

fn main() {
    let puzzle_input: Vec<Vec<i64>> = std::fs::read_to_string("day-2-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            line.split_whitespace().map(|x| x.parse::<i64>().unwrap()).collect()
        })
        .collect();
    part1(&puzzle_input);
    part2(&puzzle_input);
}
