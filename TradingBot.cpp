#include "TradingBot.h"
#include <limits>

TradingBot::TradingBot(Market *market, int initialCapacity) :
market(market),
availableStrategies(new Strategy*[initialCapacity]),
strategyCount(0),
strategyCapacity(initialCapacity)
{
}

TradingBot::~TradingBot()
{
    for (int i = 0; i < strategyCount; i++) {
        delete availableStrategies[i];
    }
    delete[] availableStrategies;
}

void TradingBot::addStrategy(Strategy *strategy)
{
    if (strategyCount > strategyCapacity) {
        cout << "ERROR: strategyCount > strategyCapacity" << endl;
    }
    else if (strategyCount == strategyCapacity) {
        Strategy** newAvailableStrategies = new Strategy*[strategyCapacity + 1];
        for (int i = 0; i < strategyCapacity; i++) {
            newAvailableStrategies[i] = availableStrategies[i];
        }
        newAvailableStrategies[strategyCapacity] = strategy;
        delete[] availableStrategies;
        availableStrategies = newAvailableStrategies;

        strategyCount++;
        strategyCapacity++;
    }
    else if (strategyCount < strategyCapacity) {
        availableStrategies[strategyCount] = strategy;
        strategyCount++;
    }
}

SimulationResult TradingBot::runSimulation()
{
    SimulationResult result;
    const int NUM_DAYS = 101;
    for (int i = 0; i < strategyCount; i++) {

        double cumulProfit = 0;
        double purchasePrice = -100;
        double currentHolding = 0;
        Strategy* strategy = availableStrategies[i];

        for (int nthDay = market->getNumTradingDays() - NUM_DAYS; nthDay < market->getNumTradingDays(); nthDay++) {
            double price = market->getPrice(nthDay);
            Action action = strategy->decideAction(market, nthDay, currentHolding);

            if (action == BUY) {
                if (currentHolding >= 0.5) { cout << "ERROR" << endl; }
                currentHolding = 1;
                purchasePrice = price;
            }
            else if (action == SELL) {
                if (currentHolding <= 0.5) { cout << "ERROR" << endl; }
                currentHolding = 0;
                cumulProfit += price - purchasePrice;
            }
        }
        if (currentHolding >= 0.5) {
            cumulProfit += market->getLastPrice() - purchasePrice;
        }
        if (result.totalReturn < cumulProfit) {
            result.bestStrategy = strategy;
            result.totalReturn = cumulProfit;
        }
    }
    return result;
}
