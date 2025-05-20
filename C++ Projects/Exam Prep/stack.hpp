#pragma once

struct Node {
    Node* next {nullptr};
    char data;
};

class Stack {
    public:
        Stack() = default;
        
        char pop();

        void push(char value);

        char peek();

        bool isEmpty();

        int size();

        ~Stack();

        bool parenMatch(char s[]);

        int precedence(char op);

        bool infixToPostfix(char s[]);

    private:
        int count{0};
        Node* head_ {nullptr};
};