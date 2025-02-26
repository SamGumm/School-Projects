#include <stdlib.h>
#include <stdio.h>

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

    while(*r++ = *d++);
    printf("t: %s\ns: %s\n", t, s);

}

void func2a() {
    
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

    

    return 0;
}