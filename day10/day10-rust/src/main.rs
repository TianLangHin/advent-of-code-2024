use std::{
    cmp::{max, min},
    collections::{BTreeSet, VecDeque},
};

fn part1(landscape: &Vec<Vec<i64>>) {
    let mut answer = 0;
    let rows = landscape.len() as i64;
    let cols = landscape[0].len() as i64;
    for i in 0i64..rows {
        for j in 0i64..cols {
            if landscape[i as usize][j as usize] == 0 {
                let mut found_nines: BTreeSet<(i64, i64)> = BTreeSet::new();
                let mut frontier = VecDeque::from([(i, j, 0)]);
                while let Some((row, col, num)) = frontier.pop_front() {
                    if num == 9 {
                        found_nines.insert((row, col));
                    } else {
                        let neighbours = [
                            (max(row-1, 0), col),
                            (min(row+1, rows-1), col),
                            (row, max(col-1, 0)),
                            (row, min(col+1, cols-1)),
                        ];
                        neighbours
                            .iter()
                            .for_each(|&(r, c)| {
                                if landscape[r as usize][c as usize] == num + 1 {
                                    frontier.push_back((r, c, num + 1));
                                }
                            })
                    }
                }
                answer += found_nines.len();
            }
        }
    }
    println!("{answer}");
}

fn part2(landscape: &Vec<Vec<i64>>) {
    let mut reachable: Vec<Vec<i64>> = landscape
        .iter()
        .map(|row| row.iter().map(|&n| if n == 9 { 1 } else { 0 }).collect())
        .collect();
    let rows = reachable.len() as i64;
    let cols = reachable[0].len() as i64;
    for number in (0..9).rev() {
        for i in 0i64..rows {
            for j in 0i64..cols {
                if landscape[i as usize][j as usize] == number {
                    let neighbours = [
                        (max(0, i-1), j),
                        (min(rows-1, i+1), j),
                        (i, max(0, j-1)),
                        (i, min(cols-1, j+1)),
                    ];
                    reachable[i as usize][j as usize] += neighbours
                        .iter()
                        .map(|&(row, col)| {
                            if landscape[row as usize][col as usize] == number + 1 {
                                reachable[row as usize][col as usize]
                            } else {
                                0
                            }
                        })
                        .sum::<i64>();
                }
            }
        }
    }
    let answer = (0..rows)
        .map(|i| {
            (0..cols)
                .map(|j| {
                    if landscape[i as usize][j as usize] == 0 {
                        reachable[i as usize][j as usize]
                    } else {
                        0
                    }
                })
                .sum::<i64>()
        })
        .sum::<i64>();
    println!("{answer}");
}

fn main() {
    let landscape: Vec<Vec<i64>> = std::fs::read_to_string("day-10-puzzle-input.txt")
        .unwrap()
        .lines()
        .map(|line| {
            line.chars().map(|x| x.to_string().parse::<i64>().unwrap()).collect()
        })
        .collect();
    part1(&landscape);
    part2(&landscape);
}
