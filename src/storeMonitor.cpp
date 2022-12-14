//
// Created by bkisly on 14.12.22.
//

#include "../include/storeMonitor.h"

StoreMonitor::StoreMonitor(unsigned int storeCapacity) {
    this->storeCapacity = storeCapacity;
    prodAmount = 0;
    consAmount = 0;

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&prodCondition, nullptr);
    pthread_cond_init(&consCondition, nullptr);
}

StoreMonitor::~StoreMonitor() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&prodCondition);
    pthread_cond_destroy(&consCondition);
}

void StoreMonitor::produce(unsigned int amount) {
    pthread_mutex_lock(&mutex);
    prodAmount++;

    if(prodUnlocked == 0) {
        pthread_cond_wait(&prodCondition, &mutex);
    }
    else {
        prodUnlocked = 0;
        int storeStatus = 0; // read the store here

        if(canProduce(storeStatus, amount)) {
            // write to store
            prodUnlocked = 1;
            prodAmount--;
            pthread_cond_signal(&prodCondition);
        }
        else {
            consUnlocked = 1;
            prodAmount--;
            pthread_cond_signal(&consCondition);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void StoreMonitor::consume(unsigned int amount) {
    pthread_mutex_lock(&mutex);
    consAmount++;

    if (consUnlocked == 0) {
        pthread_cond_wait(&consCondition, &mutex);
    } else {
        consUnlocked = 0;
        int storeStatus = 0; // read the store

        if (canConsume(storeStatus, amount)) {
            // write to store
            consUnlocked = 1;
            consAmount--;
            pthread_cond_signal(&consCondition);
        }
        else {
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