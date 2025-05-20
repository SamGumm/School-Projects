#include "binarytree.hpp"
#include "generic_stack.hpp"
#include <iostream>


int main() {
    //     Stack<char> s;
    //     s.push('a');
    //     s.push('b');
    //     char top = s.peek();
    //     char x = s.pop();
    //     bool e = s.isEmpty();
    //     std::cout << top << "\n" <<
    //                 x << "\n" <<
    //                 e << std::endl;
    // /*                    0    1    2    3    4    5    6                           */
    //     char string[] = {'{', '[', '(', ')', ']', '}', '\0'};
    //     if(s.parenMatch(string)) std::cout << "true" << std::endl;
    //     else std::cout << "false" << std::endl;
    
    
    //     char infixStr[] = {'(', 'A', '+', 'B', '/', 'C', '*', '(', 'D', '+', 'E', ')', '-', 'F', ')', '\0'};
    //     s.infixToPostfix(infixStr);
    //     char userInfix[100];
    //     std::cout << "enter an equation: ";
    //     if(scanf("%99s", userInfix) == 1) {
    //         std::cout << "Postfix result: " << s.infixToPostfix(userInfix) << std::endl;
    //     }
    
    //     BinaryTree<char> tree = BinaryTree<char>('a');
    //     BinaryTree<char> expressionTree;
    
    //     //left half
    //     tree.addChild(tree.root(), 'b', CHILD::LEFT);
    //     tree.addChild(tree.root()->left, 'd', CHILD::LEFT);
    //     tree.addChild(tree.root()->left, 'e', CHILD::RIGHT);
    //     tree.addChild(tree.root()->left->left, 'h', CHILD::LEFT);
    //     tree.addChild(tree.root()->left->left, 'i', CHILD::RIGHT);
    //     tree.addChild(tree.root()->left->right, 'k', CHILD::LEFT);
    //     tree.addChild(tree.root()->left->right, 'm', CHILD::RIGHT);
    
    //     //right half
    //     tree.addChild(tree.root(), 'c', CHILD::RIGHT);
    //     tree.addChild(tree.root()->right, 'f', CHILD::LEFT);
    //     tree.addChild(tree.root()->right, 'g', CHILD::RIGHT);
    //     tree.addChild(tree.root()->right->left, 'n', CHILD::RIGHT);
    //     tree.addChild(tree.root()->right->left->right, 'p', CHILD::LEFT);
    //     tree.addChild(tree.root()->right->right, 'o', CHILD::LEFT);
    //     tree.addChild(tree.root()->right->right->left, 'q', CHILD::RIGHT);
    
    //     //postorder print
    //     tree.postOrder(tree.root());
    //     std::cout << "\n";
    //     tree.preOrder(tree.root());
    //     std::cout << "\n";
    //     tree.inOrder(tree.root());
    //     std::cout << "\n";
    
    //     char userPostFix[100];
    //     bool result;
    //     std::cout << "enter a postfix equation: ";
    //     if(scanf("%99s", userPostFix) == 1) {
    //         result = expressionTree.postfixToExpressionTree(userPostFix);
    //     }


    //     if(result) {
    //         std::cout << "true" << std::endl;
    //     } else std::cout << "false" << std::endl;
    //     expressionTree.inOrder(expressionTree.root());
        
    //     std::cout << "\n";

    //     std::cout << expressionTree.height(expressionTree.root()) << std::endl;
        
        std::cout << "\n";


        char s1[] = {'a', 'b', 'c', 'd', '\0'};
        char a = 'a';
        char b = 'b';
        char c = 'c';
        char d = 'd';
        char end = '\0';
        char* s2[] = {&a, &b, &c, &d, &end};
        char s3[] = {a, b, c, d, end};

        std::cout << ((char *) s1 + 2) << "\n"; //output: 'cd'
        //dereferences the pointer, therefore calling operator<<(char), which just outputs 'c'
        std::cout << *((char *) s1 + 2) << "\n"; //output 'c'
        //std::cout << ((char) s1 + 2) << "\n"; //loses precision, runtime error
        std::cout << (s1 + 2) << "\n"; //output: 'cd'

        //gibberish because s2 decays to a char**, then is cast as a char*, then increments by 2 in the raw memory and tries to print the bytes
        //std::cout << ((char *) s2 + 2) << "\n"; 
        //std::cout << *((char *) s2 + 2) << "\n"; //gibberish, casts char** to char*
        std::cout << *((char **) s2 + 2) << "\n"; //outputs 'cd'
        //std::cout << ((char) s2 + 2) << "\n"; //loses precision, runtime error

        std::cout << *((char *) s3 + 2) << "\n";
        std::cout << *((char *) s3 + 2) << "\n"; 
        //std::cout << ((char) s3 + 2) << "\n"; //loses precision, runtime error


        return 0;
    }