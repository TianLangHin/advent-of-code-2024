fn part1(schematics: &Vec<(Vec<i32>, bool)>) {
    let mut answer = 0i64;
    for (lock, is_lock) in schematics {
        if !is_lock {
            continue;
        }
        for (key, is_not_key) in schematics {
            if *is_not_key {
                continue;
            }
            if lock.iter().zip(key.iter()).all(|(&x, &y)| x + y <= 5) {
                answer += 1;
            }
        }
    }
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
                .map(|n| (rows.iter().filter(|r| r.chars().nth(n) == Some('#')).count() as i32) - 1)
                .collect();
            (heights, is_lock)
        })
        .collect();
    part1(&schematics);
}
