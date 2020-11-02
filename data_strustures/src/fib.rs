/*
 *
 *
 */

pub struct Fibonacci {
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
    pub fn new() -> Fibonacci {
        Fibonacci { curr: 0, next: 1 }
    }
}