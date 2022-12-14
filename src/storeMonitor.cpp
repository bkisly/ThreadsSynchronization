//
// Created by bkisly on 14.12.22.
//

#include <iostream>
#include "../include/storeMonitor.h"
#include "../include/fileManager.h"

using namespace std;

StoreMonitor::StoreMonitor(unsigned int storeCapacity) {
    this->storeCapacity = storeCapacity;
    prodAmount = 0;
    consAmount = 0;

    pthread_cond_init(&prodCondition, nullptr);
    pthread_cond_init(&consCondition, nullptr);
    pthread_mutex_init(&mutex, nullptr);
}

StoreMonitor::~StoreMonitor() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&prodCondition);
    pthread_cond_destroy(&consCondition);
}

void StoreMonitor::produce(unsigned id, unsigned amount) {
    cout << "inside monitor, mutex is: " << endl;
    pthread_mutex_lock(&mutex);
    cout << "inside mutex" << endl;
    prodAmount++;

    if(prodUnlocked == 0) {
        pthread_cond_wait(&prodCondition, &mutex);
    }
    else {
        prodUnlocked = 0;
        unsigned storeStatus = readStore(); // read the store here

        if(canProduce(storeStatus, amount)) {
            cout << "can produce" << endl;
            writeToStore(storeStatus + amount);
            appendLog(
                    "log_producer_" + std::to_string(id) + ".txt",
                    "Producer no. " + std::to_string(id) + " has produced " + std::to_string(amount) + " units.");

            prodUnlocked = 1;
            prodAmount--;
            cout << "produced" << endl;
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
    cout << "signal sent" << endl;
    pthread_mutex_unlock(&mutex);
}

void StoreMonitor::consume(unsigned id, unsigned amount) {
    pthread_mutex_lock(&mutex);
    consAmount++;

    if (consUnlocked == 0) {
        pthread_cond_wait(&consCondition, &mutex);
    } else {
        consUnlocked = 0;
        unsigned storeStatus = readStore(); // read the store

        if (canConsume(storeStatus, amount)) {
            writeToStore(storeStatus + amount);
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

bool StoreMonitor::canProduce(unsigned storeStatus, unsigned amount) const {
    return (storeStatus < storeCapacity / 2 || consAmount == 0) && storeStatus + amount <= storeCapacity;
}

bool StoreMonitor::canConsume(unsigned storeStatus, unsigned amount) const {
    return (storeStatus >= storeCapacity / 2 || consAmount == 0) && storeStatus - amount >= 0;
}