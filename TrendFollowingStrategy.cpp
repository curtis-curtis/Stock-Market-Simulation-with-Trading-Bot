#include "TrendFollowingStrategy.h"
#include "Utils.h"
#include <iostream>

TrendFollowingStrategy::TrendFollowingStrategy() :
Strategy(), shortMovingAverageWindow(0), longMovingAverageWindow(0)
{
}

TrendFollowingStrategy::TrendFollowingStrategy(const std::string &name, int shortWindow, int longWindow) :
Strategy(name), shortMovingAverageWindow(shortWindow), longMovingAverageWindow(longWindow)
{
}

Action TrendFollowingStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    if (index == 0) { return HOLD; }
    double maShort = calculateMovingAverage(market, index, shortMovingAverageWindow);
    double maLong = calculateMovingAverage(market, index, longMovingAverageWindow);
    double maShortPrev = calculateMovingAverage(market, index - 1, shortMovingAverageWindow);
    double maLongPrev = calculateMovingAverage(market, index - 1, longMovingAverageWindow);

    if (index == market->getNumTradingDays() - 101 && maShort > maLong) { return BUY; }

    if ((maShortPrev < maLongPrev) && (maShort >= maLong) && (currentHolding <= 0.5)) {
        return BUY;
    }
    else if ((maShortPrev > maLongPrev) && (maShort <= maLong) && (currentHolding >= 0.5)) {
        return SELL;
    }
    return HOLD;
}

TrendFollowingStrategy **TrendFollowingStrategy::generateStrategySet
(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, 
int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int numStrategy = ((maxShortWindow - minShortWindow) / stepShortWindow + 1) * ((maxLongWindow - minLongWindow) / stepLongWindow + 1);
    TrendFollowingStrategy** array = new TrendFollowingStrategy*[numStrategy];
    int arrayIndex = 0;
    for (int i = minShortWindow; i <= maxShortWindow; i += stepShortWindow) {
        for (int j = minLongWindow; j <= maxLongWindow; j += stepLongWindow) {
            string strategyName = baseName + "_" + to_string(i) + "_" + to_string(j);
            array[arrayIndex] = new TrendFollowingStrategy(strategyName, i, j);
            arrayIndex++;
        }
    }
    return array;
}
