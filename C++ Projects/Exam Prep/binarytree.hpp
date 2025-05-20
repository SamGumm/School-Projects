#pragma once

#include <iostream>
#include "generic_stack.hpp"

template <typename T>
struct TreeNode {
    T data;
    TreeNode* left;
    TreeNode* right;
};

enum struct CHILD {
    LEFT, RIGHT
};

template <typename T>
class BinaryTree {
    public:
        BinaryTree(T rootData) :
        root_(new TreeNode<T>{rootData, nullptr, nullptr}) {}

        BinaryTree() : count(0), root_(nullptr){}

        ~BinaryTree() {
            destroy(root_);
        }

        TreeNode<T>* addChild(TreeNode<T>* parent, T data, CHILD branch) {
            TreeNode<T>* newNode = new TreeNode<T>{data, nullptr, nullptr};
            switch(branch) {
                case(CHILD::LEFT) : parent->left = newNode; break;
                case(CHILD::RIGHT) : parent->right = newNode; break;
            }
            count++;
            return newNode;    
        }

        void postOrder(TreeNode<T>* root) {
            if(!root) return;
            postOrder(root->left);
            postOrder(root->right);
            std::cout << root->data << " ";
        }

        void preOrder(TreeNode<T>* root) {
            if(!root) return;
            std::cout << root->data << " ";
            preOrder(root->left);
            preOrder(root->right);
        }


        void inOrder(TreeNode<T>* root) {
            if(!root) return;
            inOrder(root->left);
            std::cout << root->data << " ";
            inOrder(root->right);
        }

        // void levelOrder(TreeNode<T>* root) {
            
        // }

        TreeNode<T>* root() const {return root_; }

        bool postfixToExpressionTree(const T s[]) {
            Stack<TreeNode<T>*> stack;
            for (int i = 0; s[i] != static_cast<T>('\0'); i++) {
                T c = s[i];
                if(c == static_cast<T>(' ')) continue;
                else if((c >= static_cast<T>('A') && c <= static_cast<T>('Z')) ||
                        (c >= static_cast<T>('a') && c <= static_cast<T>('z')) ||
                        (c >= static_cast<T>('0') && c <= static_cast<T>('9'))) {
                        //if(expressionTree.isEmpty()) expressionTree = new BinaryTree<char>(c);
                        TreeNode<T>* newNode = new TreeNode<T>{c, nullptr, nullptr};
                        stack.push(newNode);        
                }
                else if(c == static_cast<T>('+') || 
                        c == static_cast<T>('-') || 
                        c == static_cast<T>('%') || 
                        c == static_cast<T>('^') ||
                        c == static_cast<T>('*') ||
                        c == static_cast<T>('/')) {
                    TreeNode<T>* right = stack.pop();
                    TreeNode<T>* left = stack.pop();
                    TreeNode<T>* newNode = new TreeNode<T>{c, left, right};
                    stack.push(newNode);
                }
        
            }
            if(stack.isEmpty()) return false;
            TreeNode<T>* expressionRoot = stack.pop();
            this->root_ = expressionRoot;
            return true;
        }

        bool isEmpty() { return count == 0; }

        int height(TreeNode<T>* node) {
            if(!node) return -1;

            int lHeight = height(node->left);
            int rHeight = height(node->right);
            if(lHeight > rHeight) return lHeight + 1;
            else return rHeight + 1;
        }



    private:
        int count{0};
        TreeNode<T>* root_{nullptr};

        void destroy(TreeNode<T>* node) {
            if(!node) return;
            destroy(node->left);
            destroy(node->right);
            delete node;
        }

};