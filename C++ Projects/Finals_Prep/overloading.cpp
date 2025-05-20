#include <iostream>

template <typename T>
class SimpleVector {
    private:
        T *data;
        size_t sz;

    public:
        SimpleVector(size_t n) : data(new T[n]), sz(n) {}

        ~SimpleVector() { delete[] data; }

        T &operator[](size_t i) { return data[i]; }
        const &operator[](size_t i) { return data[i]; }
        size_t size() const { return sz; }
};

struct Vec {
    double x, y;
    //binary + as member: left operand is *this, right is rhs
    Vec operator+(Vec const &rhs) const {
        return { x + rhs.x, y + rhs.y };
    }

    // prefix ++
    // returns a reference to the object is was called on
    Vec& operator++() {
        ++x; ++y;
        return *this;
    }

    Vec operator++(int) {
        Vec temp = *this;
        ++*this;
        return temp;
    }
};

