#include <math.h>
#include <stdio.h>

struct Node {
  int val;
  struct Node *next;
};

typedef struct Node t_node;

typedef struct Point {
  int x;
  int y;
} t_point;


double distance(t_point *p1, t_point *p2) {
  int x_d = p1->x - p2->x;
  int y_d = p1->y - p2->y;
  return sqrt((double)(x_d * x_d) + (y_d * y_d));
}

int main() {
  t_point p0 = {1, 2};
  t_point p1 = {3, 3};
  register int i;

  for (i = 1; i < 10; i++) {
    printf("%d", i);
  }

  double d0 = distance(&p0, &p1);

  printf("x: %d\n", p0.x);

  t_node n0 = {1, NULL};
  printf("x: %d\n", n0.val);
  t_node n1 = {1, &n0};
  t_node n2 = {1, &n1};

  t_node n3 = {1, &n0};

  printf("null: %x", NULL);
  t_node *np = &n2;

  printf("%d", np->val);

  return 0;
}
