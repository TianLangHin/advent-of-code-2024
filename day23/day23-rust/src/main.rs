use std::collections::{HashMap, HashSet, VecDeque};

type Computer = [char; 2];

fn part1(graph: &HashMap<Computer, HashSet<Computer>>) {
    let mut found_cliques: HashSet<[Computer; 3]> = HashSet::new();

    for (&computer, ref linked) in graph {
        if computer[0] == 't' {
            let mut cliques: HashSet<(Computer, Computer)> = linked
                .iter()
                .filter_map(|&c| {
                    graph
                        .get(&c)
                        .map(move |d| d.into_iter().map(move |&e| (c, e)))
                })
                .flatten()
                .collect();

            cliques.retain(|(_, d)| linked.contains(d));

            for clique in cliques {
                let mut vec_clique = vec![computer, clique.0, clique.1];
                vec_clique.sort();

                let key = [vec_clique[0], vec_clique[1], vec_clique[2]];

                found_cliques.insert(key);
            }
        }
    }

    println!("{}", found_cliques.len());
}

fn part2(graph: &HashMap<Computer, HashSet<Computer>>) {
    let mut keys: VecDeque<Computer> = graph.keys().copied().collect();
    let mut biggest_clique_found = HashSet::new();

    while keys.len() > biggest_clique_found.len() {
        let starting_computer = keys.pop_front().unwrap();
        let mut clique: HashSet<Computer> = HashSet::from([starting_computer]);

        for &key in &keys {
            let empty = HashSet::new();
            let intersection = graph.get(&key).unwrap_or(&empty).intersection(&clique);
            if intersection.count() == clique.len() {
                clique.insert(key);
            }
        }

        keys.retain(|computer| !clique.contains(computer));

        if clique.len() > biggest_clique_found.len() {
            biggest_clique_found = clique;
        }
    }

    let mut computers: Vec<Computer> = biggest_clique_found.into_iter().collect();
    computers.sort();

    let password = computers
        .into_iter()
        .map(|c| format!("{}{}", c[0], c[1]))
        .collect::<Vec<String>>()
        .join(",");

    println!("{password}");
}

fn main() {
    let mut graph: HashMap<Computer, HashSet<Computer>> = HashMap::new();
    let _ = std::fs::read_to_string("day-23-puzzle-input.txt")
        .unwrap()
        .lines()
        .for_each(|line| {
            let chars: Vec<char> = line.chars().collect();
            let c1 = [chars[0], chars[1]];
            let c2 = [chars[3], chars[4]];

            graph
                .entry(c1)
                .and_modify(|v| {
                    v.insert(c2);
                })
                .or_insert(HashSet::from([c2]));
            graph
                .entry(c2)
                .and_modify(|v| {
                    v.insert(c1);
                })
                .or_insert(HashSet::from([c1]));
        });
    part1(&graph);
    part2(&graph);
}
