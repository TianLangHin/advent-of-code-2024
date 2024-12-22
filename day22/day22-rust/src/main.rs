use std::collections::BTreeMap;

fn part1(secrets: &Vec<i64>) {
    let mut total = 0i64;
    for &secret in secrets {
        let mut num = secret;
        let mask = (1 << 24) - 1;
        for _ in 0..2000 {
            num = ((num << 6) ^ num) & mask;
            num = ((num >> 5) ^ num) & mask;
            num = ((num << 11) ^ num) & mask;
        }
        total += num;
    }
    println!("{total}");
}

fn part2(secrets: &Vec<i64>) {
    let buyers: Vec<Vec<i64>> = secrets
        .iter()
        .map(|&secret| {
            let mut prices = Vec::from([secret % 10]);
            let mut num = secret;
            let mask = (1 << 24) - 1;
            for _ in 0..2000 {
                num = ((num << 6) ^ num) & mask;
                num = ((num >> 5) ^ num) & mask;
                num = ((num << 11) ^ num) & mask;
                prices.push(num % 10);
            }
            prices
        })
        .collect();

    let mut changes: Vec<BTreeMap<[i64; 4], i64>> = buyers
        .iter()
        .map(|_| BTreeMap::new())
        .collect();

    for i in 0..buyers.len() {
        for j in 4..buyers[i].len() {
            let differences = [
                buyers[i][j-3] - buyers[i][j-4],
                buyers[i][j-2] - buyers[i][j-3],
                buyers[i][j-1] - buyers[i][j-2],
                buyers[i][j] - buyers[i][j-1],
            ];
            let price = buyers[i][j];

            changes[i]
                .entry(differences)
                .or_insert(price);
        }
    }

    let mut most_bananas = 0i64;

    for i in 0..changes.len() {
        let keys: Vec<_> = changes[i].keys().copied().collect();
        for key in keys {
            let bananas = (0..changes.len())
                .map(|j| changes[j].get(&key).unwrap_or(&0))
                .sum();
            for j in 0..changes.len() {
                changes[j].remove(&key);
            }
            if bananas > most_bananas {
                most_bananas = bananas;
            }
        }
    }

    println!("{most_bananas}");
}

fn main() {
    let secrets: Vec<i64> = std::fs::read_to_string("day-22-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|secret| secret.parse::<i64>().unwrap())
        .collect();
    part1(&secrets);
    part2(&secrets);
}
