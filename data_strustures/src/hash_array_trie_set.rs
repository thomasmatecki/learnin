use crate::hash_array_trie_set::Node::Value;
use std::hash::Hash;
use std::mem::replace;
use std::ops::Index;

struct BitMaskedTable<T> {
    mask: u16,
    table: Vec<T>,
}

impl<T> BitMaskedTable<T> {
    fn new() -> Self {
        BitMaskedTable {
            mask: 0,
            table: Vec::new(),
        }
    }

    /// Given:          1000 1000 0110 0000
    /// For index(4)         ^
    /// shift right 11  0000 0000 0001 0001
    /// Is the low bit set?
    fn _is_set(&self, index: usize) -> bool {
        1u16 & self.mask >> (15 - index) == 1u16
    }

    fn _index_for(&self, index: usize) -> usize {
        if index == 0 {
            0
        } else {
            let offset = 16 - (index % 16);
            let offset_mask = self.mask >> offset;
            offset_mask.count_ones() as usize
        }
    }

    fn put(&mut self, index: usize, element: T) {
        let table_index = self._index_for(index);

        if self._is_set(index) {
            panic!("index already set");
        } else {
            self.table.insert(table_index, element);
            self.mask |= 0b1000_0000_0000_0000 >> index
        }
    }

    fn get(&self, index: usize) -> Option<&T> {
        if !self._is_set(index) {
            None
        } else {
            Some(&self.table[self._index_for(index)])
        }
    }
}

impl<T> Index<usize> for BitMaskedTable<T> {
    type Output = T;
    fn index(&self, index: usize) -> &Self::Output {
        &self.table[self._index_for(index)]
    }
}

enum Node<T> {
    Empty,
    Value(T),
    HashTable(Box<[Node<T>; 16]>),
}

impl<T> Default for Node<T> {
    fn default() -> Self {
        Node::Empty
    }
}

impl<T> Node<T> {
    fn new_table(elem: &T) -> Node<T> {
        Node::HashTable(Box::new(Default::default()))
    }

    #[inline]
    fn insert(&mut self, elem: T) {
        match self {
            Node::Empty => {
                let _ = replace(self, Value(elem));
            }
            Node::Value(_) => {
                let _ = replace(self, Node::new_table(&elem));
            }
            Node::HashTable(_) => {}
        };
    }
}

struct HashArrayTrieSet<T: Hash> {
    root: Node<T>,
}

impl<T: Hash> HashArrayTrieSet<T> {}

#[cfg(test)]
mod tests {
    use super::BitMaskedTable;
    mod index_for {
        use super::BitMaskedTable;

        #[test]
        fn test_single_put() {
            let bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            for index in 0..16 {
                let table_index = bmt._index_for(index);
                assert_eq!(table_index, 0);
            }
        }

        #[test]
        fn test_incre_put() {
            use super::BitMaskedTable;
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            assert_eq!(bmt._index_for(3), 0);
            bmt.put(3, &element);
            assert_eq!(bmt._index_for(4), 1);
            assert_eq!(&bmt[3], &element);
        }
        #[test]
        fn test_decre_put() {
            use super::BitMaskedTable;
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            assert_eq!(bmt._index_for(4), 0);
            bmt.put(3, &element);
            assert_eq!(bmt._index_for(3), 0);
        }

        #[test]
        fn test_max_put() {
            use super::BitMaskedTable;
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            assert_eq!(bmt._index_for(15), 0);
            bmt.put(15, &element);
            assert_eq!(bmt._index_for(0), 0);
        }
    }

    mod put {

        use super::BitMaskedTable;
        #[test]
        fn test_insert() {
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            bmt.put(3, &element);
            assert_eq!(bmt.get(3).unwrap(), &element);
        }
        #[test]
        fn test_max_insert() {
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            bmt.put(15, &element);
            assert_eq!(bmt.mask, 1);
        }
        #[test]
        fn test_min_insert() {
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            bmt.put(0, &element);
            assert_eq!(bmt.mask, 0b1000_0000_0000_0000);
        }

        #[test]
        #[should_panic]
        fn test_duplicate_insert() {
            let mut bmt: BitMaskedTable<&str> = BitMaskedTable::new();
            let element = "the quick brown dog";
            bmt.put(0, &element);
            bmt.put(0, &element);
        }
    }
}
