#include "Strategy.h"
#include <iostream>

Strategy::Strategy() : name("")
{
}

Strategy::Strategy(const string &name) : name(name)
{
}

double Strategy::calculateMovingAverage(Market *market, int index, int window) const
{
    double priceSum = 0.0;
    if (window <= index + 1) {
        for (int i = index - window + 1; i <= index; i++) {
            priceSum += market->getPrice(i);
        }
        return priceSum / (double)window;
    }
    else {
        for (int i = 0; i <= index; i++) {
            priceSum += market->getPrice(i);
        }
        return priceSum / (double)(index + 1);
    }
}

string Strategy::getName() const
{
    return name;
}

Strategy:: ~Strategy() {

}
