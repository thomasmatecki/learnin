struct List<T> {
    head: Link<T>,
}

type Link = Option<Box<Node<T>>>;

struct Node<T> {
    elem: T,
    next: Link<T>,
}
