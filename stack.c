#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 256
#define length 256
// typedef enum{
//     TOKEN_TYPE_NUM,
//     TOKEN_TYPE_OP,
//     TOKEN_TYPE_FUNC
// }TOKENTYPE;
// struct Token{
//     char token[256];
// };

char stack1[size][length];
int top1=0;
void push1(char a[]){
    strcpy(stack1[top1],a);
    top1++;
}
char* pop1() {
  char* pop = stack1[--top1];
  return pop;
}

char stack2[size][length];
int top2=0;
void push2(char a[]){
    strcpy(stack2[top2],a);
    top2++;
}
char* pop2(){
  char* pop = stack2[--top2];
  return pop;
}

int main() {
  push1("hello");
  push1("world");
  push2("abc");
  push2("world");
  printf("%s\n", pop1());
  printf("%s\n", pop1());
  printf("%s\n", pop2());
  printf("%s\n", pop2());
  return 0;
}