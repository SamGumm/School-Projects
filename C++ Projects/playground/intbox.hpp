#ifndef INTBOX_HPP
#define INTBOX_HPP
#include <bits/stdc++.h>

class IntBox {
    private:
        int *storedvalue;

    public:
        IntBox(int);
        IntBox(const IntBox &);
        ~IntBox();
        const IntBox & operator = (const IntBox &);
        int value();
        void displayInternal();
        void displayInternalAddress();
};

#endif