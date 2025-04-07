#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

// int checkIfNull(void **ptr) {
//     if(!*ptr) {
//         fprintf(stderr, "Pointer is null.\n");
//         return -1;
//     }
//     return 1;
// }

int checkIfNull(void *ptr) {
    if(!ptr) {
        fprintf(stderr, "Pointer is null.\n");
        return -1;
    }
    return 1;
}

// node_t checkIfNull(node_t **ptr) {
//     if(!*ptr) {
//         fprintf(stderr, "Pointer is null.\n");
//         return;
//     }
// }

void push(node_t **stack, int data) {
    node_t *newNode = malloc(sizeof(node_t));
    if(checkIfNull(newNode) < 0) {
        return;
    }; //passes in the address of the pointer, which is then cast to a void pointer-to-pointer
    newNode -> data = data; //fill in the newNode's data
    newNode -> next = *stack; //pointing the newNode's next to the top of the stack
    *stack = newNode; //update the stack pointer to point to the newNode
}

void pop(node_t **stack) {
    node_t *temp = *stack;
    if(checkIfNull(temp) < 0) {
        return;
    };
    *stack = (*stack) -> next;
    free(temp);
}

void printStack(node_t *s) {
    node_t *current = s;
    checkIfNull(current);
    while(current) {
        printf("%d\n", current->data);
        current = current -> next;
    }
}


// void push(node_t **s, int d) {//need a pointer to a pointer because s points to the current pointer that points to the top of the list
//                                 //otherwise the changes are purely local
//     node_t *node = malloc(sizeof(node_t)); //create space on the heap for the new node
//     if(!node) {
//         fprintf(stderr, "Failed to allocate space for node");
//     }
//     node -> data = d; //fill the node's data with d
//     node -> next = *s; //have the node point to the beginning of the list (top of the stack)
//     *s = node; //updates the stack's head pointer to point to the new node, placing it at the top of the stack.
// }

// int pop(node_t **s) {
//     int toreturn;
//     if(!*s) {
//         fprintf(stderr, "stack underflow");
//         return 0;
//     }
//     toreturn = (*s) -> data;
//     node_t *temp = *s; //stores the address of the current top of the list, to be freed later
//     *s = (*s) -> next; //s is incremented to the next node on the list
/*
(*stack) dereferences stack once to yield a node_t * (the pointer to the top node).
(*stack)->next accesses the next field of the node pointed to by *stack.
*/
//     free(temp); //the previous top of the list is freed using the address stored in temp, to stop memory leaks
//     return toreturn; //the data from the popped node is returned
// }

// void printStack(node_t *s) {
//     node_t *current = s;
//     while(current) {
//         printf("%d\n", current->data);
//         current = current -> next;
//     }
// }

// node_t *createStack() {
//     return NULL;
// }

int main() {
    node_t *stack = NULL;
    printf("making linked list stack:\n");
    int arr[5] = {0,1,2,3,4};
    for (int i = 0; i < 5; i++) {
        push(&stack, arr[i]);
    }
    printStack(stack);
    push(&stack, 10);
    printStack(stack);
    pop(&stack);
    printStack(stack);
    for(int i = 0; i < 5; i++) {
        pop(&stack);
    }
    printStack(stack);
    pop(&stack);
    printf("Calling pop() again on an empty stack will throw a segmentation fault, unless there is error handling.\nIn this case, checkIfNull is called, and returns -1, indicating to the pop() function to return instead of attempting another pop().\nThis avoids the segmentation fault.\n");
    printf("refilling the stack...\n");
    for(int i = 0; i < 20; i++) {
        push(&stack, i);
    }
    printStack(stack);
}