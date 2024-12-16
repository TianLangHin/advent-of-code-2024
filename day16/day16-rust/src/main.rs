use std::collections::{BinaryHeap, BTreeMap, BTreeSet};

type Point = (i64, i64);

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
enum Direction {
    Left, Right, Up, Down,
}

fn part1(start: Point, end: Point, free_spaces: &BTreeSet<Point>) {
    let mut lowest_score: BTreeMap<(Point, Direction), i64> = BTreeMap::from([
        ((start, Direction::Right), 0),
    ]);
    let mut frontier = BinaryHeap::from([(0i64, start, Direction::Right)]);
    while let Some((negative_score, point, direction)) = frontier.pop() {
        if point == end {
            println!("{}", -negative_score);
            break;
        }
        let neighbours = [
            ((point.0 - 1, point.1), Direction::Left, Direction::Right),
            ((point.0 + 1, point.1), Direction::Right, Direction::Left),
            ((point.0, point.1 - 1), Direction::Up, Direction::Down),
            ((point.0, point.1 + 1), Direction::Down, Direction::Up),
        ];
        for (cell, move_direction, inverse_direction) in neighbours {
            if free_spaces.contains(&cell) && direction != inverse_direction {
                let added_score = if direction == move_direction {
                    1
                } else {
                    1001
                };
                let new_score = -negative_score + added_score;
                if let Some(&recorded_score) = lowest_score.get(&(cell, move_direction)) {
                    if recorded_score > new_score {
                        lowest_score.insert((cell, move_direction), new_score);
                        frontier.push((-new_score, cell, move_direction));
                    }
                } else {
                    lowest_score.insert((cell, move_direction), new_score);
                    frontier.push((-new_score, cell, move_direction));
                }
            }
        }
    }
}

fn main() {
    let mut start = (0i64, 0i64);
    let mut end = (0i64, 0i64);
    let mut free_spaces: BTreeSet<(i64, i64)> = BTreeSet::new();
    let _ = std::fs::read_to_string("day-16-puzzle-input.txt")
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
}
