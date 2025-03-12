#include <stdlib.h>
#include <stdio.h>
#include "intro.h"



void func1a() {
    //int v1; compiler does not like that v1 is unitialized like this, opens up error possibilities
    int v1 = 0;
    int v2 = 0;
    char c;
    char name[5];
    int *p;
    p = &v1;
    //printf("%d\n%d\n", p, *p); this is incorrect because compiler expects %ls to print p
    //                              but when given %ls it prints nothing (figure out why?)
    printf("Printing values with %%d: %d (this prints whatever is in the stack)| %d\n", p, *p);
    printf("Printing values with %%ls and %%d: %ls | %d\n", p, *p);
}

void func1b() {
    char t[4];
    char *p;
    char *r;
    char **q; //pointer to a pointer to a char
    t[0] = 'x';
    t[1] = 'y';
    t[2] = 'z';
    t[3] = '\0';
    p = &t[0]; //pointing to the address at the start of t
    r = &t[0];
    printf("%s\n%s\n", p, r); /*this prints the array, because the compiler 
                        knows the size of the array and how far to go*/
    printf("%s\n", t); /*since t acts as a pointer (in this scenario) to the start of the array
                        just like how p is acting, they will give the same output
                        this is also referred to as t 'decays' to a pointer*/
    *p++ = t[0]; /*dereferences p, and sets the value stored there to what t[0] contains. then, with the
                    post increment, it advances p to t[1]. Because p is a pointer, changes to what it contains
                    also affects t. When p is initially set to t[0], you are essentially doing 
                    t[0] = t[0]. */
    *r++ = t[2]; /*Dereferences r (also initially at t[0]) and assigns it the value t[2] (which is 'z'). 
                    This overwrites t[0], changing it from 'x' to 'z'.
                    After the assignment, the pointer r is incremented to point to t[1].t*/
    printf("%s\n%s\n", p, r); //prints 'yz' for both because they both now point to the same place
    printf("%s\n", t); //prints the array

    *p++ = '\0';

    q = &p; //sets q to point to the address of p
    printf("%s\n", *q++); /*dereferences q, then post increments to have q point to
                            (essentially) p[1]. It just jumps forward in memory by the size of the type
                            of the pointer. Since it is an int, it moves forward by 4.*/
}

void func1c() {
    char t[4];
    t[0] = 'x';
    t[1] = 'y';
    t[2] = 'z';
    t[3] = '\0';
    char s[4];

    char *d = &t[0];
    char *r = &s[0];

    //WRONG IMPLEMENTATION
    // while(*d++ = *r++) { /*This is copying from s into t because d points to the beginning of t, and
    //                         r points to the beginning of s. Therefore it is essentially 
    //                         d[i] = r[i]. The post increment allows for copying the current char in s
    //                         to t, then moves on. s is non-initialized, so when accessing it it is unknown
    //                         what exactly is occupying the memory segment in the stack.*/
    //     printf("%s", d);
    //     *r = *d; //this then copies t back into s
    // };

    while((*r++ = *d++));
    printf("t: %s\ns: %s\n", t, s);

}

void func2a() {
    struct point p1;
    point_3d p2;
    
    p1.x = 1;
    p1.y = 2;
    p2.x = 3;
    p2.y = 3;
    p2.z = 3;
    p3.x = 4;
    p3.y = 4;

    printf("%d %d\n", p1.x, p1.y);
    printf("%d %d %d\n", p2.x, p2.y, p2.z);
    printf("%d %d\n", p3.x, p3.y);

    point_3d *ppoint = &p2;
    point_3d **pppoint = &ppoint;
    printf("use (*ppoint).<var_name> to access the struct that ppoint is pointing to; i.e. x, y, z: %d %d %d\n", (*ppoint).x, (*ppoint).y, (*ppoint).z);
    printf("pppoint points to address: %p\nAddress of ppoint itself is: %p\n", *(&pppoint), &ppoint);
    printf("(*pppoint) (%p) is the value of ppoint, which pppoint points to.\n", (*pppoint));
    printf("*(&pppoint) (%p) gives the address of what pppoint is pointing instead\n", *(&pppoint));

    struct point origin = {0,0};
    struct point *ppoint2;
    ppoint2 = &origin;
    printf("%d %d\n", ppoint2 -> x, ppoint2 -> y);    

}

struct point addpoint(struct point p1, struct point p2) {
    p1.x += p2.x;
    p1.y += p2.y;
    return p1; //-> copies the changed local variable into an outside variable
}

void func2b() {
    struct point base = {0,0};
    struct point destination = {10,10};
    struct point position = base;
    printf("pre-movement\nbase: (%d, %d)\ndestination: (%d, %d)\nposition: (%d, %d)\n", 
        base.x, base.y, destination.x, destination.y, position.x, position.y);
    position = addpoint(base, destination); //copying result into position
    printf("post-movement\nposition: (%d, %d)\n", position.x, position.y);
}

void func3a() {
    int *p = malloc(sizeof(int));
    *p = 5;
    printf("address of p: %p\n", p);
    p = malloc(sizeof(int)); //sets p to point to somewhere else
    *p = 7; //after the 2nd malloc and this, the address of 5 is forever lost. This is a memory leak.
    printf("address of p: %p  <---changed because malloc was called again\n", p);
}

void func3b() {
    int a = 10;
    int b = 20;
    int c;
    printf("Using macros:\n");
    printf("the max between %d and %d is: %d\n", a, b, max(a,b));
    printf("the min between %d and %d is: %d\n", a, b, min(a,b));
}

void push(node_t **s, int d) {//need a pointer to a pointer because s points to the current pointer that points to the top of the list
                                //otherwise the changes are purely local
    node_t *node = malloc(sizeof(node_t)); //create space on the heap for the new node
    if(!node) {
        fprintf(stderr, "Failed to allocate space for node");
    }
    node -> data = d; //fill the node's data with d
    node -> next = *s; //have the node point to the beginning of the list (top of the stack)
    *s = node; //updates the stack's head pointer to point to the new node, placing it at the top of the stack.
}

int pop(node_t **s) {
    int toreturn;
    if(!*s) {
        fprintf(stderr, "stack underflow");
        return 0;
    }
    toreturn = (*s) -> data;
    node_t *temp = *s; //stores the address of the current top of the list, to be freed later
    *s = (*s) -> next; //s is incremented to the next node on the list
    free(temp); //the previous top of the list is freed using the address stored in temp, to stop memory leaks
    return toreturn; //the data from the popped node is returned
}

void printStack(node_t *s) {
    node_t *current = s;
    while(current) {
        printf("%d\n", current->data);
        current = current -> next;
    }
}

node_t *createStack() {
    return NULL;
}


int main() {
    system("clear");

    printf("func1a()\n");
    func1a();

    printf("\nfunc1b()\n");
    func1b();

    printf("\nfunc1c()\n");
    func1c();
    
    printf("\nfunc2a()\n");
    func2a();

    printf("\nfunc2b()\n");
    func2b();

    printf("\nfunc3a()\n");
    func3a();
    
    printf("\nfunc3b()\n");
    func3b();

    return 0;
}