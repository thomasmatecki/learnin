use std::cmp::max;
use std::ops::{Index, IndexMut};

///
///
///
///
struct Object {
    value: u32,
    weight: u32,
}

struct Table {
    cols: usize,
    rows: usize,
    data: Vec<u32>,
}

impl Table {
    fn new(cols: usize, rows: usize) -> Table {
        Table {
            rows: rows,
            cols: cols,
            data: vec![0u32; cols * rows],
        }
    }
}

impl Index<(usize, u32)> for Table {
    type Output = u32;
    fn index(&self, idx: (usize, u32)) -> &u32 {
        let (row, col) = idx;
        &self[(row, col as usize)]
    }
}

impl IndexMut<(usize, u32)> for Table {
    fn index_mut(&mut self, idx: (usize, u32)) -> &mut u32 {
        let (row, col) = idx;
        self.index_mut((row, col as usize))
    }
}

impl Index<(usize, usize)> for Table {
    type Output = u32;
    fn index(&self, idx: (usize, usize)) -> &u32 {
        let (row, col) = idx;
        let value = &self.data[(row * self.cols) + col];
        return value;
    }
}

impl IndexMut<(usize, usize)> for Table {
    fn index_mut(&mut self, idx: (usize, usize)) -> &mut u32 {
        let (row, col) = idx;
        let value = &mut self.data[(row * self.cols) + col];
        return value;
    }
}
///
///
///
///
fn knapsack(objs: Vec<Object>, capacity: u32) -> u32 {
    let mut k_table = Table::new(capacity as usize + 1, objs.len() + 1);

    for (i_, obj) in objs.iter().enumerate() {
        let i = i_ + 1;

        for b in 1..(capacity + 1) {
            if obj.weight <= b {
                let value_incl_obj = k_table[(i - 1, b - obj.weight)] + obj.value;
                let value_excl_obj = k_table[(i - 1, b)];

                k_table[(i, b)] = max(value_incl_obj, value_excl_obj);
            } else {
                k_table[(i, b)] = k_table[(i - 1, b)];
            }
        }
    }

    k_table[(objs.len(), capacity)]
}

#[cfg(test)]
mod tests {
    use super::*;

    macro_rules! o {
        ( $value:expr, $weight:expr ) => {
            Object {
                value: $value,
                weight: $weight,
            }
        };
    }

    #[test]
    fn sets_table_value() {
        let mut k_table = Table::new(4, 5);
        k_table[(0 as usize, 0u32)] = 3;

        assert_eq!(k_table[(0 as usize, 0u32)], 3);
    }

    #[test]
    fn test_simple_ones() {
        let objs: Vec<Object> = vec![o!(1, 1), o!(1, 1), o!(1, 1), o!(1, 1)];
        let result = knapsack(objs, 4);
        assert_eq!(4, result)
    }

    #[test]
    fn test_optimal() {
        let objs: Vec<Object> = vec![
            o!(1, 2),
            o!(2, 2),
            o!(5, 8),
            o!(6, 3),
            o!(7, 4),
            o!(9, 6),
            o!(10, 10),
        ];
        let result = knapsack(objs, 4);
        assert_eq!(result, 7);
    }
}
