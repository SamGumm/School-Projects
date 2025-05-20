#include <iostream>
#include <stdexcept>

#include "stack.hpp"


char Stack::pop() {
            if(isEmpty()) throw std::runtime_error("pop() on empty stack.") ;
            Node* temp = head_;
            char val = temp->data;
            head_ = temp->next;
            delete temp;
            count--;
            return val;
        }

        void Stack::push(char value) {
            Node* node = new Node{head_, value};
            head_ = node;
            ++count;
        }

        char Stack::peek() {
            if (isEmpty()) {
                throw std::runtime_error("peek() on empty stack.");
            }
            return head_->data;
        }

        bool Stack::isEmpty() {
            return count == 0;
        }  

        int Stack::size() {
            return count;
        }

        Stack::~Stack() {
            while(head_) {
                Node* temp = head_;
                head_ = head_->next;
                delete(temp);
            }
        }

        bool Stack::parenMatch(char s[]) {
            Stack stack;
            for(int i = 0; s[i] != '\0'; i++) {
                char c = s[i];
                if(c == '(' || c == '[' || c == '{') {
                    stack.push(c);
                } 
                else if(c == ')' || c == ']' || c == '}') {
                    if(stack.isEmpty()) return false;

                    char c2 = stack.pop();
                    
                    if((c == ')' && c2 != '(') || (c == ']' && c2 != '[') || (c == '}' && c2 != '{')) {
                        return false;
                    }
                }         
            }
            return stack.isEmpty();
        }

        int Stack::precedence(char op) {
            switch (op) {
                case '^': return 4;
                case '*': 
                case '/': 
                case '%': return 3;
                case '+': 
                case '-': return 2;
                default:  return 0;
            }
        }

        bool Stack::infixToPostfix(char s[]) {
            Stack stack;

            if(!parenMatch(s)) {
                std::cout << "Parenthesis mismatch" << std::endl;
                return false;
            }

            for(int i = 0; s[i] != '\0'; ++i) {
                char c = s[i];

                if ((c >= '0' && c <= '9') ||
                    (c >= 'A' && c <= 'Z') ||
                    (c >= 'a' && c <= 'z')) std::cout << c;

                else if(c == '(') stack.push(c);

                else if (c == ')') {
                    while (!stack.isEmpty() && stack.peek() != '(') {
                        std::cout << stack.pop();
                    }
                    if (stack.isEmpty()) {
                        // mismatched “)” 
                        return false;
                    }
                    stack.pop();  // discard the '('
                }

                else if (c=='+'||c=='-'||c=='*'||c=='/'||c=='%'||c=='^') {
                    while (!stack.isEmpty() && stack.peek() != '(') {
                        char top = stack.peek();
                        int pTop = precedence(top), pCur = precedence(c);
                        if (pTop > pCur || (pTop == pCur && c != '^')) {
                            std::cout << stack.pop();
                        } else {
                            break;
                        }
                    }
                    stack.push(c);
                }
            }

            while(!stack.isEmpty()) {
                if(stack.peek() == '(') {
                    return false;
                }
                std::cout << stack.pop();
            }

            std::cout << '\n';
            return true;
        }

int main() {
    Stack s;
    s.push('a');
    s.push('b');
    char top = s.peek();
    char x = s.pop();
    bool e = s.isEmpty();
    std::cout << top << "\n" <<
                x << "\n" <<
                e << std::endl;
/*                    0    1    2    3    4    5    6                           */
    char string[] = {'{', '[', '(', ')', ']', '}', '\0'};
    if(s.parenMatch(string)) std::cout << "true" << std::endl;
    else std::cout << "false" << std::endl;


    char infixStr[] = {'(', 'A', '+', 'B', '/', 'C', '*', '(', 'D', '+', 'E', ')', '-', 'F', ')', '\0'};
    s.infixToPostfix(infixStr);
    char userInfix[100];
    std::cout << "enter an equation: ";
    if(scanf("%99s", userInfix) == 1) {
        std::cout << "Postfix result: " << s.infixToPostfix(userInfix) << std::endl;
    }
    
    return 0;
}