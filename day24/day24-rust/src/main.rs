use std::collections::HashMap;

#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq)]
enum LogicOp {
    And, Or, Xor,
}

fn part1(initial: &HashMap<String, bool>, links: &HashMap<(String, String, LogicOp), String>) {
    let mut gates = initial.clone();
    let final_digits: Vec<String> = links
        .values()
        .filter(|g| g.starts_with("z"))
        .cloned()
        .collect();
    while final_digits.iter().any(|z| !gates.contains_key(z)) {
        for ((g1, g2, op), res) in links {
            if let (Some(&g1_val), Some(&g2_val)) = (gates.get(g1), gates.get(g2)) {
                let res_val = match op {
                    LogicOp::And => g1_val && g2_val,
                    LogicOp::Or => g1_val || g2_val,
                    LogicOp::Xor => g1_val != g2_val,
                };
                gates.insert(res.clone(), res_val);
            }
        }
    }
    let answer: u64 = final_digits
        .iter()
        .map(|d| (d.clone(), d[1..].parse::<u64>().unwrap()))
        .fold(0u64, |acc, x| {
            let change_digit = *gates.get(&x.0).unwrap();
            if change_digit {
                acc | (1 << x.1)
            } else {
                acc
            }
        });
    println!("{answer}");
}

fn main() {
    let mut initial: HashMap<String, bool> = HashMap::new();
    let mut links: HashMap<(String, String, LogicOp), String> = HashMap::new();
    let puzzle_input = std::fs::read_to_string("day-24-puzzle-input.txt").unwrap();
    let mut section = true;
    for line in puzzle_input.lines() {
        if section {
            if line.is_empty() {
                section = false;
            } else {
                let components: Vec<&str> = line.split(": ").collect();
                initial.insert(components[0].into(), components[1] == "1");
            }
        } else {
            let components: Vec<&str> = line.split_whitespace().collect();
            let gate = if components[1] == "AND" {
                LogicOp::And
            } else if components[1] == "OR" {
                LogicOp::Or
            } else if components[1] == "XOR" {
                LogicOp::Xor
            } else {
                panic!()
            };
            links.insert((components[0].into(), components[2].into(), gate), components[4].into());
        }
    }
    part1(&initial, &links);
}
