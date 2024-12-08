use std::{
    collections::{HashMap, HashSet},
};

fn part1(antennae: &HashMap<char, Vec<(i64, i64)>>, rows: i64, cols: i64) {
    let mut antinodes: HashSet<(i64, i64)> = HashSet::new();
    for frequency in antennae.keys() {
        let locations = &antennae.get(&frequency).unwrap();
        for i in 0..locations.len() {
            for j in i+1..locations.len() {
                // Here, we know that `locations[i].0 <= locations[j].0 &&
                // locations[i].1 < locations[j].1`.
                let x_distance = locations[j].0 - locations[i].0;
                let y_distance = (locations[j].1 - locations[i].1).abs();
                if x_distance % 3 == 0 && y_distance % 3 == 0 {
                    let point_near_i = (
                        locations[i].0 + x_distance / 3,
                        if locations[i].1 < locations[j].1 {
                            locations[i].1 + y_distance / 3
                        } else {
                            locations[i].1 - y_distance / 3
                        },
                    );
                    let point_near_j = (
                        locations[j].0 - x_distance / 3,
                        if locations[j].1 < locations[i].1 {
                            locations[j].1 + y_distance / 3
                        } else {
                            locations[j].1 - y_distance / 3
                        },
                    );
                    antinodes.insert(point_near_i);
                    antinodes.insert(point_near_j);
                }
                let beyond_i = (
                    locations[i].0 - (locations[j].0 - locations[i].0),
                    locations[i].1 - (locations[j].1 - locations[i].1),
                );
                let beyond_j = (
                    locations[j].0 - (locations[i].0 - locations[j].0),
                    locations[j].1 - (locations[i].1 - locations[j].1),
                );
                if 0 <= beyond_i.0 && beyond_i.0 < rows &&
                    0 <= beyond_i.1 && beyond_i.1 < cols {
                    antinodes.insert(beyond_i);
                }
                if 0 <= beyond_j.0 && beyond_j.0 < rows &&
                    0 <= beyond_j.1 && beyond_j.1 < cols {
                    antinodes.insert(beyond_j);
                }
            }
        }
    }
    println!("{}", antinodes.len());
}

fn gcd(mut a: i64, mut b: i64) -> i64 {
    let mut t: i64;
    while b != 0 {
        t = b;
        b = a % b;
        a = t;
    }
    a
}

fn part2(antennae: &HashMap<char, Vec<(i64, i64)>>, rows: i64, cols: i64) {
    let mut antinodes: HashSet<(i64, i64)> = HashSet::new();
    for frequency in antennae.keys() {
        let locations = &antennae.get(&frequency).unwrap();
        for i in 0..locations.len() {
            for j in i+1..locations.len() {
                let x_distance = locations[j].0 - locations[i].0;
                let y_distance = (locations[j].1 - locations[i].1).abs();
                let y_direction = if locations[i].1 < locations[j].1 { 1 } else { -1 };
                let divisor = gcd(x_distance, y_distance);

                let mut point = locations[i];
                while 0 <= point.0 && point.0 < rows &&
                    0 <= point.1 && point.1 < cols {
                    antinodes.insert(point);
                    point = (
                        point.0 + x_distance / divisor,
                        point.1 + y_direction * y_distance / divisor,
                    );
                }
                point = locations[i];
                while 0 <= point.0 && point.0 < rows &&
                    0 <= point.1 && point.1 < cols {
                    antinodes.insert(point);
                    point = (
                        point.0 - x_distance / divisor,
                        point.1 - y_direction * y_distance / divisor,
                    );
                }
            }
        }
    }
    println!("{}", antinodes.len());
}

fn main() {
    let mut antennae: HashMap<char, Vec<(i64, i64)>> = HashMap::new();
    let (rows, cols) = std::fs::read_to_string("day-8-puzzle-input.txt")
        .unwrap()
        .lines()
        .enumerate()
        .map(|(i, line)| {
            for (j, c) in line.chars().enumerate() {
                if c != '.' {
                    let point = (i as i64, j as i64);
                    antennae
                        .entry(c)
                        .and_modify(|vector| { vector.push(point); })
                        .or_insert(Vec::from([point]));
                }
            }
            (i, line.len() as i64)
        })
        .fold((0, 0), |acc, x| { (acc.0 + 1, x.1) });
    part1(&antennae, rows, cols);
    part2(&antennae, rows, cols);
}
