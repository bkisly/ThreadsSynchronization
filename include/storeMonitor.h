//
// Created by bkisly on 14.12.22.
//

#ifndef THREADSSYNCHRONIZATION_STOREMONITOR_H
#define THREADSSYNCHRONIZATION_STOREMONITOR_H

#include <iostream>
#include <pthread.h>
#include "fileManager.h"

using namespace std;

class StoreMonitor {
private:
    unsigned prodUnlocked = 1;
    unsigned consUnlocked = 0;

    unsigned prodAmount, consAmount, storeCapacity;

    pthread_mutex_t mutex;
    pthread_cond_t prodCondition;
    pthread_cond_t consCondition;

public:
    explicit StoreMonitor() {
        storeCapacity = 0;
        prodAmount = 0;
        consAmount = 0;

        pthread_cond_init(&prodCondition, nullptr);
        pthread_cond_init(&consCondition, nullptr);
        pthread_mutex_init(&mutex, nullptr);
    }

    ~StoreMonitor() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&prodCondition);
        pthread_cond_destroy(&consCondition);
    }

    void setStoreCapacity(unsigned capacity) {
        storeCapacity = capacity;
    }

    void produce(unsigned id, unsigned amount) {
        pthread_mutex_lock(&mutex);
        prodAmount++;

        if(prodUnlocked == 0) {
            pthread_cond_wait(&prodCondition, &mutex);
        }
        else {
            prodUnlocked = 0;
            unsigned storeStatus = readStore();

            if(canProduce(storeStatus, amount)) {
                writeToStore(storeStatus + amount);
                appendLog(
                        "log_producer_" + std::to_string(id) + ".txt",
                        "Producer no. " + std::to_string(id) + " has produced " + std::to_string(amount) + " units.");

                prodUnlocked = 1;
                prodAmount--;
                pthread_cond_signal(&prodCondition);
            }
            else {
                appendLog(
                        "log_producer_" + std::to_string(id) + ".txt",
                        "Producer no. " + std::to_string(id) + " hasn't produced anything.");

                consUnlocked = 1;
                prodAmount--;
                pthread_cond_signal(&consCondition);
            }
        }

        pthread_mutex_unlock(&mutex);
    }
    void consume(unsigned id, unsigned amount) {
        pthread_mutex_lock(&mutex);
        consAmount++;

        if (consUnlocked == 0) {
            pthread_cond_wait(&consCondition, &mutex);
        } else {
            consUnlocked = 0;
            unsigned storeStatus = readStore(); // read the store

            if (canConsume(storeStatus, amount)) {
                writeToStore(storeStatus - amount);
                appendLog(
                        "log_consumer_" + std::to_string(id) + ".txt",
                        "Consumer no. " + std::to_string(id) + " has consumed " + std::to_string(amount) + " units.");

                consUnlocked = 1;
                consAmount--;
                pthread_cond_signal(&consCondition);
            }
            else {
                appendLog(
                        "log_consumer_" + std::to_string(id) + ".txt",
                        "Consumer no. " + std::to_string(id) + " hasn't consumed anything.");

                prodUnlocked = 1;
                consAmount--;
                pthread_cond_signal(&prodCondition);
            }
        }

        pthread_mutex_unlock(&mutex);
    }
    [[nodiscard]] bool canProduce(unsigned storeStatus, unsigned amount) const {
        return (storeStatus < storeCapacity / 2 || consAmount == 0) && storeStatus + amount <= storeCapacity;
    }
    [[nodiscard]] bool canConsume(unsigned storeStatus, unsigned amount) const {
        return (storeStatus >= storeCapacity / 2 || consAmount == 0) && storeStatus - amount >= 0;
    }
} monitor;

#endif //THREADSSYNCHRONIZATION_STOREMONITOR_H
