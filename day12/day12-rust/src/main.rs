use std::{
    cmp::{max, min},
    collections::{BTreeMap, BTreeSet, VecDeque},
};

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
enum Direction {
    Up, Down, Left, Right,
}

fn up(p: (i32, i32)) -> (i32, i32) { (p.0 - 1, p.1) }
fn down(p: (i32, i32)) -> (i32, i32) { (p.0 + 1, p.1) }
fn left(p: (i32, i32)) -> (i32, i32) { (p.0, p.1 - 1) }
fn right(p: (i32, i32)) -> (i32, i32) { (p.0, p.1 + 1) }

const NEIGHBOURS: [(fn((i32, i32)) -> (i32, i32), Direction); 4] = [
    (up, Direction::Up),
    (down, Direction::Down),
    (left, Direction::Left),
    (right, Direction::Right),
];

const ADJACENTS: [(fn((i32, i32)) -> (i32, i32), Direction); 4] = [
    (right, Direction::Up),
    (right, Direction::Down),
    (down, Direction::Left),
    (down, Direction::Right),
];

fn part1(farm: &Vec<Vec<char>>) {
    let mut plant_coord_map: BTreeMap<char, VecDeque<(i32, i32)>> = BTreeMap::new();
    for i in 0..farm.len() {
        for j in 0..farm[0].len() {
            plant_coord_map
                .entry(farm[i][j])
                .and_modify(|vec_deque| vec_deque.push_back((i as i32, j as i32)))
                .or_insert(VecDeque::from([(i as i32, j as i32)]));
        }
    }
    let mut total_price = 0;
    let rows = farm.len() as i32;
    let cols = farm[0].len() as i32;
    for (plant_type, mut coords) in plant_coord_map {
        while let Some(point) = coords.pop_front() {

            let mut area = 1;
            let mut outer_squares: BTreeMap<(i32, i32), i32> = NEIGHBOURS
                .iter()
                .map(|(func, _)| (func(point), 1))
                .collect();

            let mut visited: BTreeSet<(i32, i32)> = BTreeSet::from([point]);
            let mut frontier: VecDeque<(i32, i32)> = VecDeque::from([point]);
            while let Some(point) = frontier.pop_front() {
                let next_points = [
                    (max(0, point.0 - 1), point.1),
                    (min(rows - 1, point.0 + 1), point.1),
                    (point.0, max(0, point.1 - 1)),
                    (point.0, min(cols - 1, point.1 + 1)),
                ];
                for next_point in next_points {
                    if !visited.contains(&next_point) 
                        && farm[next_point.0 as usize][next_point.1 as usize] == plant_type {

                        for (surrounding, _) in NEIGHBOURS {
                            outer_squares
                                .entry(surrounding(next_point))
                                .and_modify(|count| *count += 1)
                                .or_insert(1);
                        }

                        coords.retain(|&p| p != next_point);

                        area += 1;
                        visited.insert(next_point);
                        frontier.push_back(next_point);
                    }
                }
            }
            outer_squares.retain(|point, _| !visited.contains(point));
            let perimeter: i32 = outer_squares.values().sum();
            total_price += area * perimeter;
        }
    }
    println!("{total_price}");
}

fn part2(farm: &Vec<Vec<char>>) {
    let mut plant_coord_map: BTreeMap<char, VecDeque<(i32, i32)>> = BTreeMap::new();
    for i in 0..farm.len() {
        for j in 0..farm[0].len() {
            plant_coord_map
                .entry(farm[i][j])
                .and_modify(|vec_deque| vec_deque.push_back((i as i32, j as i32)))
                .or_insert(VecDeque::from([(i as i32, j as i32)]));
        }
    }
    let mut total_price = 0;
    let rows = farm.len() as i32;
    let cols = farm[0].len() as i32;
    for (plant_type, mut coords) in plant_coord_map {
        while let Some(point) = coords.pop_front() {

            let mut area = 1;
            let mut outer_squares: BTreeSet<((i32, i32), Direction)> = NEIGHBOURS
                .iter()
                .map(|(func, dir)| (func(point), *dir))
                .collect();

            let mut visited: BTreeSet<(i32, i32)> = BTreeSet::from([point]);
            let mut frontier: VecDeque<(i32, i32)> = VecDeque::from([point]);
            while let Some(point) = frontier.pop_front() {
                let next_points = [
                    (max(0, point.0 - 1), point.1),
                    (min(rows - 1, point.0 + 1), point.1),
                    (point.0, max(0, point.1 - 1)),
                    (point.0, min(cols - 1, point.1 + 1)),
                ];
                for next_point in next_points {
                    if !visited.contains(&next_point) 
                        && farm[next_point.0 as usize][next_point.1 as usize] == plant_type {

                        for (surrounding, dir) in NEIGHBOURS {
                            outer_squares.insert((surrounding(next_point), dir));
                        }

                        coords.retain(|&p| p != next_point);

                        area += 1;
                        visited.insert(next_point);
                        frontier.push_back(next_point);
                    }
                }
            }
            outer_squares.retain(|(point, _)| !visited.contains(point));

            let mut number_of_sides = 0;
            while !outer_squares.is_empty() {
                let (sample_point, direction) = *outer_squares.iter().next().unwrap();

                let (func, _) = ADJACENTS.iter().find(|&(_, d)| *d == direction).unwrap();

                outer_squares.remove(&(sample_point, direction));
                let mut p = sample_point;
                while outer_squares.contains(&(func(p), direction)) {
                    p = func(p);
                    outer_squares.remove(&(p, direction));
                }

                number_of_sides += 1;
            }

            total_price += area * number_of_sides;
        }
    }
    println!("{total_price}");
}

fn main() {
    let farm: Vec<Vec<char>> = std::fs::read_to_string("day-12-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            line.chars().collect()
        })
        .collect();
    part1(&farm);
    part2(&farm);
}
