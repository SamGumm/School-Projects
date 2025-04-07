#include <iostream>

void printThis(char* a) {
    for (int i = 0; a[i] != '\0'; i++) {
        std::cout << a[i] << std::ends;    
    };
};

int main() {
    std::cout << "Hello World!" << std::endl;
    bool flag = false;
    if(!flag) std::cout << "flag was false" << std::endl;
    char lines[] = {'t', 'h', 'i', 's', ' ', 'w', 'o', 'r', 'k', 'e', 'd', '\n','\0'};
    printThis(lines);
}