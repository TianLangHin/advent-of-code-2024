use std::{
    collections::{BTreeMap, BTreeSet, VecDeque},
    iter::once,
};

fn part1(codes: &Vec<&str>) {
    let base_keypad: BTreeMap<char, (i32, i32)> = BTreeMap::from([
        ('7', (0, 0)),
        ('8', (1, 0)),
        ('9', (2, 0)),
        ('4', (0, 1)),
        ('5', (1, 1)),
        ('6', (2, 1)),
        ('1', (0, 2)),
        ('2', (1, 2)),
        ('3', (2, 2)),
        ('0', (1, 3)),
        ('A', (2, 3)),
    ]);
    let dir_keypad: BTreeMap<char, (i32, i32)> = BTreeMap::from([
        ('^', (1, 0)),
        ('A', (2, 0)),
        ('<', (0, 1)),
        ('v', (1, 1)),
        ('>', (2, 1)),
    ]);

    let mut answer = 0;

    for code in codes {

        let mut first_robot = VecDeque::from([("".to_string(), 0)]);
        let steps = once('A').chain(code.chars()).zip(code.chars()).enumerate();
        for (i, (start, end)) in steps {
            let (x_start, y_start) = *base_keypad.get(&start).unwrap();
            let (x_end, y_end) = *base_keypad.get(&end).unwrap();
            let x_dist = (x_end - x_start).abs();
            let y_dist = (y_end - y_start).abs();

            let x_key = if x_start < x_end { '>' } else { '<' };
            let y_key = if y_start < y_end { 'v' } else { '^' };
            let x_path = (0..x_dist).map(|_| x_key).collect::<String>();
            let y_path = (0..y_dist).map(|_| y_key).collect::<String>();

            let mut paths = BTreeSet::new();
            if base_keypad.values().any(|&p| p == (x_end, y_start)) {
                paths.insert(x_path.clone() + &y_path);
            }
            if base_keypad.values().any(|&p| p == (x_start, y_end)) {
                paths.insert(y_path.clone() + &x_path);
            }


            while !first_robot.is_empty() {
                let sol = first_robot.front().unwrap();
                if sol.1 > i {
                    break;
                }
                let item = first_robot.pop_front().unwrap().0;
                for path in &paths {
                    first_robot.push_back(
                        (format!("{}{}A", item, path), i + 1)
                    );
                }
            }
        }

        let min_length = first_robot.iter().map(|(s, _)| s.len()).min().unwrap();
        first_robot.retain(|(s, _)| s.len() == min_length);

        let mut robot: Vec<_> = first_robot.into_iter().collect(); 

        for _ in 0..2 {
            let mut next_robot = Vec::new();
            for (code, _) in &robot {
                let mut possible_solutions = VecDeque::from([("".to_string(), 0)]);
                let steps = once('A').chain(code.chars()).zip(code.chars()).enumerate();
                for (i, (start, end)) in steps {
                    let (x_start, y_start) = *dir_keypad.get(&start).unwrap();
                    let (x_end, y_end) = *dir_keypad.get(&end).unwrap();
                    let x_dist = (x_end - x_start).abs();
                    let y_dist = (y_end - y_start).abs();

                    let x_key = if x_start < x_end { '>' } else { '<' };
                    let y_key = if y_start < y_end { 'v' } else { '^' };
                    let x_path = (0..x_dist).map(|_| x_key).collect::<String>();
                    let y_path = (0..y_dist).map(|_| y_key).collect::<String>();

                    let mut paths = BTreeSet::new();
                    if dir_keypad.values().any(|&p| p == (x_end, y_start)) {
                        paths.insert(x_path.clone() + &y_path);
                    }
                    if dir_keypad.values().any(|&p| p == (x_start, y_end)) {
                        paths.insert(y_path.clone() + &x_path);
                    }

                    while !possible_solutions.is_empty() {
                        let sol = possible_solutions.front().unwrap();
                        if sol.1 > i {
                            break;
                        }
                        let item = possible_solutions.pop_front().unwrap().0;
                        for path in &paths {
                            possible_solutions.push_back(
                                (format!("{}{}A", item, path), i + 1)
                            );
                        }
                    }
                }
                next_robot.extend(possible_solutions.into_iter());
            }

            let min_length = next_robot.iter().map(|(s, _)| s.len()).min().unwrap();
            next_robot.retain(|(s, _)| s.len() == min_length);

            robot.clear();
            robot.append(&mut next_robot);
        }

        let min_length = robot.iter().map(|(s, _)| s.len()).min().unwrap();
        let numeric_part: usize = code.chars().take(3).collect::<String>().parse().unwrap();

        answer += min_length * numeric_part;
    }

    println!("{answer}");
}

fn main() {
    let puzzle = std::fs::read_to_string("day-21-puzzle-input.txt").unwrap();
    let codes: Vec<&str> = puzzle.lines().collect();
    part1(&codes);
}
