#include <stdlib.h>
#include <stdio.h>
#include "dll.h"

/**
 * Adds a user-defined type of data to the end of a circular doubly-linked list.
 * Returns 1 on success and -1 on failure.
 */
int add(LISTPTR lst, void *item) {
    NODEPTR p = malloc(sizeof(NODE)); //new node is allocated space
    if(!p) {
        return -1; //error: allocation failure
    }
    p -> data = item;

    //insert p before the head (lst->head)
    p -> prev = lst -> head -> prev;
    p -> next = lst -> head;
    lst -> head -> prev -> next = p;
    lst -> head -> prev = p;

    //update the count
    lst->numItems++;
    return 1; //success
};

/** 
 * Returns the data of the node at the given index
 * If it cannot find the index, returns NULL.
 */
void *get(LISTPTR lst, int index) {
    int count = 0; //Indicates where in the list the current pointer is at.
    NODEPTR current = lst -> head -> next; //pointer to current memory position in list, initialized to the first non-sentinel node
    while (current != lst -> head) { //stops when reaching the sentinel node
        if(count == index) {
            return current -> data;
        }
        current = current->next;
        count++;
    }
    return NULL;
};

/**
 * Removes a node at the given index,
 * Then returns a pointer to the address of the data 
 * contained in the removed node.
 */ 
void *removeNode(LISTPTR lst, int index) {
    int count = 0; //Indicates where in the list the current pointer is at.
    NODEPTR current = lst -> head; //pointer to current memory position in list.
    while (current) {
        if (count == index) {
            // Found the node to remove.
            // If there's a previous node, update its next pointer.
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                // Removing the head node: update the list's head pointer.
                lst->head = current->next;
            }
            // If there's a next node, update its prev pointer.
            if (current->next) {
                current->next->prev = current->prev;
            }
            // Retrieve the data before freeing.
            void *data = current->data;
            free(current);
            lst->numItems--;
            return data;  // Return the removed node's data.
        }
        current = current->next;
        count++;
    }
    // Index out of bounds.
    return NULL;
}


//Checks whether the given list is empty.
int empty(LISTPTR lst) {
    if(lst -> head == NULL) {
        return 1;
    }
    return (lst -> numItems == 0);
};

//Resets the given list.
void clear(LISTPTR lst) {
    if (!lst || !lst->head) return;
    NODEPTR sentinel = lst -> head;
    NODEPTR current = lst -> head -> next;
    while (current != sentinel) {
        NODEPTR next = current -> next;
        free(current);
        current = next;
    }
    free(sentinel);
    lst->head = NULL;
    lst -> numItems = 0;
};

//Frees the list from memory by iterating through all nodes, freeing them, and then freeing the list itself.
//Returns 1 on success, 0 on failure
int deleteList(LISTPTR lst) {
    if(lst) {
        clear(lst);
        free(lst);
        return 1;
    }
    return 0;
};

void printListInt(LISTPTR lst) {
    // for(int i = 0; i < lst -> numItems; i++) {
    //     //this calls get, which returns a void pointer to the data of the node grabbed, (get(lst, i))
    //     //then casts it to an int pointer (int *),
    //     //then dereferences the int pointer to retrieve the actual data -->*((code))
    //     printf("%d\n", *((int *)get(lst, i)));
    // }
    NODEPTR current = lst -> head -> next;
    int count = 0;
    while(current != lst -> head && count < lst -> numItems) {
        printf("%d\n", *((int *)get(lst, count)));
        count++;
    }
};

void printListChar(LISTPTR lst) {
    NODEPTR current = lst -> head -> next;
    int count = 0;
    while(current != lst -> head && count < lst -> numItems) {
        int asciiValue = *((char *)get(lst, count));
        char asciiChar = asciiValue;
        printf("%d %c\n", asciiValue, asciiChar);
        count++;
    }
};

//Creates a Doubly-Linked Circular List with a Head Node containing NULL data.
LISTPTR createList() {
    LISTPTR createdList = malloc(sizeof(LIST));
    if(!createdList) {
        perror("Failed to allocate list");
        exit(EXIT_FAILURE);
    }
    //make initial head node for dll
    NODEPTR headNode = malloc(sizeof(NODE));
    //fill headNode
    headNode -> data = NULL;
    headNode -> next = headNode;
    headNode -> prev = headNode;
    //insert into the createdList
    createdList -> head = headNode;
    createdList -> numItems = 0;
    return createdList;
};

int main () {
    //initializing my own list with createList
    LISTPTR myIntList = createList();
    LISTPTR myCharList = createList();
    //Array to be used to fill myList
    int arrI[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    char arrC[11] = {'a', 'b', 'c', 'd','e','f','g','h','i','j'};
    //adding initial 4 elements to the list
    for(int i = 0; i < 4; i++) {
        add(myIntList, &arrI[i]);//passing the address, because myList node data are pointers
        add(myCharList, &arrC[i]);
    }
    printListInt(myIntList); //prints first 4 elements
    printListChar(myCharList);
    add(myIntList, &arrI[4]);
    removeNode(myIntList, 4);
    printListInt(myIntList);
    printf("Is the list empty? [1 = yes][0 = no]: %d\n", empty(myIntList));
    printf("clear(myList) has been called\n");
    clear(myIntList);
    printf("Is the list empty? [1 = yes][0 = no]: %d\n", empty(myIntList));
};