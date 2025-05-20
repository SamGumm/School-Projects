#pragma once
#include <vector>
#include <algorithm>
#include <iostream>


template <class T>
class ObservableData {
    public:
        virtual T getData() = 0;
};


template <class T>
class Observer {
    public:
        virtual void update(ObservableData<T>&) = 0;
};


template <class T>
class Observable {
    protected:
        std::vector<Observer<T>*> observers;

    public:


        /**
         * Guards against duplication
         */
        void registerObserver(Observer<T>* o) {
            //performs a linear scan from begin() to end() and compares (using pointer equality) p against o
            //returns a pointer to a position in the list, if that pointer equals the end of observers, then it reached the end without finding duplicates
            //if find reaches the end, it means that o was not already in the list
            if(find(observers.begin(), observers.end(), o) == observers.end()) {
                observers.push_back(o);
            }
        }

        void removeObserver(Observer<T> *o) {
            auto it = std::find(observers.begin(), observers.end(), o);
            if (it != observers.end()) {
                observers.erase(it);
            }
        }

        void notifyObservers(ObservableData<T>& subject) {
            for (auto* o : observers) {
                o->update(subject);
            }
        }
};

class ConcreteObserver : public Observer<int> {
    public: 
        void update(ObservableData<int>& subject) override {
            int newVal = subject.getData();
            std::cout << "Observed new value: " << newVal << "\n";
        }
};