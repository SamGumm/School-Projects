#ifndef DLL_H
#define DLL_H

//Structures
typedef struct node_t NODE;
typedef struct node_t *NODEPTR;
typedef struct list_t LIST;
typedef struct list_t *LISTPTR;
struct node_t {
    void *data; //pointer to the type a user provides
    NODEPTR next; //pointer to the next node connected to this node
    NODEPTR prev; //pointer to the previous node connected to this node
};
struct list_t {
    NODEPTR head; //pointer to the first node in the list
    int numItems; //count of items in the list, manually set
};


//methods
int add(LISTPTR lst, void *item);
void *get(LISTPTR lst, int index);
void *removeNode(LISTPTR lst, int index);
int empty(LISTPTR lst);
void clear(LISTPTR lst);
int deleteList(LISTPTR lst);
void printList(LISTPTR lst);


#endif