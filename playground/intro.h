#ifndef INTRO_H
#define INTRO_H

//structures
typedef struct node node_t;
struct node {
    int data;
    node_t *next;
};

struct point {
    int x;
    int y;
};
typedef struct {
    int x;
    int y;
    int z;
} point_3d;
struct {
    int x;
    int y;
} p3;




//macros
//    <func_name> <func_operation> : <func_result>
#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))

//functions
void func1a();
void func1b();
void func1c();
void func2a();
void func2b();
void func3a();
void push(node_t **s, int d);
int pop(node_t **s);

//helper methods
struct point addpoint(struct point p1, struct point p2);

#endif