extern crate rand;

use rand::prelude::*;
use std::collections::hash_map::DefaultHasher;
use std::convert::TryInto;
use std::hash::{Hash, Hasher};

fn calc_hash<T: Hash>(t: &T) -> u64 {
    let mut s = DefaultHasher::new();
    t.hash(&mut s);
    s.finish()
}

fn sub_hash(hash: &u64, offset: u8) -> usize {
    const SUB_HASH_LENGTH: u8 = 4;
    const SUBHASH_PAD: u8 = 64 - SUB_HASH_LENGTH;

    let bit_offset = offset * 4;

    ((hash << (SUBHASH_PAD - bit_offset)) >> SUBHASH_PAD)
        .try_into()
        .unwrap()
}
#[derive(Debug)]
enum Node {
    Empty,
    SubHashTable { entries: Box<[Entry; 16]> },
    HashKey { key: u64 },
}

impl Default for Entry {
    fn default() -> Self {
        Entry { node: Node::Empty }
    }
}

#[derive(Debug)]
struct Entry {
    node: Node,
}

impl Entry {
    fn add(&mut self, key: u64, depth: u8) {
        match &mut self.node {
            Node::Empty => self.node = Node::HashKey { key },
            Node::SubHashTable { entries } => {
                let idx = sub_hash(&key, depth);
                &entries[idx].add(key, depth + 1);
            }
            Node::HashKey { key: existing_key } if *existing_key != key => {
                let mut entries: Box<[Entry; 16]> = Box::new(Default::default());

                let existing_key_idx = sub_hash(existing_key, depth);
                &entries[existing_key_idx].add(*existing_key, depth + 1);

                let idx = sub_hash(&key, depth);
                &entries[idx].add(key, depth + 1);

                self.node = Node::SubHashTable { entries };
            }
            _ => {}
        }
    }
    fn contains(&self, key: u64, depth: u8) -> bool {
        match &self.node {
            Node::Empty => false,
            Node::HashKey { key: existing_key } => *existing_key == key,
            Node::SubHashTable { entries } => {
                entries[sub_hash(&key, depth)].contains(key, depth + 1)
            }
        }
    }

    fn remove(&mut self, key: u64, depth: u8) {
        match &mut self.node {
            Node::Empty => {}
            Node::HashKey { key: _ } => {
                self.node = Node::Empty;
            }
            Node::SubHashTable { entries } => {
                let idx = sub_hash(&key, depth);
                &entries[idx].remove(key, depth + 1);
            }
        }
    }

    fn size(&self) -> u64 {
        match &self.node {
            Node::Empty => 0,
            Node::HashKey { key: _existing_key } => 1,
            Node::SubHashTable { entries } => entries.into_iter().map(|entry| entry.size()).sum(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn it_replaces_an_empty_node() {
        let mut entry = Entry { node: Node::Empty };
        let hash = calc_hash(&42);
        entry.add(hash, 0);

        assert_eq!(
            hash,
            match entry.node {
                Node::HashKey { key } => key,
                _ => unreachable!(),
            }
        );
    }

    #[test]
    fn it_does_not_contains_if_empty() {
        let entry = Entry { node: Node::Empty };
        assert!(!entry.contains(100, 0));
    }

    #[test]
    fn it_contains_a_hash_after_it_is_inserted() {
        let mut entry: Entry = Default::default();

        for hash in 0..15 {
            entry.add(hash, 0);
            assert!(entry.contains(hash, 0));
        }
    }

    #[test]
    fn it_does_not_contain_a_hash_after_it_is_removed() {
        let mut entry: Entry = Default::default();

        for hash in 0..15 {
            entry.add(hash, 0);
            assert!(entry.contains(hash, 0));
            assert_eq!(entry.size(), 1);
            entry.remove(hash, 0);
            assert!(!entry.contains(hash, 0));
            assert_eq!(entry.size(), 0);
        }
    }

    #[test]
    fn it_does_not_contains_a_hash_that_is_not_inserted() {
        let mut entry: Entry = Default::default();

        for hash in 0..15 {
            entry.add(hash, 0);
        }

        for hash in 16..32 {
            assert!(!entry.contains(hash, 0));
        }
    }
    struct Fibonacci {
        curr: u64,
        next: u64,
    }

    impl Iterator for Fibonacci {
        // From https://doc.rust-lang.org/rust-by-example/trait/iter.html
        type Item = u64;
        fn next(&mut self) -> Option<u64> {
            let new_next = self.curr + self.next;
            self.curr = self.next;
            self.next = new_next;
            Some(self.curr)
        }
    }

    impl Fibonacci {
        fn new() -> Fibonacci {
            Fibonacci { curr: 0, next: 1 }
        }
    }

    #[test]
    fn it_has_size_zero_if_empty() {
        let entry = Entry { node: Node::Empty };
        assert_eq!(entry.size(), 0);
    }

    #[test]
    fn it_can_contain_fibonacci_numbers() {
        let mut entry: Entry = Default::default();
        let fib_seq = Fibonacci::new();

        // The 93rd fibonacci number is more than 64 bits
        for (i, hash) in fib_seq.skip(1).take(91).enumerate() {
            assert_eq!(i as u64, entry.size());
            entry.add(hash, 0);
            assert!(entry.contains(hash, 0));
        }
    }

    #[test]
    fn it_inserts_into_a_sub_hash_table_node() {
        let entries: Box<[Entry; 16]> = Box::new(Default::default());

        let mut entry = Entry {
            node: Node::SubHashTable { entries: entries },
        };
        let hash = calc_hash(&42);
        entry.add(hash, 0);
        assert!(entry.contains(hash, 0));
    }

    #[test]
    fn it_inserts_same_subhash_into_same_sub_hash_table_node() {
        let mut entry: Entry = Default::default();

        for hash in 0..15 {
            entry.add(hash, 0);
        }
    }

    #[test]
    fn it_can_contain_many_u64s() {
        let mut entry: Entry = Default::default();

        for _ in 0..10000000 {
            let hash: u64 = random();
            entry.add(hash, 0);
        }

        assert_eq!(entry.size(), 10000000)
    }
}
