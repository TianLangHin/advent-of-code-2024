use regex::Regex;

fn part1(claw_machines: &Vec<Vec<i64>>) {

    let mut token_cost = 0;

    for machine in claw_machines {
        let (x_a, y_a) = (machine[0], machine[1]);
        let (x_b, y_b) = (machine[2], machine[3]);
        let (x, y) = (machine[4], machine[5]);

        let mut a_button_times = (x * y_b - y * x_b, x_a * y_b - y_a * x_b);
        let mut b_button_times = (x * y_a - y * x_a, x_b * y_a - y_b * x_a);

        if a_button_times.0 * a_button_times.1 < 0 ||
            b_button_times.0 * b_button_times.1 < 0 {
            continue;
        }

        if a_button_times.0 < 0 {
            a_button_times = (-a_button_times.0, -a_button_times.1);
        }
        if b_button_times.0 < 0 {
            b_button_times = (-b_button_times.0, -b_button_times.1);
        }

        if a_button_times.0 % a_button_times.1 != 0 || 
            b_button_times.0 % b_button_times.1 != 0 {
            continue;
        }

        let n_a = a_button_times.0 / a_button_times.1;
        let n_b = b_button_times.0 / b_button_times.1;

        if n_a <= 100 && n_b <= 100 {
            token_cost += 3 * n_a + n_b;
        }
    }

    println!("{token_cost}");
}

fn part2(claw_machines: &Vec<Vec<i64>>) {

    let mut token_cost = 0;

    for machine in claw_machines {
        let (x_a, y_a) = (machine[0], machine[1]);
        let (x_b, y_b) = (machine[2], machine[3]);
        let (x, y) = (10000000000000 + machine[4], 10000000000000 + machine[5]);

        let mut a_button_times = (x * y_b - y * x_b, x_a * y_b - y_a * x_b);
        let mut b_button_times = (x * y_a - y * x_a, x_b * y_a - y_b * x_a);

        if a_button_times.0 * a_button_times.1 < 0 ||
            b_button_times.0 * b_button_times.1 < 0 {
            continue;
        }

        if a_button_times.0 < 0 {
            a_button_times = (-a_button_times.0, -a_button_times.1);
        }
        if b_button_times.0 < 0 {
            b_button_times = (-b_button_times.0, -b_button_times.1);
        }

        if a_button_times.0 % a_button_times.1 != 0 || 
            b_button_times.0 % b_button_times.1 != 0 {
            continue;
        }

        let n_a = a_button_times.0 / a_button_times.1;
        let n_b = b_button_times.0 / b_button_times.1;

        token_cost += 3 * n_a + n_b;
    }

    println!("{token_cost}");
}

fn main() {
    let pattern = Regex::new(
        r"Button A: X\+(\d+)[,] Y\+(\d+)\nButton B: X\+(\d+)[,] Y\+(\d+)\nPrize: X=(\d+)[,] Y=(\d+)"
    ).unwrap();

    let claw_machines: Vec<Vec<i64>> = pattern
        .captures_iter(&std::fs::read_to_string("day-13-puzzle-input.txt").unwrap())
        .map(|capture| {
            (1..=6).map(|n| capture[n].parse::<i64>().unwrap()).collect()
        })
        .collect();

    part1(&claw_machines);
    part2(&claw_machines);
}
