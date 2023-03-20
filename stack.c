#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define cap 256

typedef struct {
    char *name;
} Variable;
typedef struct {
    Variable *content;
    int size;
} Stack;
Stack *Stack_construct() {
    Stack *stack = malloc(sizeof(Stack));
    stack->content = calloc(cap, sizeof(Variable));
    stack->size=0;
    return stack;
}

typedef struct {
    Variable *content;
    int size;
    int current;
} Queue;
Queue *Queue_construct() {
    Queue *queue = malloc(sizeof(Queue));
    queue->content = calloc(cap, sizeof(Variable));
    queue->size=0;
    queue->current=0;
    return queue;
}
void push(Stack* stack,char* key){
  stack->content[stack->size++].name = key;
}
char* pop(Stack* stack){
  return stack->content[--(stack->size)].name;
}
void enqueue(Queue* queue,char* key){
  queue->content[queue->size++].name = key;
  if(queue->size==cap){
    queue->size=0;
  } 
}
char* dequeue(Queue* queue){
  char * temp;
  temp = queue->content[queue->current++].name;
  if(queue->current==cap){
    queue->current=0;
  } 
  return temp;
}
int main() {
  Stack *s1 = Stack_construct();
  Stack *s2 = Stack_construct();
  push(s1,"hello");
  push(s1,"world");
  push(s2,"abc");
  push(s2,"word");
  printf("%s\n", pop(s1));
  printf("%s\n", pop(s1));
  printf("%s\n", pop(s2));
  printf("%s\n", pop(s2));
  Queue *q1 = Queue_construct();
  enqueue(q1,"a");
  enqueue(q1,"b");
  printf("%s\n", dequeue(q1));
  printf("%s\n", dequeue(q1));
  return 0;
}
// typedef enum{
//     TOKEN_TYPE_NUM,
//     TOKEN_TYPE_OP,
//     TOKEN_TYPE_FUNC
// }TOKENTYPE;
// struct Token{
//     char token[256];
// };

// char stack1[size][length];//first stack
// int top1=0;
// void push1(char a[]){
//     strcpy(stack1[top1],a);
//     top1++;
// }
// char* pop1() {
//   char* pop = stack1[--top1];
//   return pop;
// }

// char stack2[size][length];//second stack
// int top2=0;
// void push2(char a[]){
//     strcpy(stack2[top2],a);
//     top2++;
// }
// char* pop2(){
//   char* pop = stack2[--top2];
//   return pop;
// }

