fn part1(schematics: &Vec<(Vec<i32>, bool)>) {
    let answer = schematics
        .iter()
        .map(|(lock, is_lock)| {
            schematics
                .iter()
                .filter(|(key, is_not_key)| {
                    !!is_lock
                        && !is_not_key
                        && lock.iter().zip(key.iter()).all(|(&x, &y)| x + y <= 5)
                })
                .count()
        })
        .sum::<usize>();
    println!("{answer}");
}

fn main() {
    let schematics: Vec<(Vec<i32>, bool)> = std::fs::read_to_string("day-25-puzzle-input.txt")
        .unwrap()
        .replace("\r", "")
        .split("\n\n")
        .map(|s| {
            let rows: Vec<&str> = s.lines().collect();
            let length = rows[0].len();
            let is_lock = rows[0].chars().all(|s| s == '#');
            let heights = (0..length)
                .map(|n| {
                    (rows
                        .iter()
                        .filter(|r| r.chars().nth(n) == Some('#'))
                        .count() as i32)
                        - 1
                })
                .collect();
            (heights, is_lock)
        })
        .collect();
    part1(&schematics);
}
