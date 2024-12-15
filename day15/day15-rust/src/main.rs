use std::collections::{BTreeSet, VecDeque};

type Point = (i64, i64);
type Points = BTreeSet<Point>;

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum Direction {
    Left, Right, Up, Down,
}

fn part1(mut robot: Point, walls: &Points, boxes: &Points, steps: &Vec<Direction>) {
    let mut boxes = boxes.clone();

    for step in steps {

        let (mut x, mut y) = robot;
        match step {
            Direction::Left => { x -= 1; }
            Direction::Right => { x += 1; }
            Direction::Up => { y -= 1; }
            Direction::Down => { y += 1; }
        };
        let mut found_boxes: Vec<Point> = Vec::new();
        let mut wall_hit = walls.contains(&(x, y));
        let mut box_hit = boxes.contains(&(x, y));
        while wall_hit || box_hit {
            if wall_hit {
                break;
            } else if box_hit {
                found_boxes.push((x, y));
            }
            match step {
                Direction::Left => { x -= 1; }
                Direction::Right => { x += 1; }
                Direction::Up => { y -= 1; }
                Direction::Down => { y += 1; }
            };
            wall_hit = walls.contains(&(x, y));
            box_hit = boxes.contains(&(x, y));
        }
        if !wall_hit {
            for b in &found_boxes {
                boxes.remove(b);
            }
            match step {
                Direction::Left => {
                    for &b in &found_boxes {
                        boxes.insert((b.0 - 1, b.1));
                    }
                    robot = (robot.0 - 1, robot.1);
                }
                Direction::Right => {
                    for &b in &found_boxes {
                        boxes.insert((b.0 + 1, b.1));
                    }
                    robot = (robot.0 + 1, robot.1);
                }
                Direction::Up => {
                    for &b in &found_boxes {
                        boxes.insert((b.0, b.1 - 1));
                    }
                    robot = (robot.0, robot.1 - 1);
                }
                Direction::Down => {
                    for &b in &found_boxes {
                        boxes.insert((b.0, b.1 + 1));
                    }
                    robot = (robot.0, robot.1 + 1);
                }
            }
        }
    }

    let answer = boxes.iter().fold(0i64, |acc, (x, y)| acc + 100 * y + x);

    println!("{answer:?}");
}

