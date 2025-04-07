#ifndef SLL_H
#define SLL_H
//structures
typedef struct node node_t;
struct node {
    int data;
    node_t *next;
};


//methods
void push(node_t **stack, int data);
void pop(node_t **stack);




#endif