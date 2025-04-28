#include <bits/stdc++.h>
#include "intbox.hpp"


/**
 * @param intvalue the integer to initialize the IntBox object with from the heap.
 * Constructor
 */
IntBox::IntBox(int intvalue = 0) {
    storedvalue = new int(intvalue);
}

/**
 * @param rhs the referenced object used to initialize the new object with.
 * Called when you do IntBox b = a
 * Allocates your own copy of the integer so each box manages its own memory
 */
IntBox::IntBox(const IntBox &rhs) {
//   pointer                  pointer                  
    storedvalue = new int(*(rhs.storedvalue));
}

/**
 * Destructor for IntBox objects, deletes storedvalue.
 * Frees the heap memory when the IntBox goes out of scope or is deleted.
 */
IntBox::~IntBox() {
    delete(storedvalue);
}
//    reference                  takes a reference
/**
 * @param rhs the referenced object used to initialize the new object with. 
 * @return the instance of the newly created object.
 * Handles a=b after BOTH EXIST ALREADY
 * Self-assignment check avoids deleting your own data before copying
 */
const IntBox & IntBox::operator=(const IntBox & rhs) { //understand what is going on here
    if(this != &rhs) {
        // 1. Free existing storage
        delete storedvalue;
        // 2. Allocate new storage and copy
        *storedvalue = *(rhs.storedvalue);
    }
    return *this; //takes the instance and returns a reference
}

/**
 * Displays what integer value the object contains.
 */
void IntBox::displayInternal() {
    std::cout << *(storedvalue) << std::endl;
}

/**
 * Displays the address of where the object stores its value.
 */
void IntBox::displayInternalAddress() {
    std::cout << this->storedvalue << std::endl;
}

int main() {
    IntBox box = 10;
    IntBox *boxptr;
    box.displayInternal();
    box.displayInternalAddress();
    box = 20;
    box.displayInternal();
    box.displayInternalAddress();
    IntBox box2;
    box2 = box;
    box2.displayInternal();
    box2.displayInternalAddress();
//===================================================================
    IntBox a(42);
    a.displayInternal(); //42
    a.displayInternalAddress();

    IntBox b = a; //copy-ctor
    b.displayInternal();
    b.displayInternalAddress();

    b = IntBox(99);
    b.displayInternal();

    return 0;
}