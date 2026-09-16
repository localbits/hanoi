#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"

typedef struct {
    u32 capacity;
    u32 amount;
    s32* items;
} Stack;

Stack* create_stack(u32 capacity);
int get_stack_top(Stack* stack);
int pop_stack(Stack* stack);
bool is_stack_empty(Stack* stack);
bool is_stack_full(Stack* stack);
bool push_stack(Stack* stack, s32 element);
void empty_stack(Stack* stack);
void free_stack(Stack* stack);

#endif // STACK_H_
