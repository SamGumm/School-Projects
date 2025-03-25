#include <stdio.h>


//tells the compiler that funcPtr is a pointer to a function 
//that accepts two ints and returns an integer
int (*funcPtr)(int, int); 

//function that performs an operation on two integers using a function pointer
int operate(int a, int b, int (*operation)(int, int)) {
    return operation(a,b);
}

int add(int x, int y) {
    return x + y;
}

int multiply(int x, int y) {
    return x * y;
}


//                      pointer to an array of chars
//                      |
int main(int argc, char *argv[]) {
    funcPtr = add;
    int result = funcPtr(2,3);//calls add(2,3) and returns 5
    printf("%d\n", result);
    //array of function pointers [3] that take 2 ints and return an int
    int (*operations[3])(int, int) = { add, add, add};
    int a = 10, b = 5;
    for (int i = 0; i < 3; i++) {
        printf("result %d: %d\n", i, operations[i](a,b));
    } 
    // FILE *readfile;
    // if(argc > 1) {
    //     readfile = fopen(argv[1], "r");//open to read
    //     if(!readfile) {
    //         fprintf(stderr, "No such file\n");
    //         exit(-1);
    //     }
    // } else {
    //     readfile = stdin;
    // }
    // float input1;
    // float input2;
    // int r = fscanf(readfile, "%f %f", &input1, &input2);
    return 0;
}