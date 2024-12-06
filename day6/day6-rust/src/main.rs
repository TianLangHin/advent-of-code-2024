use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
enum Direction {
    Up, Right, Down, Left,
}

impl Direction {
    fn next(&self) -> Self {
        match *self {
            Self::Up => Self::Right,
            Self::Right => Self::Down,
            Self::Down => Self::Left,
            Self::Left => Self::Up,
        }
    }
}

fn make_maps(obstacles: &Vec<(usize, usize)>) -> (
    BTreeMap<usize, BTreeSet<usize>>,
    BTreeMap<usize, BTreeSet<usize>>,
) {
    let mut row_map: BTreeMap<usize, BTreeSet<usize>> = BTreeMap::new();
    let mut col_map: BTreeMap<usize, BTreeSet<usize>> = BTreeMap::new();
    for &(row, col) in obstacles {
        row_map
            .entry(row)
            .and_modify(|value| { value.insert(col); })
            .or_insert(BTreeSet::from([col]));
        col_map
            .entry(col)
            .and_modify(|value| { value.insert(row); })
            .or_insert(BTreeSet::from([row]));
    }
    (row_map, col_map)
}

fn make_move<const TRACKING: bool>(
    obstacle: usize,
    direction: Direction,
    visited: &mut BTreeSet<(usize, usize)>,
    pos: &mut (usize, usize)
) {
    match direction {
        Direction::Up => {
            if TRACKING {
                visited.extend((obstacle + 1 ..= pos.0).map(|i| (i, pos.1)));
            }
            *pos = (obstacle + 1, pos.1);
        }
        Direction::Right => {
            if TRACKING {
                visited.extend((pos.1 .. obstacle).map(|i| (pos.0, i)));
            }
            *pos = (pos.0, obstacle - 1);
        }
        Direction::Down => {
            if TRACKING {
                visited.extend((pos.0 .. obstacle).map(|i| (i, pos.1)));
            }
            *pos = (obstacle - 1, pos.1);
        }
        Direction::Left => {
            if TRACKING {
                visited.extend((obstacle + 1 ..= pos.1).map(|i| (pos.0, i)));
            }
            *pos = (pos.0, obstacle + 1);
        }
    }
}

fn get_next_obstacle(
    direction: Direction,
    pos: (usize, usize),
    row_map: &BTreeMap<usize, BTreeSet<usize>>,
    col_map: &BTreeMap<usize, BTreeSet<usize>>,
) -> Option<usize> {
    match direction {
        Direction::Up => {
            if let Some(set) = col_map.get(&pos.1) {
                set.iter().rev().find(|&&x| x < pos.0).cloned()
            } else {
                None
            }
        }
        Direction::Right => {
            if let Some(set) = row_map.get(&pos.0) {
                set.iter().find(|&&x| x > pos.1).cloned()
            } else {
                None
            }
        }
        Direction::Down => {
            if let Some(set) = col_map.get(&pos.1) {
                set.iter().find(|&&x| x > pos.0).cloned()
            } else {
                None
            }
        }
        Direction::Left => {
            if let Some(set) = row_map.get(&pos.0) {
                set.iter().rev().find(|&&x| x < pos.1).cloned()
            } else {
                None
            }
        }
    }
}

fn part1(
    obstacles: &Vec<(usize, usize)>,
    start: (usize, usize),
    rows: usize,
    cols: usize
) -> BTreeSet<(usize, usize)> {

    let (row_map, col_map) = make_maps(obstacles);

    let mut pos = start;
    let mut visited: BTreeSet<(usize, usize)> = BTreeSet::from([start]);
    let mut direction = Direction::Up;

    let mut next_obstacle = get_next_obstacle(direction, pos, &row_map, &col_map);

    while let Some(obstacle) = next_obstacle {
        make_move::<true>(obstacle, direction, &mut visited, &mut pos);
        direction = direction.next();
        next_obstacle = get_next_obstacle(direction, pos, &row_map, &col_map);
    }

    match direction {
        Direction::Up => {
            visited.extend((0 .. pos.0).map(|i| (i, pos.1)));
        }
        Direction::Right => {
            visited.extend((pos.1 .. cols).map(|i| (pos.0, i)));
        }
        Direction::Down => {
            visited.extend((pos.0 .. rows).map(|i| (i, pos.1)));
        }
        Direction::Left => {
            visited.extend((0 .. pos.1).map(|i| (pos.0, i)));
        }
    }

    println!("{}", visited.len());

    visited.remove(&start);

    visited
}

fn part2(
    obstacles: &Vec<(usize, usize)>,
    start: (usize, usize),
    possible_placements: &BTreeSet<(usize, usize)>)
{
    let mut sum = 0;

    // Never used, only for convenience with earlier helper function
    let mut visited: BTreeSet<(usize, usize)> = BTreeSet::new();

    let (mut row_map, mut col_map) = make_maps(obstacles);

    for &(x, y) in possible_placements {

        row_map
            .entry(x)
            .and_modify(|set| { set.insert(y); })
            .or_insert(BTreeSet::from([y]));

        col_map
            .entry(y)
            .and_modify(|set| { set.insert(x); })
            .or_insert(BTreeSet::from([x]));

        let mut overlapped = false;

        let mut pos = start;
        let mut direction = Direction::Up;
        let mut orientations: BTreeSet<(usize, usize, Direction)> =
            BTreeSet::from([(start.0, start.1, Direction::Up)]);

        let mut next_obstacle = get_next_obstacle(direction, pos, &row_map, &col_map);

        while let Some(obstacle) = next_obstacle {
            make_move::<false>(obstacle, direction, &mut visited, &mut pos);
            direction = direction.next();
            if orientations.contains(&(pos.0, pos.1, direction)) {
                overlapped = true;
                break;
            }
            orientations.insert((pos.0, pos.1, direction));
            next_obstacle = get_next_obstacle(direction, pos, &row_map, &col_map);
        }

        if overlapped {
            sum += 1;
        }

        row_map
            .entry(x)
            .and_modify(|set| { set.remove(&y); });
        col_map
            .entry(y)
            .and_modify(|set| { set.remove(&x); });
    }

    println!("{sum}");
}

fn main() {
    let mut cols = 0;
    let mut start = (0, 0);
    let file_string = std::fs::read_to_string("day-6-puzzle-input.txt").unwrap();
    let rows = file_string.lines().count();
    let obstacles: Vec<(usize, usize)> = file_string
        .lines()
        .enumerate()
        .flat_map(|(row_index, line)| {
            cols = line.len();
            let guard = line.chars().position(|c| c == '^');
            if let Some(s) = guard {
                start = (row_index, s);
            }
            line
                .chars()
                .enumerate()
                .filter_map(move |(i, c)| {
                    if c == '#' {
                        Some((row_index, i))
                    } else {
                        None
                    }
                })
        })
        .collect();
    let path = part1(&obstacles, start, rows, cols);
    part2(&obstacles, start, &path);
}
