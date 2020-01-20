use core::num::FpCategory::Subnormal;
use std::borrow::{Borrow, BorrowMut};
use std::cell::Cell;
use std::collections::hash_map::DefaultHasher;
use std::convert::TryInto;
use std::hash::{Hash, Hasher};
use std::ops::Deref;
use std::rc::Rc;

#[derive(Hash)]
struct Person {
    id: u32,
    name: String,
    phone: u64,
}

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

struct Entry {
    node: Node,
}

impl Entry {
    fn add(&mut self, key: u64, depth: u8) {
        let idx = sub_hash(&key, depth);
        match &mut self.node {
            Node::Empty => self.node = Node::HashKey { key: key },
            Node::SubHashTable { entries } => {
                &entries[idx].add(key, depth + 1);
            }
            Node::HashKey { key: existing_key } if *existing_key != key => {
                let mut entries: Box<[Entry; 16]> = Box::new(Default::default());
                &entries[idx].add(key, depth);

                let idx = sub_hash(existing_key, depth);
                &entries[idx].add(*existing_key, depth);
                self.node = Node::SubHashTable { entries: entries };
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
    fn it_empty_returns_false() {
        let entry = Entry { node: Node::Empty };
        assert!(!entry.contains(100, 0));
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
}
