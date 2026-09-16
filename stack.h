#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef struct {
    u32 capacity;
    u32 amount;
    i32* items;
} Stack;

Stack* create_stack(u32 capacity);
int get_stack_top(Stack* stack);
int pop_stack(Stack* stack);
bool is_stack_empty(Stack* stack);
bool is_stack_full(Stack* stack);
bool push_stack(Stack* stack, int element);
void empty_stack(Stack* stack);
void free_stack(Stack* stack);

#endif // STACK_H_
