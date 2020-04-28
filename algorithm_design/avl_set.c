/*
 * An AVL set of integer intervals
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#define CHECKED_HEIGHT(node_symb) (node_symb ? node_symb->height : 0)
#define MODE_NORMAL 0
#define MODE_TEST 1

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

typedef struct BackTracedNode {
  _s_interval_set_node *node;
  int depth;
} _s_node_depth;

/*
 *
 */
static void print_interval(s_interval *interval) {
  printf("%d ... %d", interval->from, interval->to);
}

/*
 *
 */
int max_height(_s_interval_set_node *node_a, _s_interval_set_node *node_b) {
  int a_height = CHECKED_HEIGHT(node_a);
  int b_height = CHECKED_HEIGHT(node_b);

  if (a_height > b_height) {
    return a_height;
  } else {
    return b_height;
  }
}

/*
 *
 */
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

static _s_interval_set_node *balance_tree(_s_interval_set_node *node) {
  // Increment the height
  node->height = 1 + max_height(node->right, node->left);

  // Rebalance the tree
  int balance = get_balance(node);

  if (balance > 1) {
    // The left branch is too heavy
    if (get_balance(node->left) < 0) {

      /* Left/Right is heavier than Left/Left,
       * (1) Rotate A(first left) to the left.
       * (2) Rotate C(the root) to the right.
       *     C           C           D
       *    / \         / \         / \
       *   A   E       D   E       /   \
       *  / \     =>  / \    =>   A     C
       * B   D       A   G       / \   / \
       *    / \     / \         B   F G   E
       *   F   G   B   F
       */
      node->left = rotate_left(node->left);
    }

    // Left/Left is heavier than Left/Right; Just rotate right - (2) above.
    return rotate_right(node);

  } else if (balance < -1) {
    // the right branch is too heavy; Do the opposite of above
    if (get_balance(node->right) > 0) {
      node->right = rotate_right(node->right);
    }
    return rotate_left(node);
  } else {
    // The tree is balanced
    return node;
  }
}

/*
 * Recursively traverse `node` and inserts `interval` at the
 * appropriate position.
 */
static _s_interval_set_node *insert(_s_interval_set_node *node,
                                    s_interval *interval, int d) {

  // Recursively descend and insert a leaf in the tree.
  if (!node) {
    node = malloc(sizeof(_s_interval_set_node));
    node->val = interval;
    node->left = NULL;
    node->right = NULL;
    /* Sort the tree using interval `to` date; this is the
     * 'completion' of the interval in the optimal scheduling
     * algorithm.
     */
  } else if (node->val->to >= interval->to) {
    node->left = insert(node->left, interval, d + 1);
  } else {
    node->right = insert(node->right, interval, d + 1);
  }

  return balance_tree(node);
}

/*
 * Recursively descend and remove the minimum element from
 * `node`. set popped_node_depth->node to the removed node
 * and return the new root node.
 */
static _s_interval_set_node *pop_left(
  _s_interval_set_node *node,
  _s_node_depth *popped_node_depth
) {

  if (node && node->left) {
    node->left = pop_left(node->left, popped_node_depth);
    if (popped_node_depth->depth == 0) {

      if (popped_node_depth->node->right) {
        /* with C as `node`, if popping A
         *     C           C
         *    / \         / \
         *   A   E       D   E
         *    \     =>
         *     D
         */
        node->left = popped_node_depth->node->right;
        node->left->height = 1;
      } else {
        /* with C as `node`, if popping D
         *    C
         *   / \
         *  D   E
         *
         */
        node->left = NULL;
      }
      node->height = max_height(node->left, node->right);
    } else {
      // While backtracing, re-balance tree.
      node->left = balance_tree(node->left);
    }

    popped_node_depth->depth++;
    return node;
  } else {
    popped_node_depth->node = node;
    if (node){
      return node->right;
    }
    return NULL;
  }
}

/*
 *
 */
static void set_add(interval_set *set, s_interval *interval) {
  set->head = insert(set->head, interval, 0);
}

/*
 *
 */
static s_interval *set_pop_min(interval_set *set) {

  _s_node_depth popped_node = {NULL, 0};
  set->head = pop_left(set->head, &popped_node);

  // if (popped_node.depth == 0) {
  //   set->head = NULL;
  // }

  if (!popped_node.node) {
    return NULL;
  } else {
    _s_interval_set_node *min_node = popped_node.node;
    s_interval *ret = min_node->val;
    return ret;
  }

}

static s_interval *new_interval(int from, int to) {
  s_interval *i0 = malloc(sizeof(s_interval));

  i0->from = from;
  i0->to = to;
  return i0;
}

static void test_set_add_inserts_interval_at_root(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(0, 3);
  set_add(set, i0);

  assert(set->head != NULL);
  assert(set->head->val == i0);
  assert(set->head->left == NULL);
  assert(set->head->right == NULL);
  assert(set->head->height == 1);
}

static void test_set_add_inserts_min_interval_at_left(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(1, 3);
  s_interval *i1 = new_interval(1, 2);

  set_add(set, i0);
  set_add(set, i1);

  assert(set->head != NULL);
  assert(set->head->val == i0);
  assert(set->head->left != NULL);
  assert(set->head->right == NULL);
  // Inserted Left
  assert(set->head->left->val == i1);
  assert(set->head->left->left == NULL);
  assert(set->head->left->right == NULL);
  assert(set->head->height == 2);
}

