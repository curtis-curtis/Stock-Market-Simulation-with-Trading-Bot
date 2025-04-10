#include "WeightedTrendFollowingStrategy.h"
#include "Utils.h"
#include <cmath>

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy() : TrendFollowingStrategy()
{
}

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy(const string &name, int shortWindow, int longWindow) :
TrendFollowingStrategy(name, shortWindow, longWindow)
{
}

double WeightedTrendFollowingStrategy::calculateExponentialWeight(int index) const
{
    const double GROWTH_FACTOR = 1.1;
    return pow(GROWTH_FACTOR, index);
}

double WeightedTrendFollowingStrategy::calculateMovingAverage(Market *market, int index, int window) const
{
    double denominator = 0;
    double weightedPriceSum = 0;

    if (window <= index + 1) {
        for (int i = index - window + 1; i <= index; i++) {
            weightedPriceSum += market->getPrice(i) * calculateExponentialWeight(i);
            denominator += calculateExponentialWeight(i);
        }
        return weightedPriceSum / denominator;
    }
    else {
        for (int i = 0; i <= index; i++) {
            weightedPriceSum += market->getPrice(i) * calculateExponentialWeight(i);
            denominator += calculateExponentialWeight(i);
        }
        return weightedPriceSum / denominator;
    }
}

WeightedTrendFollowingStrategy **WeightedTrendFollowingStrategy::generateStrategySet(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int numStrategy = ((maxShortWindow - minShortWindow) / stepShortWindow + 1) * ((maxLongWindow - minLongWindow) / stepLongWindow + 1);
    WeightedTrendFollowingStrategy** array = new WeightedTrendFollowingStrategy*[numStrategy];
    int arrayIndex = 0;
    for (int i = minShortWindow; i <= maxShortWindow; i += stepShortWindow) {
        for (int j = minLongWindow; j <= maxLongWindow; j += stepLongWindow) {
            string strategyName = baseName + "_" + to_string(i) + "_" + to_string(j);
            array[arrayIndex] = new WeightedTrendFollowingStrategy(strategyName, i, j);
            arrayIndex++;
        }
    }
    return array;
}
