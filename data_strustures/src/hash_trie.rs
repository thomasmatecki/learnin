use std::collections::hash_map::DefaultHasher;
use std::convert::TryInto;
use std::fs::OpenOptions;
use std::hash::{Hash, Hasher};
use std::ops::Deref;
use std::rc::Rc;
use std::borrow::BorrowMut;
use std::cell::Cell;

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
    entries: [Option<Rc<dyn HashTableEntry>>; 16],
}

pub struct HashTrie {
    root: SubHashTable,
}

trait HashTableEntry {
    fn contains(&self, key: &u64, idx: u8) -> bool;
    fn sub_hashed(&self, e: &Rc<dyn HashTableEntry>) -> Rc<dyn HashTableEntry>;
}

impl SubHashTable {
    fn new() -> Self {
        SubHashTable {
            entries: Default::default(),
        }
    }

    fn add(&mut self, key: &u64, depth: u8) {
        let idx = sub_hash4(key, depth);
        let tab = Rc::new(SubHashTable::new());
        self.entries[idx] = Some(Rc::new(KeyEntry { key: *key }));

        self.entries[idx]
            .as_ref()
            .map(|k| Rc::new(SubHashTable::new()));

        self.entries[idx].as_ref().map(|k| {
            let x = Rc::new(KeyEntry { key: *key });
            x
        });

        match &self.entries[idx] {
            None => {
                self.entries[idx] = Some(Rc::new(KeyEntry { key: *key }));
            }
            Some(entry) => {
                self.entries[idx].as_ref().map(|k| entry.sub_hashed(k));
            }
        }
    }
}

impl HashTableEntry for KeyEntry {
    fn contains(&self, key: &u64, _idx: u8) -> bool {
        *key == self.key
    }

    fn sub_hashed(&self, e: &Rc<dyn HashTableEntry>) -> Rc<dyn HashTableEntry> {


        let mut new_sub_hash = Rc::new(SubHashTable::new());

        new_sub_hash.clone()
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

    fn sub_hashed(&self, e: &Rc<dyn HashTableEntry>) -> Rc<dyn HashTableEntry> {
        e.clone()
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

    fn add(&mut self, key: &u64) {}
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

    #[test]
    fn test_sub_hash_table_contains() {
        let mut sub_hash_table = SubHashTable::new();
        let h = calc_hash(&42);
        let i = sub_hash4(&h, 0);
        let key_entry_ref: Rc<dyn HashTableEntry> = Rc::new(KeyEntry { key: h });

        sub_hash_table.entries[i] = Some(key_entry_ref);
        assert!(sub_hash_table.contains(&h, 0));
    }

    #[test]
    fn test_sub_hash_table_refs() {
        let h = calc_hash(&42);
        let key_entry_ref: Rc<dyn HashTableEntry> = Rc::new(KeyEntry { key: h });
        let i = sub_hash4(&h, 0);

        let mut sub_hash_table0 = SubHashTable::new();
        sub_hash_table0.entries[i] = Some(key_entry_ref.clone());

        let mut sub_hash_table1 = SubHashTable::new();
        sub_hash_table1.entries[i] = Some(key_entry_ref.clone());
    }

    #[test]
    fn test_sub_hash_table_add_existing() {
        let h = calc_hash(&42);
        let key_entry_ref: Rc<dyn HashTableEntry> = Rc::new(KeyEntry { key: h });
        let i = sub_hash4(&h, 0);

        let mut sub_hash_table0 = Rc::new(SubHashTable::new());

        sub_hash_table0.add(&h, 0);
    }
}
