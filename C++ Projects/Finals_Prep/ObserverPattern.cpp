#include "ObserverPattern.hpp"

class IntSensor 
    : public ObservableData<int>,
      public Observable<int>
{
    int value_{0};

    public:
        int getData() override { return value_; }

        void setValue(int v) {
            value_ = v;
            notifyObservers(*this); //push update to every Observer<int>*
        }
};

int main() {
    IntSensor sensor;
    ConcreteObserver obs1, obs2;

    sensor.registerObserver(&obs1);
    sensor.registerObserver(&obs2);

    sensor.setValue(42);
}