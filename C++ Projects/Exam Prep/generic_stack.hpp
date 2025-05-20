#pragma once

#include <stdexcept>

template <typename T>
struct Node {
    Node<T>* next {nullptr};
    T data;
};

template <typename T> class Stack {
    public:
        Stack() = default;

        T pop() {
            if(isEmpty()) throw std::underflow_error("pop() on empty stack.");
            Node<T>* temp = head_;
            T val = temp->data;
            head_ = temp->next;
            delete temp;
            count--;
            return val;
        }

        void push(T value) {
            Node<T>* node = new Node<T>{head_, value};
            head_ = node;
            ++count;
        }

        T peek() const {
            if(isEmpty()) throw std::underflow_error("peek() on empty stack.");
            return head_->data;
        }

        int size() const {
            return count;
        }        

        bool isEmpty() const { return count == 0; }

        ~Stack() {
            while(head_) {
                Node<T>* temp = head_;
                head_ = head_->next;
                delete(temp);
            }
        }

        bool parenMatch(T s[]) {
            Stack<T> stack;
            for(int i = 0; s[i] != static_cast<T>('\0'); i++) {
                T c = s[i];
                if( c == static_cast<T>('(') || 
                    c == static_cast<T>('[') || 
                    c == static_cast<T>('{')) {
                    stack.push(c);
                } 
                else if(c == static_cast<T>(')') || 
                        c == static_cast<T>(']') || 
                        c == static_cast<T>('}')) {
                    if(stack.isEmpty()) return false;
        
                    T c2 = stack.pop();
                    
                    if( (c == static_cast<T>(')') && c2 != static_cast<T>('(')) || 
                        (c == static_cast<T>(']') && c2 != static_cast<T>('[')) || 
                        (c == static_cast<T>('}') && c2 != static_cast<T>('{'))) {
                        return false;
                    }
                }         
            }
            return stack.isEmpty();
        }

        T precedence(T op) {
            switch (op) {
                case static_cast<T>('^') : return static_cast<T>(4);
                case static_cast<T>('*') :
                case static_cast<T>('/') :
                case static_cast<T>('%') : return static_cast<T>(3);
                case static_cast<T>('+') :
                case static_cast<T>('-') : return static_cast<T>(2);
                default: return static_cast<T>(0);
            }
        }

        bool infixToPostfix(T s[]) {
            Stack<T> stack;
        
            if(!parenMatch(s)) {
                std::cout << "Parenthesis mismatch" << std::endl;
                return false;
            }
        
            for (int i = 0; s[i] != T('\0'); i++) {
                T c = s[i];
        
                if ((c >= static_cast<T>('0') && c <= static_cast<T>('9')) ||
                    (c >= static_cast<T>('A') && c <= static_cast<T>('Z')) ||
                    (c >= static_cast<T>('a') && c <= static_cast<T>('z'))) std::cout << c;
        
                else if (c == static_cast<T>('(')) stack.push(c);
        
                else if (c == static_cast<T>(')')) {
                    while(!stack.isEmpty() && stack.peek() != static_cast<T>('(')) {
                        std::cout << stack.pop();
                    }
                    if (stack.isEmpty()) {
                        return false;
                    }
                    stack.pop();
                }
        
                else if(c == static_cast<T>('+') || 
                        c == static_cast<T>('-') || 
                        c == static_cast<T>('%') || 
                        c == static_cast<T>('^') ||
                        c == static_cast<T>('*') ||
                        c == static_cast<T>('/')) {
                    while(!stack.isEmpty() && stack.peek() != T('(')) {
                        T top = stack.peek();
                        T pTop = precedence(top), pCur = precedence(c);
                        if (pTop > pCur || (pTop == pCur && c != T('^'))) {
                            std::cout << stack.pop();
                        } else {
                            break;
                        }
                    }
                    stack.push(c);
                }
            }
        
            while(!stack.isEmpty()) {
                if(stack.peek() == static_cast<T>('(')) {
                    return false;
                }
                std::cout << stack.pop();
            }
            std::cout << "\n";
            return true;
        }

    private:
        int count{0};
        Node<T>* head_{nullptr};
};