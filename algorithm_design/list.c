#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SNODE(T)                                                               \
  typedef struct _SNode_##T##_ {                                               \
    T val;                                                                     \
    struct _SNode_##T##_ *next;                                                \
  }

#define SLIST(T)                                                               \
  SNODE(T) n_##T;                                                              \
                                                                               \
  typedef struct _SList_##T##_ {                                               \
    struct _SNode_##T##_ *head;                                                \
  }

SLIST(char) ls_char;

static void print_char_list(ls_char *ls) {

  n_char *cursor = NULL;
  if (ls->head) {
    cursor = ls->head;
  }

  while (cursor) {
    printf("%c\n", cursor->val);
    cursor = cursor->next;
  }
}

void append(ls_char *ls, char c) {

  n_char *node = malloc(sizeof(n_char));
  node->val = c;

  if (ls->head) {
    node->next = ls->head;
    ls->head = node;
  } else {
    node->next = NULL;
  }

  ls->head = node;
}

bool contains(ls_char *ls, char c) {
  n_char *cursor = NULL;

  if (ls->head) {
    cursor = ls->head;
  }

  while (cursor) {
    if (cursor->val == c){
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

int main(int argc, char *argv[]) {

  ls_char *xs = malloc(sizeof(struct _SList_char_));
  xs->head = NULL;

  append(xs, 'a');
  append(xs, 'b');
  append(xs, 'c');
  append(xs, 'd');
  append(xs, 'e');
  append(xs, 'f');
  append(xs, 'g');

  // printf("iterating over our list... \n");

  // print_char_list(xs);
  // print_char_list(xs);
  assert(contains(xs, 'e') == true);
  assert(contains(xs, 'z') == false);
}
