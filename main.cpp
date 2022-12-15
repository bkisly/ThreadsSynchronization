#include <random>
#include <cstdlib>
#include <vector>

#include "unistd.h"
#include "include/fileManager.h"
#include "include/storeMonitor.h"
#include "include/threadArgs.h"

#define MAX_THREADS 128

void *producer(void *arg);
void *consumer(void *arg);
threadArgs getArgs(unsigned id, unsigned minValue, unsigned maxValue);

int main(int argc, char *argv[]) {
    writeToStore(0);
    monitor.setStoreCapacity(std::stoi(argv[1]));

    unsigned prodCount = std::stoi(argv[2]);
    unsigned consCount = std::stoi(argv[3]);

    pthread_t threads[MAX_THREADS];
    threadArgs args[MAX_THREADS];

    for(int i = 0; i < prodCount; i++) {
        args[i] = getArgs(
                i + 1,
                std::stoi(argv[4 + i * 2]),
                std::stoi(argv[5 + i * 2]));
    }

    for(int i = 0; i < consCount; i++) {
        args[i + prodCount] = getArgs(
                i + 1,
                std::stoi(argv[4 + 2 * (i + prodCount)]),
                std::stoi(argv[5 + 2 * (i + prodCount)]));
    }

    for(int i = 0; i < prodCount + consCount; i++) {
        if(i < prodCount) {
            pthread_create(&threads[i], nullptr, &producer, &args[i]);
        }
        else {
            pthread_create(&threads[i], nullptr, &consumer, &args[i]);
        }
    }

    pthread_exit(nullptr);
}

void *producer(void *arg) {
    auto *args = (threadArgs*)arg;
    std::uniform_int_distribution<unsigned> intDist(args->minValue, args->maxValue);
    std::default_random_engine engine;

    while(true) {
        sleep(1);
        unsigned amount = intDist(engine);
        monitor.produce(args->id, amount);
    }
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
}

threadArgs getArgs(unsigned id, unsigned minValue, unsigned maxValue) {
    threadArgs args;
    args.id = id;
    args.minValue = minValue;
    args.maxValue = maxValue;

    return args;
}
