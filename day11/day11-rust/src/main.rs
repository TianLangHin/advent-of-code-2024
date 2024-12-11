use std::collections::BTreeMap;

struct Memokeeper {
    memo: BTreeMap<(i64, usize), i64>,
    steps: usize,
}

fn digits(x: i64) -> i64 {
    (x.checked_ilog10().unwrap_or(0) + 1) as i64
}

impl Memokeeper {
    fn depth_first_search(&mut self, stone_value: i64, blink_step: usize) -> i64 {
        if blink_step == self.steps {
            return 1;
        }
        if let Some(&stones_created) = self.memo.get(&(stone_value, blink_step)) {
            return stones_created;
        } else {
            let d = digits(stone_value);
            let stones_created = if stone_value == 0 {
                self.depth_first_search(1, blink_step + 1)
            } else if d % 2 == 0 {
                let fst_num = stone_value / (10_i64.pow((d/2) as u32) as i64);
                let snd_num = stone_value % (10_i64.pow((d/2) as u32) as i64);
                self.depth_first_search(fst_num, blink_step + 1)
                    + self.depth_first_search(snd_num, blink_step + 1)
            } else {
                self.depth_first_search(stone_value * 2024, blink_step + 1)
            };
            self.memo.insert((stone_value, blink_step), stones_created);
            return stones_created;
        }
    }
}

fn part1(stones: &[i64]) {
    let mut memo_keeper = Memokeeper {
        memo: BTreeMap::new(),
        steps: 25,
    };
    println!(
        "{}",
        stones.iter().map(|&stone| memo_keeper.depth_first_search(stone, 0)).sum::<i64>()
    );
}

fn part2(stones: &[i64]) {
    let mut memo_keeper = Memokeeper {
        memo: BTreeMap::new(),
        steps: 75,
    };
    println!(
        "{}",
        stones.iter().map(|&stone| memo_keeper.depth_first_search(stone, 0)).sum::<i64>()
    );
}

fn main() {
    let stones: Vec<i64> = std::fs::read_to_string("day-11-puzzle-input.txt")
        .unwrap()
        .split_whitespace()
        .map(|stone| stone.parse::<i64>().unwrap())
        .collect();
    part1(&stones);
    part2(&stones);
}
