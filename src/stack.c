#include <stdlib.h>
#include "stack.h"

void stack_init(Stack *stack)
{
    stack->top = NULL;
    stack->size = 0;
}

int stack_is_empty(const Stack *stack)
{
    return stack->top == NULL;
}

int stack_push(Stack *stack, int value)
{
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));

    if (node == NULL) {
        return 0;
    }
    node->value = value;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    return 1;
}

int stack_pop(Stack *stack, int *value)
{
    StackNode *temp;

    if (stack_is_empty(stack)) {
        return 0;
    }
    temp = stack->top;
    if (value != NULL) {
        *value = temp->value;
    }
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return 1;
}

void stack_free(Stack *stack)
{
    int dummy;

    while (stack_pop(stack, &dummy)) {
    }
}
