#include <stdio.h>
#include <string.h>
#include <assert.h>

void swap(char * a, char * b){
  char temp = *a;
  *a = *b;
  *b = temp;
}

void isort(char s[]){
  int n = strlen(s);

  int i,j;

  for (i=1;i<n;i++){
    j=i;

    while((j>0) && (s[j] < s[j-1])){
      //printf("%s\n", s);
      swap(&s[j], &s[j-1]);
      j-=1;
    }
  }
}

int main(){
  printf("\033[1;32m");

  char str[] = "The quick brown fox jumped over the lazy dog";
  char c = 'a';
  printf("The character is: %c\n", c);
  printf("The string is: %s\n", str);
  printf("the length of the string is %zu\n", strlen(str));
  printf("The 10th character in the string is: %c\n", str[10]);

  char abc[] = "abc";
  swap(&abc[0], &abc[1]);

  assert(strcmp(abc,"bac") == 0);

  isort(str);
  return 0;
}



