fn part1(files_or_spaces: &Vec<(i64, bool)>) {
    let final_length: i64 = files_or_spaces
        .iter()
        .map(|&(num, is_file)| {
            if is_file { num } else { 0 }
        })
        .sum();
    let files_only: Vec<(usize, i64)> = files_or_spaces
        .iter()
        .filter_map(|&(num, is_file)| {
            if is_file && num != 0 { Some(num) } else { None }
        })
        .enumerate()
        .collect();

    let mut all_interval_sizes = files_or_spaces
        .iter()
        .map(|&(x, _)| x);

    let mut files_forward: Vec<_> = files_only
        .iter()
        .cloned()
        .rev()
        .collect();

    let mut files_reverse = files_only;

    let mut checksum = 0;
    let mut on_file = true;
    let mut interval_idx = all_interval_sizes.next().unwrap();

    for i in 0..final_length {
        while interval_idx == 0 {
            interval_idx = all_interval_sizes.next().unwrap();
            on_file = !on_file;
        }
        if on_file {
            if let Some((num, size)) = files_forward.pop() {
                if size > 1 {
                    files_forward.push((num, size - 1));
                }
                checksum += i * (num as i64);
            }
        } else {
            if let Some((num, size)) = files_reverse.pop() {
                if size > 1 {
                    files_reverse.push((num, size - 1));
                }
                checksum += i * (num as i64);
            }
        }
        interval_idx -= 1;
    }

    println!("{checksum}");
}

fn part2(files_or_spaces: &Vec<(i64, bool)>) {
    let mut files: Vec<(i64, i64, i64)> = Vec::new();
    let mut spaces: Vec<(i64, i64)> = Vec::new();
    let mut idx = 0;
    let mut id = 0;
    for &(size, is_file) in files_or_spaces {
        if is_file {
            files.push((id, idx, size));
            idx += size;
            id += 1;
        } else {
            spaces.push((idx, size));
            idx += size;
        }
    }
    for i in (0..files.len()).rev() {
        let first_free_space = spaces.iter().position(|&x| x.1 >= files[i].2);
        if let Some(free_space) = first_free_space {
            if spaces[free_space].0 < files[i].1 {
                let (file_id, _, file_size) = files[i];
                let (space_idx, space_size) = spaces[free_space];
                files[i] = (file_id, space_idx, file_size);
                spaces[free_space] = (space_idx + file_size, space_size - file_size);
            }
        }
    }

    let checksum = files
        .iter()
        .map(|&(file_id, file_idx, file_size)| {
            (file_idx .. file_idx + file_size)
                .fold(0, |acc, x| acc + file_id * x)
        })
        .sum::<i64>();

    println!("{checksum}");
}

fn main() {
    let files_or_spaces = std::fs::read_to_string("day-9-puzzle-input.txt")
        .unwrap()
        .trim()
        .chars()
        .fold((Vec::<(i64, bool)>::new(), 0), |acc, x| {
            let (mut v, i) = acc;
            let num = x.to_string().parse::<i64>().unwrap();
            v.push((num, i % 2 == 0));
            (v, i + 1)
        }).0;
    part1(&files_or_spaces);
    part2(&files_or_spaces);
}
