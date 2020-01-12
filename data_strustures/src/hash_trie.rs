use std::collections::hash_map::DefaultHasher;
use std::convert::TryInto;
use std::hash::{Hash, Hasher};

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

fn sub_hash4(h: &u64, offset: u8) -> usize {
    const SUB_HASH_LENGTH: u8 = 4;
    const SUBHASH_PAD: u8 = 64 - SUB_HASH_LENGTH;

    let bit_offset = offset * 4;

    ((h << (SUBHASH_PAD - bit_offset)) >> SUBHASH_PAD)
        .try_into()
        .unwrap()
}

/// BEGIN
struct KeyEntry {
    key: u64,
}

struct SubHashTable {
    entries: [Option<Box<dyn HashTableEntry>>; 16],
}

pub struct HashTrie {
    root: SubHashTable,
}

trait HashTableEntry {
    fn contains(&self, key: &u64, idx: u8) -> bool;
    fn sub_hashed(self: Box<Self>) -> Box<SubHashTable>;
}

impl SubHashTable {
    fn new() -> Self {
        SubHashTable {
            entries: Default::default(),
        }
    }

    fn add(&mut self, key: &u64, depth: u8) {
        let idx = sub_hash4(key, depth);
        let maybe_entry = &self.entries[idx];

        match maybe_entry {
            None => self.entries[idx] = Some(Box::new(KeyEntry { key: *key })),
            Some(entry) => {
                entry.sub_hashed();
            }
        }
    }
}

impl HashTableEntry for KeyEntry {
    fn contains(&self, key: &u64, _idx: u8) -> bool {
        *key == self.key
    }

    fn sub_hashed(self: Box<Self>) -> Box<SubHashTable> {
        let new_box = Box::new(SubHashTable::new());
        return new_box;
    }
}

impl HashTableEntry for SubHashTable {
    fn contains(&self, key: &u64, depth: u8) -> bool {
        let idx = sub_hash4(key, depth);
        let maybe_entry = &self.entries[idx];

        match maybe_entry {
            Some(entry) => entry.contains(key, depth + 1),
            None => false,
        }
    }

    fn sub_hashed(self: Box<Self>) -> Box<SubHashTable> {
        self
    }
}

impl HashTrie {
    fn new() -> Self {
        HashTrie {
            root: SubHashTable::new(),
        }
    }
    fn contains(&self, key: &u64) -> bool {
        self.root.contains(key, 0)
    }
}

mod test {
    use super::*;

    #[test]
    fn test_key_entry() {
        let _key_entry = KeyEntry { key: 42 };
    }

    #[test]
    fn test_sub_hash_table_entry() {
        let _sub_hash_table_entry = SubHashTable::new();
    }

    #[test]
    fn test_sub_hash4() {
        for i in 0..15 {
            assert_eq!(15, sub_hash4(&u64::max_value(), i));
        }
    }
}
