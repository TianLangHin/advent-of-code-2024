fn get_combo_operand(x: i64, a: i64, b: i64, c: i64) -> i64 {
    match x {
        0 | 1 | 2 | 3 => x,
        4 => a,
        5 => b,
        6 => c,
        _ => panic!(),
    }
}

fn part1(mut a: i64, mut b: i64, mut c: i64, program: &Vec<i64>) {
    let mut output: Vec<String> = Vec::new();

    let mut i = 0;
    while i < program.len() {
        let fst = program[i];
        let snd = program[i+1];
        match fst {
            0 => {
                let combo_operand = get_combo_operand(snd, a, b, c);
                a = a / (1 << combo_operand);
                i += 2;
            }
            1 => {
                b = b ^ snd;
                i += 2;
            }
            2 => {
                let combo_operand = get_combo_operand(snd, a, b, c);
                b = combo_operand % 8;
                i += 2;
            }
            3 => {
                if a == 0 {
                    i += 2;
                } else {
                    i = snd as usize;
                }
            }
            4 => {
                b = b ^ c;
                i += 2;
            }
            5 => {
                let combo_operand = get_combo_operand(snd, a, b, c);
                output.push((combo_operand % 8).to_string());
                i += 2;
            }
            6 => {
                let combo_operand = get_combo_operand(snd, a, b, c);
                b = a / (1 << combo_operand);
                i += 2;
            }
            7 => {
                let combo_operand = get_combo_operand(snd, a, b, c);
                c = a / (1 << combo_operand);
                i += 2;
            }
            _ => {}
        }
    }

    println!("{}", output.join(","));
}

fn main() {
    let mut a: i64 = 0;
    let mut b: i64 = 0;
    let mut c: i64 = 0;
    let mut program: Vec<i64> = Vec::new();
    let _ = std::fs::read_to_string("day-17-puzzle-input.txt")
        .unwrap()
        .lines()
        .for_each(|line| {
            if line.starts_with("Register A: ") {
                a = line
                    .split("Register A: ")
                    .nth(1)
                    .unwrap()
                    .parse::<i64>()
                    .unwrap();
            } else if line.starts_with("Register B: ") {
                b = line
                    .split("Register B: ")
                    .nth(1)
                    .unwrap()
                    .parse::<i64>()
                    .unwrap();
            } else if line.starts_with("Register C: ") {
                c = line
                    .split("Register C: ")
                    .nth(1)
                    .unwrap()
                    .parse::<i64>()
                    .unwrap();
            } else if line.starts_with("Program: ") {
                program = line 
                    .split("Program: ")
                    .nth(1)
                    .unwrap()
                    .split(',')
                    .map(|n| n.parse::<i64>().unwrap())
                    .collect();
            }
        });
    part1(a, b, c, &program);
}
