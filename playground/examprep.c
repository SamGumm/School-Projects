#include <stdlib.h>
#include <stdio.h>
//Write a function that takes a pointer of type void * 
//and an integer n representing the number of bytes, 
//and prints the hexadecimal values of those n bytes. For example:

void printBytes(void *ptr, size_t n) {
    unsigned char *p = (unsigned char *)ptr;
    for(size_t i = 0; i < n; i++) {
        printf("%02x ", p[i]);
    }
    printf("\n");
};

int main() {
    int a = 20;
    float b = 20.1;
    typedef struct node {
        int size;
        void *datum;
    } node_t;
    node_t c = {10, NULL};
    size_t n;
    n = 40;
    void *ptr = &a;
    printBytes(ptr, sizeof(a));
    printBytes(ptr, sizeof(a));
    ptr = &b;
    printBytes(ptr, sizeof(b));
    ptr = &c;
    printBytes(ptr, sizeof(c));
}