fn part2(mut robot: Point, walls: &Points, boxes: &Points, steps: &Vec<Direction>) {
    let walls: Points = walls
        .iter()
        .map(|point| [(point.0 * 2, point.1), (point.0 * 2 + 1, point.1)])
        .flatten()
        .collect();
    let mut boxes: Points = boxes
        .iter()
        .map(|point| (point.0 * 2, point.1))
        .collect();

    robot = (robot.0 * 2, robot.1);
    for step in steps {
        match step {
            Direction::Left => {
                let (mut x, y) = robot;
                x -= 1;
                let mut found_boxes: Vec<Point> = Vec::new();
                let mut wall_hit = walls.contains(&(x, y));
                let mut box_hit = boxes.contains(&(x-1, y));
                while wall_hit || box_hit {
                    if wall_hit {
                        break;
                    } else if box_hit {
                        found_boxes.push((x-1, y));
                        x -= 2;
                    }
                    wall_hit = walls.contains(&(x, y));
                    box_hit = boxes.contains(&(x-1, y));
                }
                if !wall_hit {
                    for b in &found_boxes {
                        boxes.remove(b);
                    }
                    for &b in &found_boxes {
                        boxes.insert((b.0 - 1, b.1));
                    }
                    robot = (robot.0 - 1, robot.1);
                }
            }
            Direction::Right => {
                let (mut x, y) = robot;
                x += 1;
                let mut found_boxes: Vec<Point> = Vec::new();
                let mut wall_hit = walls.contains(&(x, y));
                let mut box_hit = boxes.contains(&(x, y));
                while wall_hit || box_hit {
                    if wall_hit {
                        break;
                    } else if box_hit {
                        found_boxes.push((x, y));
                        x += 2;
                    }
                    wall_hit = walls.contains(&(x, y));
                    box_hit = boxes.contains(&(x, y));
                }
                if !wall_hit {
                    for b in &found_boxes {
                        boxes.remove(b);
                    }
                    for &b in &found_boxes {
                        boxes.insert((b.0 + 1, b.1));
                    }
                    robot = (robot.0 + 1, robot.1);
                }
            }
            Direction::Up => {
                let mut visited: Vec<Point> = Vec::new();
                let mut frontier: VecDeque<Point> = VecDeque::new();
                if walls.contains(&(robot.0, robot.1 - 1)) {
                    continue;
                }
                if boxes.contains(&(robot.0, robot.1 - 1)) {
                    visited.push((robot.0, robot.1 - 1));
                    frontier.push_back((robot.0, robot.1 - 1));
                }
                if boxes.contains(&(robot.0 - 1, robot.1 - 1)) {
                    visited.push((robot.0 - 1, robot.1 - 1));
                    frontier.push_back((robot.0 - 1, robot.1 - 1));
                }
                let mut wall_hit = false;
                while let Some(b) = frontier.pop_front() {
                    if walls.contains(&(b.0, b.1 - 1)) || walls.contains(&(b.0 + 1, b.1 - 1)) {
                        wall_hit = true;
                        break;
                    }
                    for x_value in b.0 - 1 ..= b.0 + 1 {
                        if boxes.contains(&(x_value, b.1 - 1)) {
                            visited.push((x_value, b.1 - 1));
                            frontier.push_back((x_value, b.1 - 1));
                        }
                    }
                }
                if !wall_hit {
                    for point in &visited {
                        boxes.remove(point);
                    }
                    for &point in &visited {
                        boxes.insert((point.0, point.1 - 1));
                    }
                    robot = (robot.0, robot.1 - 1);
                }
            }
            Direction::Down => {
                let mut visited: Vec<Point> = Vec::new();
                let mut frontier: VecDeque<Point> = VecDeque::new();
                if walls.contains(&(robot.0, robot.1 + 1)) {
                    continue;
                }
                if boxes.contains(&(robot.0, robot.1 + 1)) {
                    visited.push((robot.0, robot.1 + 1));
                    frontier.push_back((robot.0, robot.1 + 1));
                }
                if boxes.contains(&(robot.0 - 1, robot.1 + 1)) {
                    visited.push((robot.0 - 1, robot.1 + 1));
                    frontier.push_back((robot.0 - 1, robot.1 + 1));
                }
                let mut wall_hit = false;
                while let Some(b) = frontier.pop_front() {
                    if walls.contains(&(b.0, b.1 + 1)) || walls.contains(&(b.0 + 1, b.1 + 1)) {
                        wall_hit = true;
                        break;
                    }
                    for x_value in b.0 - 1 ..= b.0 + 1 {
                        if boxes.contains(&(x_value, b.1 + 1)) {
                            visited.push((x_value, b.1 + 1));
                            frontier.push_back((x_value, b.1 + 1));
                        }
                    }
                }
                if !wall_hit {
                    for point in &visited {
                        boxes.remove(point);
                    }
                    for &point in &visited {
                        boxes.insert((point.0, point.1 + 1));
                    }
                    robot = (robot.0, robot.1 + 1);
                }
            }
        }
    }

    let answer = boxes.iter().fold(0i64, |acc, (x, y)| acc + 100 * y + x);

    println!("{answer:?}");
}

fn main() {
    let mut walls: Points = BTreeSet::new();
    let mut boxes: Points = BTreeSet::new();
    let mut robot = (0i64, 0i64);
    let mut steps: Vec<Direction> = Vec::new();

    let mut i = 0i64;
    let mut section = true;
    let _ = std::fs::read_to_string("day-15-puzzle-input.txt")
        .unwrap()
        .lines()
        .for_each(|line| {
            if section {
                if line.is_empty() {
                    section = false;
                } else {
                    for (j, c) in line.chars().enumerate() {
                        match c {
                            '#' => { walls.insert((j as i64, i)); }
                            'O' => { boxes.insert((j as i64, i)); }
                            '@' => { robot = (j as i64, i); }
                            _ => {}
                        }
                    }
                    i += 1;
                }
            } else {
                for c in line.chars() {
                    match c {
                        '^' => { steps.push(Direction::Up); }
                        '>' => { steps.push(Direction::Right); }
                        'v' => { steps.push(Direction::Down); }
                        '<' => { steps.push(Direction::Left); }
                        _ => {}
                    }
                }
            }
        });
    // println!("{walls:?}");
    // println!("{boxes:?}");
    // println!("{steps:?}");
    // println!("{robot:?}");
    part1(robot, &walls, &boxes, &steps);
    part2(robot, &walls, &boxes, &steps);
}
