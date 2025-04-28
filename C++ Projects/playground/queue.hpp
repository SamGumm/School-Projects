#ifndef QUEUE_HPP
#define QUEUE_HPP
#ifndef NULL
#define NULL 0
#endif


struct ListNode {
    public:
        int element;
        ListNode *next;
        ListNode(int theElement, ListNode *n = NULL): 
            element(theElement), next(n){};
};

class UnderflowException{};

class IntQueue {
    private:
        ListNode *front;
        ListNode *back;
        int size;
        void makeEmpty();

    public:
        IntQueue();
        ~IntQueue();
        const IntQueue & operator = (const IntQueue &);
        void deQueue();
        bool isEmpty();
        IntQueue & getFront();
        void enQueue();
        void print();
};

#endif