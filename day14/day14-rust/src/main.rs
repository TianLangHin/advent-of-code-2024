use std::cmp::Ordering;

fn part1(robots: &Vec<((i64, i64), (i64, i64))>) {
    let width = 101;
    let height = 103;

    let mut quadrant = [0, 0, 0, 0];

    for ((px, py), (vx, vy)) in robots {
        let result_x = ((px % width) + 100 * (vx % width)) % width;
        let result_x = if result_x < 0 { result_x + width } else { result_x };

        let result_y = ((py % height) + 100 * (vy % height)) % height;
        let result_y = if result_y < 0 { result_y + height } else { result_y };

        let quadrant_x = match result_x.cmp(&(width / 2)) {
            Ordering::Equal => None,
            Ordering::Less => Some(0),
            Ordering::Greater => Some(2),
        };

        let quadrant_y = match result_y.cmp(&(height / 2)) {
            Ordering::Equal => None,
            Ordering::Less => Some(0),
            Ordering::Greater => Some(1),
        };

        if let (Some(x), Some(y)) = (quadrant_x, quadrant_y) {
            quadrant[x + y] += 1;
        }
    }

    let answer = quadrant.iter().fold(1, |acc, x| acc * x);

    println!("{answer}");
}

fn part2(mut robots: Vec<((i64, i64), (i64, i64))>) {
    let width = 101;
    let height = 103;

    for seconds in 1..width * height {
        let mut grid = [[false; 101]; 103];
        for robot in robots.iter_mut() {
            let ((px, py), (vx, vy)) = robot;
            *robot = (((*px + *vx) % width, (*py + *vy) % height), (*vx, *vy));
            if robot.0.0 < 0 {
                robot.0.0 += width;
            }
            if robot.0.1 < 0 {
                robot.0.1 += height;
            }
            grid[robot.0.1 as usize][robot.0.0 as usize] = true;
        }
        for row in grid {
            let mut exit = false;
            let mut run = 0;
            for cell in row {
                if cell {
                    run += 1;
                } else {
                    run = 0;
                }
                // This value of `width / 4` is estimated based on trial-and-error.
                if run > width / 4 {
                    println!("{seconds}");
                    exit = true;
                    break;
                }
            }
            if exit {
                break;
            }
        }
    }
}

fn main() {
    let robots: Vec<((i64, i64), (i64, i64))> = std::fs::read_to_string("day-14-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            let p_and_v: Vec<&str> = line.split_whitespace().collect();
            let p = p_and_v[0].split('=').collect::<Vec<_>>()[1];
            let v = p_and_v[1].split('=').collect::<Vec<_>>()[1];
            let p_xy: Vec<i64> = p.split(',').map(|n| n.parse::<i64>().unwrap()).collect();
            let v_xy: Vec<i64> = v.split(',').map(|n| n.parse::<i64>().unwrap()).collect();

            ((p_xy[0], p_xy[1]), (v_xy[0], v_xy[1]))
        })
        .collect();
    part1(&robots);
    part2(robots.clone());
}
