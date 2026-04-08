/*
 * Note that there are a lot of places in this code that could be
 * improved with proper memory ordering. Some of these are noted in
 * commented-out code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

struct node {
    int data;
    struct node *next;
};

struct stack {
    _Atomic(struct node *) top;
};

/**
 * Construct a new stack
 */
struct stack *stack_new(void)
{
    struct stack *s = malloc(sizeof *s);
    s->top = NULL;

    return s;
}

/**
 * Construct a new node
 */
struct node *node_new(int data)
{
    struct node *node = malloc(sizeof *node);
    node->data = data;
    node->next = NULL;

    return node;
}

/**
 * Push onto the stack
 */
void push(struct stack *s, int val)
{
    struct node *new = node_new(val);
    struct node *old_top = s->top;

    new->next = old_top;

    while(atomic_compare_exchange_weak(&(s->top), &old_top, new))
        new->next = old_top;
}

/**
 * Pop off stack
 *
 * Returns false on failure, true on success.
 */
int pop(struct stack *s, int *value)
{
    // TODO for challenge Part 2
    struct node *old_top = s->top;
    if (old_top == NULL)
        return 0;

    while(!atomic_compare_exchange_weak(&(s->top), &old_top, old_top->next));

    *value = old_top->data;

    free(old_top);
    return 1;
}

/**
 * Print the stack (not threadsafe)
 */
void print_stack(struct stack *s)
{
    struct node *p = s->top;

    printf("Stack (top):%s", p == NULL? " [empty]": "");

    while (p != NULL) {
        printf(" %d", p->data);
        p = p->next;
    }

    putchar('\n');
}

int main(void)
{
    struct stack *s = stack_new();

    print_stack(s);

    for (int i = 0; i < 4; i++) {
        push(s, i);
        print_stack(s);
  }

    int val;

    while (pop(s, &val)) {
        printf("Popped %d\n", val);
        print_stack(s);
    }
}

// 1. The ABA problem can be counteracted by preventing the reuse of memory while it is being used,
// or tagging pointers that are in-use.
// 2. It can be prevented by atomically marking the pointers as hazardous before dereferencing.