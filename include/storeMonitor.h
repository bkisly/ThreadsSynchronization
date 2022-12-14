//
// Created by bkisly on 14.12.22.
//

#ifndef THREADSSYNCHRONIZATION_STOREMONITOR_H
#define THREADSSYNCHRONIZATION_STOREMONITOR_H

#include <pthread.h>

class StoreMonitor {
private:
    unsigned prodUnlocked = 1;
    unsigned consUnlocked = 0;

    unsigned prodAmount, consAmount, storeCapacity;

    pthread_mutex_t mutex;
    pthread_cond_t prodCondition;
    pthread_cond_t consCondition;

public:
    explicit StoreMonitor(unsigned storeCapacity);
    ~StoreMonitor();

    void produce(unsigned id, unsigned amount);
    void consume(unsigned id, unsigned amount);
    [[nodiscard]] bool canProduce(unsigned storeStatus, unsigned amount) const;
    [[nodiscard]] bool canConsume(unsigned storeStatus, unsigned amount) const;
} monitor(100);

#endif //THREADSSYNCHRONIZATION_STOREMONITOR_H