static void test_set_add_inserts_max_interval_at_right(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(1, 2);
  s_interval *i1 = new_interval(1, 3);

  set_add(set, i0);
  set_add(set, i1);

  assert(set->head != NULL);
  assert(set->head->val == i0);
  assert(set->head->right != NULL);
  assert(set->head->left == NULL);
  // Inserted Right
  assert(set->head->right->val == i1);
  assert(set->head->right->left == NULL);
  assert(set->head->right->right == NULL);
  assert(set->head->height == 2);
}

static void test_set_add_rotates_right_if_min_inserted_twice(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(0, 3);
  s_interval *i1 = new_interval(0, 2);
  s_interval *i2 = new_interval(0, 1);

  set_add(set, i0);
  set_add(set, i1);
  set_add(set, i2);

  assert(set->head != NULL);
  assert(set->head->right != NULL);
  assert(set->head->left != NULL);

  assert(i1 == set->head->val);
  assert(i2 == set->head->left->val);
  assert(i0 == set->head->right->val);

  assert(set->head->height == 2);
  assert(set->head->left->height == 1);
  assert(set->head->right->height == 1);

  assert(set->head->right->left == NULL);
  assert(set->head->right->left == NULL);
  assert(set->head->left->right == NULL);
  assert(set->head->left->right == NULL);
}

static void test_set_add_rotates_left_if_max_inserted_twice(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(0, 3);
  s_interval *i1 = new_interval(0, 2);
  s_interval *i2 = new_interval(0, 1);

  set_add(set, i2);
  set_add(set, i1);
  set_add(set, i0);

  assert(set->head != NULL);
  assert(set->head->right != NULL);
  assert(set->head->left != NULL);

  assert(i1 == set->head->val);
  assert(i2 == set->head->left->val);
  assert(i0 == set->head->right->val);

  assert(set->head->height == 2);
  assert(set->head->left->height == 1);
  assert(set->head->right->height == 1);

  assert(set->head->right->left == NULL);
  assert(set->head->right->left == NULL);
  assert(set->head->left->right == NULL);
  assert(set->head->left->right == NULL);
}

static void test_set_add_rotates_once_if_min_inserted_3_times(){
  interval_set *set = new_set();

  s_interval *i0 = new_interval(0, 4);
  s_interval *i1 = new_interval(0, 3);
  s_interval *i2 = new_interval(0, 2);
  s_interval *i3 = new_interval(0, 1);

  set_add(set, i0);
  set_add(set, i1);
  set_add(set, i2);
  set_add(set, i3);

  assert(i3 == set->head->left->left->val);
  assert(NULL == set->head->left->right);
}

static void test_set_add_maintains_ordering_in_nodes_if_height_1(){
  interval_set *set = new_set();

  s_interval *i0 = new_interval(0, 4);
  s_interval *i1 = new_interval(0, 3);
  s_interval *i2 = new_interval(0, 1);
  s_interval *i3 = new_interval(0, 2);

  set_add(set, i0);
  set_add(set, i1);
  set_add(set, i2);
  set_add(set, i3);

  assert(i3 == set->head->left->right->val);
  assert(NULL == set->head->left->left);
}

static void test_set_pop_min_returns_null_if_set_empty(){
  interval_set *set = new_set();
  s_interval *i0 = set_pop_min(set);
  assert(i0 == NULL);
}

static void test_set_pop_min_returns_min_node_if_height_1(){
  interval_set *set = new_set();
  s_interval *i0 = new_interval(0, 4);
  set_add(set, i0);
  s_interval *i1 = set_pop_min(set);
  assert(i1 == i0);
  assert(set->head == NULL);
}

static void test_set_pop_min_returns_min_node_if_height_2(){
  interval_set *set = new_set();

  s_interval *i1 = new_interval(0, 3);
  s_interval *i2 = new_interval(0, 1);
  s_interval *i3 = new_interval(0, 2);
  s_interval *i0;

  set_add(set, i1);
  set_add(set, i2);
  set_add(set, i3);

  i0 = set_pop_min(set);
  assert(i0 == i2);
  
  i0 = set_pop_min(set);
  assert(i0 == i3);
  
  i0 = set_pop_min(set);
  assert(i0 == i1);
  
  assert(set->head == NULL);
}


int main(int argc, char *argv[]) {

  printf("Running tests...");

  test_set_add_inserts_interval_at_root();
  test_set_add_inserts_min_interval_at_left();
  test_set_add_inserts_max_interval_at_right();
  test_set_add_rotates_right_if_min_inserted_twice();
  test_set_add_rotates_left_if_max_inserted_twice();
  test_set_add_rotates_once_if_min_inserted_3_times();
  test_set_add_maintains_ordering_in_nodes_if_height_1();
  test_set_pop_min_returns_null_if_set_empty();
  test_set_pop_min_returns_min_node_if_height_1();
  test_set_pop_min_returns_min_node_if_height_2();

  return 0;
}
