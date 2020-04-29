
#include <assert.h>
#include <limits.h>
#include "../avl_set.c"

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

static void test_set_pop_min_add_and_remove(){
  interval_set *set = new_set();

  s_interval *i0 = new_interval(0, 3);
  s_interval *i1 = new_interval(0, 4);
  s_interval *i2 = new_interval(0, 2);
  s_interval *i3 = new_interval(0, 1);

  set_add(set, i0);
  set_add(set, i1);
  set_add(set, i2);
  assert(i2 == set_pop_min(set));
  set_add(set, i2);
  set_add(set, i3);

  assert(i3 == set_pop_min(set));
  assert(i2 == set_pop_min(set));
  assert(i0 == set_pop_min(set));
  assert(i1 == set_pop_min(set));
  
  assert(set->head == NULL);
}

static void test_set_remains_balanced(){
  interval_set *set = new_set();

  for (int i = 1; i < 512; i++){
    set_add(set, new_interval(0, i));
  }

  assert(set->head->height == 9);

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
  test_set_pop_min_add_and_remove();
  test_set_remains_balanced();

  return 0;
}