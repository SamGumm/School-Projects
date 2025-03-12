#include <stdio.h>



float plus(float a, float b) {return a + b;}
float minus(float a, float b) {return a - b;}

float evaluate(float a, float b, char op) {
    float result;
    switch(op) {
        case '+':
            result = plus(a,b);
            break;
        case '-':
            result = minus(a,b);
            break;
        return result;
    }
    return -1;
}

//                      pointer to an array of chars
//                      |
int main(int argc, char *argv[]) {
    
    float (*func)(float, float);
    func = &plus;
    float x = evaluate(2.0,3.0,*func);
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
}