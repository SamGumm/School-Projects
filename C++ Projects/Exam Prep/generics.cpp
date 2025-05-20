#include <iostream>

/*One function works for all data types.
this would work even for user defined types
IF the '>' operator is overloaded */
template <typename T>
T myMax(T x, T y) {
    return (x > y) ? x : y;
};

template <typename T>
class Array {
    private:
        T* ptr;
        int size;
    
    public:
        Array(T arr[], int s);
        void print();
};

template <typename T>
Array<T>::Array(T arr[], int s) {
    ptr = new T[s];
    size = s;
    for (int i = 0; i < size; i++) {
        ptr[i] = arr[i];
    }
}

template <typename T>
void Array<T>::print() {
    for(int i = 0; i < size; i++) {
        std::cout << " " << *(ptr + i);
    } std::cout << std::endl;
}

int main() {
    std::cout << myMax<int>(3, 7) << std::endl;
    std::cout << myMax<double>(3.0, 7.0) << std::endl;
    std::cout << myMax<char>('g','e') << std::endl;

    int arr[5] = {1,2,3,4,5};
    Array<int> a(arr, 5);
    a.print();
    return 0;
};
