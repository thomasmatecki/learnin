/*
 * The Algorithm Cookbook, Steven S. Skiena
 * 1.3: An optimal Scheduling Algorithm
 *
 * c is hard
 */
#include "avl_set.c"

void static optimal_schedule(){}

int main(int argc, char *argv[]) {

  interval_set *set = new_set();

  FILE *f_in = fopen("./data/intervals1.dat", "r");

  char buf[100] = {'\0'};
  char *line;

  do {
    line = fgets(buf, 100, f_in);

    if (line) {
      s_interval *i0 = parse_line(line);
      set_add(set, i0);
    }

  } while (line);
  fclose(f_in);


  return 0;
}
