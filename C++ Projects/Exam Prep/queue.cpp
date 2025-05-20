#include <iostream>

struct Node {
    Node* next{nullptr};
    Node* prev{nullptr};
    char data;
};

class Queue {
    public:
        char element() {}

        char peek() {}

        bool add(char e) {}

        bool offer(char e) {}

        char remove() {}

        char poll() {}

        bool isEmpty() {}

        int size() {}

        void iterator() {}

    private:
        Node* head_{nullptr};
        int count{0};
};