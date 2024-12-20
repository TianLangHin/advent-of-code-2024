use std::collections::{BTreeMap, BTreeSet, VecDeque};

fn find_best_path(
    start: (i64, i64),
    end: (i64, i64),
    free_spaces: &BTreeSet<(i64, i64)>
) -> Vec<(i64, i64)> {
    let mut frontier = VecDeque::new();
    frontier.push_back((start, Vec::from([start])));

    while let Some(state) = frontier.pop_front() {
        let (point, mut path) = state;

        if point == end {
            return path;
        }

        let neighbours = [
            (point.0 - 1, point.1),
            (point.0 + 1, point.1),
            (point.0, point.1 - 1),
            (point.0, point.1 + 1),
        ];

        for next_point in neighbours {
            if free_spaces.contains(&next_point) && !path.contains(&next_point) {
                path.push(next_point);
                frontier.push_back((next_point, path.clone()));
                path.pop();
            }
        }
    }

    panic!("No possible path");
}

fn part1(start: (i64, i64), end: (i64, i64), free_spaces: &BTreeSet<(i64, i64)>) {

    let steps: BTreeMap<(i64, i64), i64> = find_best_path(start, end, free_spaces)
        .iter()
        .enumerate()
        .map(|(i, &p)| (p, i as i64))
        .collect();

    let mut cheats = 0;

    for &p in steps.keys() {
        let neighbours = [
            (p.0, p.1 - 2),
            (p.0 - 1, p.1 - 1), (p.0 + 1, p.1 - 1),
            (p.0 - 2, p.1), (p.0 + 2, p.1),
            (p.0 - 1, p.1 + 1), (p.0 + 1, p.1 + 1),
            (p.0, p.1 + 2),
        ];
        for n in neighbours {
            if n > p {
                if let (Some(&step1), Some(&step2)) = (steps.get(&p), steps.get(&n)) {
                    if (step2 - step1).abs() >= 102 {
                        cheats += 1;
                    }
                }
            }
        }
    }

    println!("{cheats}");
}

fn manhattan_distance(x: (i64, i64), y: (i64, i64)) -> i64 {
    (x.0 - y.0).abs() + (x.1 - y.1).abs()
}

fn part2(start: (i64, i64), end: (i64, i64), free_spaces: &BTreeSet<(i64, i64)>) {

    let steps: BTreeMap<(i64, i64), i64> = find_best_path(start, end, free_spaces)
        .iter()
        .enumerate()
        .map(|(i, &p)| (p, i as i64))
        .collect();

    let mut cheats = 0;

    for (&cheat_start, &step_start) in &steps {
        for (&cheat_end, &step_end) in &steps {
            let distance_cheat = manhattan_distance(cheat_start, cheat_end);
            if step_start < step_end && distance_cheat <= 20 {
                let distance_diverted = step_end - step_start;
                if distance_diverted - distance_cheat >= 100 {
                    cheats += 1;
                }
            }
        }
    }

    println!("{cheats}");
}

fn main() {
    let mut start = (0i64, 0i64);
    let mut end = (0i64, 0i64);
    let mut free_spaces: BTreeSet<(i64, i64)> = BTreeSet::new();
    let _ = std::fs::read_to_string("day-20-puzzle-input.txt")
        .unwrap()
        .lines()
        .enumerate()
        .for_each(|(y, line)| {
            for (x, spot) in line.chars().enumerate() {
                match spot {
                    'S' => {
                        free_spaces.insert((x as i64, y as i64));
                        start = (x as i64, y as i64);
                    }
                    'E' => {
                        free_spaces.insert((x as i64, y as i64));
                        end = (x as i64, y as i64);
                    }
                    '.' => {
                        free_spaces.insert((x as i64, y as i64));
                    }
                    _ => {}
                }
            }
        });
    part1(start, end, &free_spaces);
    part2(start, end, &free_spaces);
}
