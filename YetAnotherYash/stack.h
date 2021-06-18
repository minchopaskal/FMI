#ifndef __YAY_LIST_H
#define __YAY_LIST_H

#include <stddef.h>
#include <sys/types.h>

#ifndef STACK_DATA
#define STACK_DATA pid_t
#endif

#define data STACK_DATA

typedef struct stack_t {
  data *arr;
  unsigned ptr;
  unsigned max;
} stack;

int init_stack(stack*);

int empty(const stack*);
size_t stack_size(const stack*);
int top(const stack*, data*);
int pop(stack*, data*);
int push(stack*, data);
int get_nth(const stack*, data*, size_t); /* zero-based */

#endif
