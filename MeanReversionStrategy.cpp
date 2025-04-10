#include "MeanReversionStrategy.h"
#include "Utils.h"
#include <cmath>
#include <iostream>

MeanReversionStrategy::MeanReversionStrategy() :
Strategy(), window(0), threshold(0)
{
}

MeanReversionStrategy::MeanReversionStrategy(const string &name, int window, int threshold) :
Strategy(name), window(window), threshold(threshold)
{

}

Action MeanReversionStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    double ma = calculateMovingAverage(market, index, window);
    double upperBound = ma * (1 + (double)threshold / 100);
    double lowerBound = ma * (1 - (double)threshold / 100);
    double price = market->getPrice(index);
    if (price > upperBound && currentHolding >= 0.5) {
        return SELL;
    }
    else if (price < lowerBound && currentHolding <= 0.5) {
        return BUY;
    }
    return HOLD;
}

MeanReversionStrategy **MeanReversionStrategy::generateStrategySet
(const string &baseName, int minWindow, int maxWindow, int windowStep, 
int minThreshold, int maxThreshold, int thresholdStep)
{
    int numStrategy = ((maxWindow - minWindow) / windowStep + 1) * ((maxThreshold - minThreshold) / thresholdStep + 1);
    MeanReversionStrategy** array = new MeanReversionStrategy*[numStrategy];
    int arrayIndex = 0;
    for (int i = minWindow; i <= maxWindow; i += windowStep) {
        for (int j = minThreshold; j <= maxThreshold; j += thresholdStep) {
            string strategyName = baseName + "_" + to_string(i) + "_" + to_string(j);
            array[arrayIndex] = new MeanReversionStrategy(strategyName, i, j);
            arrayIndex++;
        }
    }
    return array;
}