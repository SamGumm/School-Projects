#include <iostream>
#include <cstdio> //using c stdio here
using namespace std; //using this in practice might cause errors


class A {
    private:
        int x = 10;
        int y = 11;

    public:
    int incAndRet() { //method dispatch works differently
        return ++x;
    }
};

class B : public A {

};



int main() 
{
    cout << "Hello, World!" << endl;
    cout << "This code is compiled with G++ version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << endl;
    //A a1 = new A(); <--- needs a pointer, because it carves out from the heap
    A *a1 = new A();
    a1->incAndRet();
    delete a1; //<--- like free()

    
    return 0;
}