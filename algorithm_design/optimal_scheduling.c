/*
 * An optimal Scheduling Algorithm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECKED_HEIGHT(node_symb) (node_symb ? node_symb->height : 0)

/*
 * First, we need a 'Set' data structure to hold our intervals. I'll
 * give an AVL tree a try.
 */
char SPLIT_CHAR = ' ';

typedef struct Interval {
  int from;
  int to;
} s_interval;

typedef struct IntervalSetNode {
  s_interval *val;
  struct IntervalSetNode *left;
  struct IntervalSetNode *right;
  int height;
} _s_interval_set_node;

typedef struct IntervalSet {
  _s_interval_set_node *head;
} interval_set;

static void print_interval(s_interval *interval) {
  printf("%d ... %d\n", interval->from, interval->to);
}

int max_height(_s_interval_set_node *node_a, _s_interval_set_node *node_b) {
  int a_depth = CHECKED_HEIGHT(node_a);
  int b_depth = CHECKED_HEIGHT(node_b);

  if (a_depth > b_depth) {
    return a_depth;
  } else {
    return b_depth;
  }
}
static interval_set *new_set(void) {
  interval_set *set = malloc(sizeof(interval_set));
  set->head = NULL;
  return set;
}

/*
 *  From...
 *    A        return=C
 *   / \             / \
 *  B   C           A   E
 *     / \         / \
 *    D   E       B   D
 * Preserving: B < A < D < C < E
 *
 *    A         A         A   return=C
 *   / \       / \       / \        / \
 *  B   C  => B   C  => B   D      A   E
 *     / \       / \              / \
 *    D   E     A   E            B   D
 */
static _s_interval_set_node *rotate_left(_s_interval_set_node *node) {

  _s_interval_set_node *tmp = node->right->left; // D, above
  node->right->left = node;                      // C->left  <- A
  _s_interval_set_node *ret = node->right;       // C, above
  node->right = tmp;                             // C->right <- D

  // Recalculate height of A;
  node->height = 1 + max_height(node->right, node->left);

  // Recalculate height of C;
  ret->height = 1 + max_height(ret->right, ret->left);

  return ret;
}

/*
 * The opposite of rotate left
 */
static _s_interval_set_node *rotate_right(_s_interval_set_node *node) {

  _s_interval_set_node *tmp = node->left->right;
  node->left->right = node;
  _s_interval_set_node *ret = node->left;
  node->left = tmp;

  node->height = 1 + max_height(node->right, node->left);

  ret->height = 1 + max_height(ret->right, ret->left);
  return ret;
}

static int get_balance(_s_interval_set_node *node) {
  return CHECKED_HEIGHT(node->left) - CHECKED_HEIGHT(node->right);
}

/*
 * Recursively traverses `node` and inserts `interval` at the
 * appropriate position.
 */
static _s_interval_set_node *insert(_s_interval_set_node *node,
                                    s_interval *interval, int d) {
  if (!node) {
    node = malloc(sizeof(_s_interval_set_node));
    node->val = interval;
    node->left = NULL;
    node->right = NULL;
  } else if (node->val->from <= interval->from) {
    node->left = insert(node->left, interval, d + 1);
  } else {
    node->right = insert(node->right, interval, d + 1);
  }

  // Increment the height
  node->height = 1 + max_height(node->right, node->left);

  int balance = get_balance(node);
  printf("node at %p is height %d and with balance %d\n", node, node->height,
         balance);

  if (balance > 1) {
    // The left branch is too heavy
    if (get_balance(node->left) < 0) {
      /* Left/Right heavier than Left/Left,
       * 1) Rotate A(first left) to the left.
       * 2) Rotate C(the root) to the right.
       *     C           C           D
       *    / \         / \         / \
       *   A   E       D   E       /   \
       *  / \     =>  / \    =>   A     C
       * B   D       A   G       / \   / \
       *    / \     / \         B   F G   E
       *   F   G   B   F
       */
      return rotate_left(node->left);
    }
    // Just rotate right; (2) above.
    return rotate_right(node);
  } else if (balance < -1) {
    // the right branch is too heavy; Do the opposite of above
    if (get_balance(node->right) > 0) {
      return rotate_right(node->right);
    }
    return rotate_left(node);
  } else {
    return node;
  }
}
static void print_set_node(_s_interval_set_node *node, int idx) {
  int i;
  if (node) {
    for (i = 0; i < idx; i++) {
      printf(".");
    }

    print_set_node(node->right, idx + 1);
    print_interval(node->val);
    print_set_node(node->left, idx + 1);
  }
}

static void set_add(interval_set *set, s_interval *interval) {

  set->head = insert(set->head, interval, 0);
}

s_interval *parse_line(char *line) {
  char *split_ptr = strchr(line, SPLIT_CHAR);

  int split_idx = split_ptr - line;
  line[split_idx] = '\0';

  s_interval *i0 = malloc(sizeof(s_interval));
  i0->from = atoi(line);
  i0->to = atoi(split_ptr + 1);
  return i0;
}

int main() {
  FILE *f_in = fopen("./data/intervals1.dat", "r");

  char buf[100] = {'\0'};
  char *line;

  interval_set *set = new_set();

  do {
    line = fgets(buf, 100, f_in);

    if (line) {
      s_interval *i0 = parse_line(line);
      set_add(set, i0);
    }

  } while (line);
  // print_set_node(set->head, 0);

  fclose(f_in);
  return 0;
}
