use std::collections::{HashMap, VecDeque};

fn find_path(start: (i32, i32), end: (i32, i32), obstacles: &[(i32, i32)]) -> Option<i32> {
    let mut best_costs = HashMap::new();
    let mut frontier = VecDeque::new();
    frontier.push_back((start, 0));

    while let Some((point, cost)) = frontier.pop_front() {

        if point == end {
            return Some(cost);
        }

        if let Some(&prev_cost) = best_costs.get(&point) {
            if cost >= prev_cost {
                continue;
            }
        }

        best_costs.insert(point, cost);

        let neighbours = [
            (point.0 - 1, point.1),
            (point.0 + 1, point.1),
            (point.0, point.1 - 1),
            (point.0, point.1 + 1),
        ];

        for next_pos in neighbours {
            if 0 <= point.0 && point.0 <= end.0 &&
                0 <= point.1 && point.1 <= end.1 &&
                !obstacles.contains(&next_pos)
            {
                frontier.push_back((next_pos, cost + 1));
            }
        }
    }

    None
}

fn part1(bytes: &Vec<(i32, i32)>) {
    let start = (0, 0);
    let end = (70, 70);

    let number_of_fallen = 1024;
    let fallen = &bytes[0..number_of_fallen];

    println!("{}", find_path(start, end, fallen).unwrap());
}

fn part2(bytes: &Vec<(i32, i32)>) {
    let start = (0, 0);
    let end = (70, 70);

    let (mut low, mut high) = (0, bytes.len());
    while low < high {
        let mid = (low + high) / 2;
        let result = find_path(start, end, &bytes[0..mid]);
        if result.is_some() {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    println!("{},{}", bytes[low - 1].0, bytes[low - 1].1);
}

fn main() {
    let bytes: Vec<(i32, i32)> = std::fs::read_to_string("day-18-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            let coord: Vec<&str> = line.split(',').collect();
            (coord[0].parse::<i32>().unwrap(), coord[1].parse::<i32>().unwrap())
        })
        .collect();
    part1(&bytes);
    part2(&bytes);
}
