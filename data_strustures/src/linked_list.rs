struct List<T> {
    head: Link<T>,
}

type Link<T> = Option<Box<Node<T>>>;

struct Node<T> {
    elem: T,
    next: Link<T>,
}

impl<T> List<T> {
    pub fn new() -> Self {
        List { head: None }
    }
    pub fn push(&mut self, elem: T) {
        self.head = Some(Box::new(Node {
            elem,
            next: self.head.take(),
        }));
    }

    pub fn pop(&mut self) -> Option<T> {
        match self.head.take() {
            None => None,
            Some(node) => {
                self.head = node.next;
                return Some(node.elem);
            }
        }
    }

    pub fn update(&self) {}

    pub fn peek(&self) -> Option<&T> {
        self.head.as_ref().map(|node| &node.elem)
    }

    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.head.as_mut().map(|node| &mut node.elem)
    }
}

mod test {
    #[cfg(test)]
    use super::*;

    #[test]
    fn basics() {
        let mut list = List::new();

        // Check empty list behaves right
        assert_eq!(list.pop(), None);

        // Populate list
        list.push(1);
        list.push(2);
        list.push(3);

        assert_eq!(list.peek(), Some(&3));

        // Check normal removal
        assert_eq!(list.pop(), Some(3));
        assert_eq!(list.pop(), Some(2));

        // Push some more just to make sure nothing's corrupted
        list.push(4);
        list.push(5);

        // Check normal removal
        assert_eq!(list.pop(), Some(5));
        assert_eq!(list.pop(), Some(4));

        // Check exhaustion
        assert_eq!(list.pop(), Some(1));
        assert_eq!(list.pop(), None);
    }

    #[test]
    fn can_map_peek_mut() {
        let mut list = List::new();

        // Populate list
        list.push(1);

        list.peek_mut().map(|value| {
            // set to 42
            *value = 42
        });
        assert_eq!(list.peek(), Some(&42));
    }
    #[test]
    fn can_replace_peek_mut() {
        let mut s0: Option<u32> = Some(2);

        s0.replace(3);

        let mut list = List::new();

        // Populate list
        list.push(1);

        let o0 = list.peek_mut();

        o0.map(|value| {
            // set to 42
            *value = 42
        });

        assert_eq!(list.peek(), Some(&42));
        list.peek_mut().replace(&mut 3);
        //        assert_eq!(list.peek(), Some(&3));

        let mut v1 = 4;
        let mut s1: Option<&mut u32> = list.peek_mut();
        s1.replace(&mut v1);

        let mut v2 = 4;
        let e2 = Some(&mut v2);
        assert_eq!(s1, e2);
    }
}
