#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define INIT_SIZE 100

int init_stack(stack *s) {
  s->ptr = 0;
  s->max = INIT_SIZE;

  s->arr = (data*) calloc(s->max, sizeof(data));
  if (!s->arr) {
    return -1;
  }
}

int empty(const stack *s) {
  return s->ptr == 0;
}

size_t stack_size(const stack *s) {
  return s->ptr;
}

int top(const stack *s, data *val) {
  if (empty(s)) return -1;
  *val = s->arr[s->ptr - 1];
  return 0;
}

int pop(stack *s, data *val) {
  if (empty(s)) return -1;
  *val = s->arr[s->ptr - 1];
  --s->ptr;
  return 0;
}

int push(stack *s, data val) {
  if (s->ptr == s->max) {
    s->max *= 2;
    s->arr = (data*) reallocarray(s->arr, s->max, sizeof(data));

    if (!s->arr) {
      perror("stack push");
      return -1;
    }
  }
  
  s->arr[s->ptr++] = val;
  return 0;
}

int get_nth(const stack *s, data *val, size_t n) {
  if (n >= stack_size(s)) return -1;
  *val = s->arr[n];
  return 0;
}
