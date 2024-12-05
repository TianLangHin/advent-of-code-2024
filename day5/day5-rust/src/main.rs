use std::collections::{BTreeMap, BTreeSet};

fn part1(page_ordering_rules: &Vec<(u64, u64)>, updates: &Vec<Vec<u64>>) {
    let mut page_orders: BTreeMap<u64, BTreeSet<u64>> = BTreeMap::new();
    for &(x, y) in page_ordering_rules {
        page_orders
            .entry(x)
            .and_modify(|list| {
                list.insert(y);
            })
            .or_insert(BTreeSet::from([y]));
    }
    let answer = updates
        .iter()
        .map(|update| {
            let mut correct = true;
            let mut seen: BTreeSet<u64> = BTreeSet::from([update[0]]);
            for &item in update.iter().skip(1) {
                if let Some(rules) = page_orders.get(&item) {
                    let mut intersection = rules.intersection(&seen);
                    if intersection.next().is_some() {
                        correct = false;
                        break;
                    }
                }
                seen.insert(item);
            }
            if correct {
                update[update.len() / 2]
            } else {
                0
            }
        })
        .sum::<u64>();
    println!("{answer}");
}

fn part2(page_ordering_rules: &Vec<(u64, u64)>, updates: &Vec<Vec<u64>>) {
    let mut page_orders: BTreeMap<u64, BTreeSet<u64>> = BTreeMap::new();
    for &(x, y) in page_ordering_rules {
        page_orders
            .entry(x)
            .and_modify(|list| {
                list.insert(y);
            })
            .or_insert(BTreeSet::from([y]));
    }
    let mut sum: u64 = 0;
    for update in updates {
        let mut swapped = false;
        let mut row = update.clone();
        let mut index = 0;
        while index < row.len() {
            let first_violation = (0..index)
                .find(|&i| {
                    if let Some(rules) = page_orders.get(&row[index]) {
                        rules.contains(&row[i])
                    } else {
                        false
                    }
                });
            if let Some(swap_index) = first_violation {
                swapped = true;
                (row[index], row[swap_index]) = (row[swap_index], row[index]);
                index = swap_index + 1;
            } else {
                index += 1;
            }
        }
        if swapped {
            sum += row[row.len() / 2];
        }
    }

    println!("{sum}");
}

fn main() {
    let mut page_ordering_rules: Vec<(u64, u64)> = Vec::new();
    let mut updates: Vec<Vec<u64>> = Vec::new();
    let mut section = true;
    let _ = std::fs::read_to_string("day-5-puzzle-input.txt")
        .unwrap()
        .lines()
        .for_each(|line| {
            if line.is_empty() {
                section = false;
            } else if section {
                let numbers: Vec<u64> =
                    line.split('|').map(|n| n.parse::<u64>().unwrap()).collect();
                page_ordering_rules.push((numbers[0], numbers[1]));
            } else {
                updates.push(line.split(',').map(|n| n.parse::<u64>().unwrap()).collect());
            }
        });

    part1(&page_ordering_rules, &updates);
    part2(&page_ordering_rules, &updates);
}
