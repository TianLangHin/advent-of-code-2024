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

        let quadrant_x = match result_x.cmp(&((width) / 2)) {
            Ordering::Equal => None,
            Ordering::Less => Some(0),
            Ordering::Greater => Some(2),
        };

        let quadrant_y = match result_y.cmp(&((height) / 2)) {
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
}
