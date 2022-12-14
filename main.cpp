#include <iostream>
#include <random>
#include "unistd.h"
#include "include/fileManager.h"
#include "include/storeMonitor.h"
#include "include/threadArgs.h"

using namespace std;

void *producer(void *arg);
void *consumer(void *arg);
threadArgs getArgs(unsigned id, unsigned minValue, unsigned maxValue, StoreMonitor *monitor);

int main() {
    pthread_t threads[2];

    threadArgs prodArgs = getArgs(1, 15, 25, &monitor);
    pthread_create(&threads[0], nullptr, &producer, &prodArgs);
    pthread_join(threads[0], nullptr);

    pthread_exit(nullptr);
}

void *producer(void *arg) {
    auto *args = (threadArgs*)arg;
    std::uniform_int_distribution<unsigned> intDist(args->minValue, args->maxValue);
    std::default_random_engine engine;

    while(true) {
        sleep(1);
        cout << "inside producer func" << endl;
        unsigned amount = intDist(engine);
        monitor.produce(args->id, amount);
    }

    pthread_exit(nullptr);
}

void *consumer(void *arg) {
    auto *args = (threadArgs*)arg;
    std::uniform_int_distribution<unsigned> intDist(args->minValue, args->maxValue);
    std::default_random_engine engine;

    while(true) {
        sleep(1);
        unsigned amount = intDist(engine);
        monitor.consume(args->id, amount);
    }

    pthread_exit(nullptr);
}

threadArgs getArgs(unsigned id, unsigned minValue, unsigned maxValue, StoreMonitor *monitor) {
    threadArgs args;
    args.id = id;
    args.minValue = minValue;
    args.maxValue = maxValue;
}
