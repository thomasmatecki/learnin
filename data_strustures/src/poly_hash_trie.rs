use std::convert::TryInto;

fn sub_hash(hash: &u64, offset: u8) -> usize {
    const SUB_HASH_LENGTH: u8 = 4;
    const SUBHASH_PAD: u8 = 64 - SUB_HASH_LENGTH;

    let bit_offset = offset * 4;

    ((hash << (SUBHASH_PAD - bit_offset)) >> SUBHASH_PAD)
        .try_into()
        .unwrap()
}

trait Node {
    fn contains(&self, key: u64) -> bool;
    fn replacement(&self, key: u64, depth: u8) -> &dyn Node;
}

struct Empty {}
struct HashKey {
    key: u64,
}
struct SubHashTable<'a> {
    entries: Box<[&'a dyn Node; 16]>,
}

impl Node for Empty {
    fn contains(&self, _key: u64) -> bool {
        false
    }
    fn replacement(&self, key: u64, depth: u8) -> &dyn Node {
        return Box::new(HashKey { key }).as_ref();
    }
}

impl Node for HashKey {
    fn contains(&self, key: u64) -> bool {
        key == self.key
    }
    fn replacement(&self, key: u64, depth: u8) -> &dyn Node {
        let mut entries: Box<[&'static dyn Node; 16]> = Box::new(Default::default());
        let existing_key_idx = sub_hash(&self.key, depth);
        let replacement: &dyn Node = entries[existing_key_idx].replacement(key, depth);
        entries[existing_key_idx] = replacement;
        let subhash_table = Box::new(SubHashTable { entries: entries });
        return &subhash_table;
    }
}
impl Node for SubHashTable<'_> {
    fn contains(&self, _key: u64) -> bool {
        return true;
    }
    fn replacement(&self, _: u64, _: u8) -> &dyn Node {
        unimplemented!()
    }
}

const EMPTY: Empty = Empty {};

impl Default for &dyn Node {
    fn default() -> Self {
        &EMPTY
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn subhash_table() {
        let entries: Box<[&'static dyn Node; 16]> = Box::new(Default::default());
        let subhash_table = SubHashTable { entries: entries };
    }

    #[test]
    fn hash_key_replacement() {
        let hk = HashKey { key: 92 };
        let r = hk.replacement(87, 0);
    }
}
