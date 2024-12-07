fn part1(equations: &Vec<(u64, Vec<u64>)>) {
    let mut sum = 0;
    for (target, nums) in equations {
        let mut stack: Vec<(u64, usize)> = Vec::from([
            (nums[0] + nums[1], 2),
            (nums[0] * nums[1], 2)
        ]);
        while let Some((result, idx)) = stack.pop() {
            if result == *target {
                sum += *target;
                break;
            }
            if idx >= nums.len() {
                continue;
            }
            stack.push((result + nums[idx], idx + 1));
            stack.push((result * nums[idx], idx + 1));
        }
    }
    println!("{sum}");
}

fn concat(a: u64, b: u64) -> u64 {
    let digits = b.checked_ilog10().unwrap_or(0) + 1;
    a * (10_u64.pow(digits as u32) as u64) + b
}

fn part2(equations: &Vec<(u64, Vec<u64>)>) {
    let mut sum = 0;
    for (target, nums) in equations {
        let mut stack: Vec<(u64, usize)> = Vec::from([
            (nums[0] + nums[1], 2),
            (nums[0] * nums[1], 2),
            (concat(nums[0], nums[1]), 2),
        ]);
        while let Some((result, idx)) = stack.pop() {
            if result == *target && idx == nums.len() {
                sum += *target;
                break;
            }
            if idx >= nums.len() {
                continue;
            }
            stack.push((concat(result, nums[idx]), idx + 1));
            stack.push((result + nums[idx], idx + 1));
            stack.push((result * nums[idx], idx + 1));
        }
    }
    println!("{sum}");
}

fn main() {
    let equations: Vec<(u64, Vec<u64>)> = std::fs::read_to_string("day-7-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            let sections: Vec<_> = line.split(": ").collect();
            let test: u64 = sections[0].parse::<u64>().unwrap();
            let nums: Vec<u64> = sections[1]
                .split_whitespace()
                .map(|n| n.parse::<u64>().unwrap())
                .collect();
            (test, nums)
        })
        .collect();
    part1(&equations);
    part2(&equations);
}
