#include "stack.h"

Stack* create_stack(u32 capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    assert(stack != NULL);

    i32* items = (i32*)malloc(sizeof(i32) * capacity);
    assert(items != NULL);

    stack->capacity = capacity;
    stack->amount = 0;
    stack->items = items;

    return stack;
}

int get_stack_top(Stack* stack)
{
    return stack->items[stack->amount - 1];
}

int pop_stack(Stack* stack)
{
    int top = get_stack_top(stack);
    stack->amount--;

    return top;
}

bool is_stack_empty(Stack* stack)
{
    return stack->amount == 0;
}

bool is_stack_full(Stack* stack)
{
    return stack->amount >= stack->capacity;
}

bool push_stack(Stack* stack, int element)
{
    if (stack->amount >= stack->capacity) {
        return false;
    }

    stack->items[stack->amount++] = element;
    return true;
}

void empty_stack(Stack* stack)
{
    stack->amount = 0;
}

void free_stack(Stack* stack)
{
    free(stack->items);
    stack->items = NULL;
    free(stack);
    stack = NULL;
}
